# MafikesWatch

A homemade LaMetric-style clock: 32×8 LED matrix driven by a **Wemos D1 mini (ESP8266)**, Arduino framework, PlatformIO.
Shows time, weather (OpenWeatherMap), and temperature/humidity from a DHT11. Controlled by three buttons and a web UI.

## Build and upload

The PlatformIO CLI is not on PATH, use the full path:

```sh
~/.platformio/penv/bin/pio run                    # build
~/.platformio/penv/bin/pio run -t upload          # upload firmware (USB, /dev/cu.usbserial-110)
~/.platformio/penv/bin/pio run -t uploadfs        # upload data/ to LittleFS (web UI + config)
~/.platformio/penv/bin/pio device monitor         # Serial 115200
```

`data/config.json` is in `.gitignore` (contains the weather API key), the template is `data/config.example.json`. Without a config everything loads as zeros.
`uploadfs` overwrites the config on the device, including settings saved from the web UI.

## Hardware (pins)

| What | Pin |
|---|---|
| WS2812 LED matrix 32×8 (column zigzag) | GPIO2 (D4) |
| Button 1 / 2 / 3 | GPIO13 (D7) / GPIO12 (D6) / GPIO15 (D8) |
| DHT11 | GPIO14 (D5) |
| Photoresistor (auto brightness) | A0 |
| RTC DS3231 | I2C (D1/D2) |

Power: 256 LEDs at high brightness draw several amps. A brownout is a possible restart cause that is invisible in code.

## Architecture

- `src/main.cpp`: `setup()` initializes Config → Display → WiFi → RTC → NTP → ApplicationManager. `loop()` calls `wifi.loop()`, `config.loop()`, `ntp.loop()`, auto brightness and `applicationManager.loop()`.
- Most classes are singletons (`X::getInstance()`).
- **ApplicationManager** (`src/ApplicationManager.cpp`) owns the active app (`ApplicationView*`), reads buttons, switches apps and renders (max 50 FPS, `FRAME_TIME`).
  - App order (`nextApp`): 0 Time → 1 Weather → 2 Temperature → 3 Humidity → 4 Brightness (button only) → 5 IP address (button only).
  - Auto switching: after `view_main_switch_time` ms it cycles through the apps every `view_app_switch_time` ms and returns to the original one.
  - Button 2 = next app, buttons 1 and 3 are forwarded to the active app (`btn1_process`/`btn3_process`).
  - Buttons are sampled every 30 ms (`BUTTON_READ_TIME`), which acts as debounce.
  - Apps are switched **only** via `setApplication(app, transition)`, which deletes the old app and starts the slide transition.
- **Apps** (`src/Apps/`, derive from `include/ApplicationView.h`): `beforeRender()`, `render(Display&)`, `clear()`, `btn1_process()`, `btn3_process()`.
  - `TimeApp`: 5 clock themes (button 3) and colors (button 1), saved to config. Hours and minutes use a rolling digit animation (`drawTime`): each changed digit (including seconds in theme 1) slides up and the new one comes from the bottom (`DIGIT_ANIMATION_TIME` 350 ms, ease-out). Digits are positioned using AwtrixFont advances (digit 4 px, colon 2 px).
  - `WeatherApp`: OpenWeatherMap API, retry at most every 30 s, 3 s HTTP timeout.
  - `TemperatureApp(view)`: 0 temperature, 1 heat index, 2 humidity. DHT is read every 2 s.
  - `BrightnessApp`, `ShowTextApp` (scrolling text).
- **Display** (`src/Display.cpp`):
  - Everything is drawn into an in-memory `GFXcanvas16 canvas`, not directly to the matrix.
  - `show()` composes the frame (including a running transition) and pushes it to the LEDs **only when it changed**. Pushing to the LEDs disables interrupts for ~8 ms and disturbs WiFi. `setBrightness()` forces the next push.
  - Slide transition: `startTransition(TRANSITION_SLIDE_UP/DOWN)` stores the last frame, then `show()` slides the old frame out and the new one in over `TRANSITION_TIME` (350 ms, ease-out). Next app = up, back to clock = down.
  - Icon animation (`showAnimateIcon`) and loading animation (`showLoading`) are timed with `millis()`.
  - Icons live in `include/Icons.h` (8×8 frame arrays), font is `src/AwtrixFont.h`.
