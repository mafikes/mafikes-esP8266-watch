#include <OTA.h>
#include <Config.h>
#include <Updater.h>
#include <flash_hal.h>
#include "LittleFS.h"

#define OTA_TIMEOUT 15000 // ms without data, upload is aborted
#define OTA_REBOOT_DELAY 1000 // ms, time to send the response before restart

static const char UPDATE_PAGE[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>MafikesWatch Update</title>
<style>
body { font-family: sans-serif; background: #111; color: #eee; max-width: 420px; margin: 40px auto; padding: 0 16px; }
h1 span { color: #e33; }
label { display: block; margin: 16px 0 6px; }
select, input, button { width: 100%; padding: 10px; font-size: 16px; box-sizing: border-box; }
button { background: #e33; color: #fff; border: 0; border-radius: 4px; margin-top: 20px; cursor: pointer; }
button:disabled { opacity: .5; }
progress { width: 100%; height: 20px; margin-top: 20px; }
.note { color: #999; font-size: 13px; }
a { color: #999; }
</style>
</head>
<body>
<h1>Mafikes<span>Watch</span> update</h1>
<form id="form">
  <label>Type</label>
  <select id="type">
    <option value="firmware">Firmware (firmware.bin)</option>
    <option value="filesystem">Web UI / filesystem (littlefs.bin)</option>
  </select>
  <p class="note">Filesystem update keeps the current watch settings.</p>
  <label>File</label>
  <input type="file" id="file" accept=".bin" required>
  <button type="submit" id="button">Upload</button>
</form>
<progress id="progress" max="100" value="0" hidden></progress>
<p id="status"></p>
<p><a href="/">Back</a></p>
<script>
document.getElementById('form').onsubmit = function (e) {
  e.preventDefault();
  var file = document.getElementById('file').files[0];
  if (!file) return;

  var progress = document.getElementById('progress');
  var status = document.getElementById('status');
  var button = document.getElementById('button');
  var data = new FormData();
  data.append('update', file, file.name);

  var xhr = new XMLHttpRequest();
  xhr.upload.onprogress = function (e) {
    if (e.lengthComputable) progress.value = e.loaded / e.total * 100;
  };
  xhr.onload = function () {
    if (xhr.status === 200) {
      status.textContent = 'Done, watch is restarting...';
      setTimeout(function () { location.href = '/'; }, 10000);
    } else {
      status.textContent = 'Error: ' + xhr.responseText;
      button.disabled = false;
    }
  };
  xhr.onerror = function () {
    status.textContent = 'Upload failed, connection lost.';
    button.disabled = false;
  };

  button.disabled = true;
  progress.hidden = false;
  status.textContent = 'Uploading...';
  xhr.open('POST', '/update?type=' + document.getElementById('type').value);
  xhr.send(data);
};
</script>
</body>
</html>
)rawliteral";

void OTA::setup(AsyncWebServer& server)
{
    server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", UPDATE_PAGE);
    });

    server.on("/update", HTTP_POST,
        [](AsyncWebServerRequest *request) {
            OTA::getInstance().handleUploadDone(request);
        },
        [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
            OTA::getInstance().handleUpload(request, index, data, len, final);
        }
    );
}

// Runs in web server callback: no delay(), no yield()
void OTA::handleUpload(AsyncWebServerRequest *request, size_t index, uint8_t *data, size_t len, bool final)
{
    if(index == 0) {
        if(running || finished) {
            return; // Another update is already running
        }

        filesystem = request->hasParam("type") && request->getParam("type")->value() == "filesystem";
        uploadRequest = request;
        totalSize = request->contentLength();
        uploadedSize = 0;
        running = true;

        size_t maxSize;
        if(filesystem) {
            maxSize = FS_end - FS_start;
            LittleFS.end();
        } else {
            maxSize = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
        }

        Serial.printf("OTA: %s update started.\n", filesystem ? "Filesystem" : "Firmware");

        Update.runAsync(true);
        if(!Update.begin(maxSize, filesystem ? U_FS : U_FLASH)) {
            Serial.println("OTA: " + Update.getErrorString());
        }
    }

    if(!running || request != uploadRequest) {
        return;
    }

    lastDataTime = millis();

    if(!Update.hasError() && Update.write(data, len) != len) {
        Serial.println("OTA: " + Update.getErrorString());
    }

    uploadedSize = index + len;

    if(final) {
        running = false;

        if(Update.end(true)) {
            Serial.printf("OTA: Update finished, %u bytes.\n", index + len);
            finished = true;
            finishedTime = millis();
        } else {
            Serial.println("OTA: " + Update.getErrorString());
            uploadFailed();
        }
    }
}

// Runs in web server callback when the whole request is received
void OTA::handleUploadDone(AsyncWebServerRequest *request)
{
    bool success = finished && request == uploadRequest && !Update.hasError();

    AsyncWebServerResponse *response = request->beginResponse(
        success ? 200 : 500,
        "text/plain",
        success ? "OK" : "Update failed: " + Update.getErrorString()
    );
    response->addHeader("Connection", "close");
    request->send(response);
}

void OTA::uploadFailed()
{
    running = false;

    if(filesystem) {
        remountFilesystem = true;
    }
}

void OTA::loop()
{
    // Upload interrupted (closed browser, lost WiFi)
    if(running && millis() - lastDataTime > OTA_TIMEOUT) {
        Serial.println(F("OTA: Upload timeout, aborted."));
        Update.end();
        uploadFailed();
    }

    if(remountFilesystem) {
        remountFilesystem = false;
        LittleFS.begin();
    }

    if(finished && millis() - finishedTime > OTA_REBOOT_DELAY) {
        if(filesystem) {
            // New filesystem image contains its own config.json, keep the current settings
            LittleFS.begin();
            Config::getInstance().save();
        }

        Serial.println(F("OTA: Restarting."));
        ESP.restart();
    }
}

bool OTA::isRunning()
{
    return running || finished;
}

int OTA::progress()
{
    if(finished) {
        return 100;
    }

    if(totalSize == 0) {
        return 0;
    }

    return min(100, (int)((uint64_t)uploadedSize * 100 / totalSize));
}