- **Wifi** (`src/Wifi.cpp`): ESPAsyncWiFiManager (AP `MAFIKESWATCH` on first boot), mDNS `mafikeswatch.local`, AsyncWebServer serving the web UI from LittleFS.
- **Config** (`src/Config.cpp`): `/config.json` in LittleFS, ArduinoJson 6, `StaticJsonDocument<1024>`. The capacity must also cover copied key names; check it when adding keys, otherwise the config stops loading.
- **Temperature**: the DHT11 sits in an enclosure heated by the electronics, hence `temperature_offset` (°C, set from the web UI, default 0). The user sets −2 based on a reference thermometer. Displayed rounded to whole degrees (`getStringRounded(..., 5, 0)`); decimals were deliberately not added because DHT11 only has 1 °C resolution and ±2 °C accuracy.
- **Time**: the RTC DS3231 is the time source, NTP (`tik.cesnet.cz`, custom `NTPClient`) syncs it every `time_update_interval` hours. `time_offset` is in whole hours, no DST.
- **Web UI** (`data/`): Alpine.js, `app.js` calls the GET endpoints.

### Web API (all GET)

`/change-view?button=1|2|3`, `/brightness?value=`, `/show-text?value=`, `/show-main-app`, `/save?...` (all settings), `/switch-app-time?value=`, `/custom-color-watch?red=&green=&blue=`, `/sync-time`, `/restart`, `/info` (free heap), `/config.json` (static file).

## ⚠️ Important rules (causes of past crashes)

1. **AsyncWebServer callbacks do not run in `loop()`.** They must not call `delay()`/`yield()`, write to LittleFS, change the active app or draw. They only record a request (`requestButton`, `requestShowText`, `requestShowMainApp`, `requestLoadSwitchTime`, `Config::requestSave`, `ServiceNTP::requestUpdate`, `restartRequested` flag), which is executed in `loop()`.
2. **No `delay()` in app rendering.** Use `millis()` for timing, otherwise buttons and transitions stall.
3. Create apps with `new` and pass them to `setApplication()`; never assign `application` directly (memory leak).
4. Always check web parameters with `hasParam()` (null pointer).
5. Pass `String` to `printf`/`snprintf` via `.c_str()`.
6. Keep `String` usage low (heap fragmentation; the ESP8266 has only ~30–40 KB free with the web server running).

## Change history (with Claude, 2026-11)

The initial review identified these main restart causes:
- memory leak (apps created with `new`, never deleted),
- `delay()` inside the `/sync-time` callback,
- null pointer in `/save`,
- `String` passed to `snprintf` in the weather app,
- `while` loop without `delay` waiting for WiFi in NTP.

Fixed:
- **Restarts:** `setApplication()` with `delete`; queue of web requests processed in `loop()`; `hasParam` fallbacks in `/save`; weather request with `.c_str()`, `http.end()`, timeout and retry throttling; `delay` in the NTP WiFi wait and a minimum NTP interval of 1 h; restart from the web waits 0.5 s and saves pending config.
- **Slide transitions** between apps (canvas + frame composition), removed all `delay()` from rendering, LEDs pushed only on change, button debounce, DHT read every 2 s, removed Serial spam.
- **Temperature correction** `temperature_offset` (config, web UI, `TemperatureApp::readData`), config capacity raised to 1024.

- **Rolling digits** in `TimeApp` when hours/minutes change, `Display::drawChar()` helper.

Status: slide transitions and stability fixes tested on hardware, look good. Rolling digits compile, not yet tested on hardware.

## Known bugs (not fixed yet)

- `TimeApp::getColor()`: `== 4` appears twice, so green is unreachable.
- `TimeApp` theme 4: `activeOffset` for the day of week is computed as negative (wrong).
- `BrightnessApp::btn3_process`: `> 7` allows index 7 in a 7-element array, should be `>= 7`. Brightness set via buttons is not saved and auto brightness overrides it.
- `Wifi.cpp`: `if (MDNS.begin(...), WiFi.localIP())` uses the comma operator. The IP is read before waiting for the connection. `LittleFS.begin()` is called twice.
- `data/app.js`: `rgbToHex` has no `return`. Text for `/show-text` is not passed through `encodeURIComponent`.
- Config parse failure → all zeros (`view_app_switch_time = 0` switches constantly). No defaults.
- `time_offset` from `/save` reaches the NTP client only after a restart.
- `config.json` (with the API key) is publicly readable on the LAN.

## Ideas for further work

- **OTA** (ArduinoOTA via `upload_protocol = espota`, or ElegantOTA at `/update`); there is enough flash. Recommended: ArduinoOTA with a password, first flash via USB.
- Diagnostics: `ESP.getResetReason()`, `getHeapFragmentation()`, `getMaxFreeBlockSize()` in `/info`; `monitor_filters = esp8266_exception_decoder`.
- Time zones with DST: `configTime("CET-1CEST,M3.5.0,M10.5.0/3", ...)` instead of the custom NTPClient. The `Timezone` library in `lib_deps` is unused.
- Better temperature sensor: SHT40/SHT31 or BME280 on the existing I2C bus (shared with the RTC), mounted outside the enclosure.
- MQTT / Home Assistant (notifications, custom apps), night mode, alarm/timer, weather forecast, counters (YouTube, crypto), choosing clock theme/color and app order from the web UI.
