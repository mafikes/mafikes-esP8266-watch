# MafikesWatch

A DIY smart pixel clock inspired by LaMetric Time, built on a **Wemos D1 mini (ESP8266)** and a **32×8 WS2812 LED matrix**.
It shows the time, weather and indoor climate, switches between apps with smooth slide animations, and is controlled
by three hardware buttons and a web interface on your local network.

## Features

- **Clock** with 5 themes (time with seconds, time only, calendar tile with day of month, day-of-week indicator, ...) and selectable or custom colors
- **Rolling digit animation**: when the time changes, the old digit slides up and the new one comes in from the bottom
- **Weather** from OpenWeatherMap with animated icons (sun, clouds, rain, thunder, snow, mist)
- **Indoor temperature and humidity** from a DHT11 sensor, with a configurable temperature correction
- **Slide transitions** between apps: the old screen slides out while the new one slides in
- **Automatic app switching**: periodically cycles through the apps and returns to the clock
- **Auto brightness** based on a photoresistor, or manual brightness
- **Accurate time** from a DS3231 RTC (keeps time without internet), synchronized via NTP
- **Web interface** at `http://mafikeswatch.local`: settings, virtual buttons, scrolling text messages, custom clock color, restart
- **Over-the-air updates** of firmware and web UI at `/update`, with a progress bar on the display
- **Easy WiFi setup**: on first boot the clock creates a `MAFIKESWATCH` access point with a configuration portal

## Hardware

| Part | Notes |
|---|---|
| Wemos D1 mini (ESP8266) | 4 MB flash |
| WS2812B LED matrix 32×8 | 256 LEDs, column zigzag layout |
| DS3231 RTC module | I2C |
| DHT11 sensor | temperature and humidity |
| Photoresistor (LDR) | with a voltage divider to A0 |
| 3 push buttons | active HIGH (with pull-down resistors) |
| 5 V power supply | see the power note below |

### Wiring

| Component | D1 mini pin | GPIO |
|---|---|---|
| LED matrix data | D4 | GPIO2 |
| Button 1 | D7 | GPIO13 |
| Button 2 | D6 | GPIO12 |
| Button 3 | D8 | GPIO15 |
| DHT11 data | D5 | GPIO14 |
| Photoresistor | A0 | A0 |
| DS3231 SCL / SDA | D1 / D2 | GPIO5 / GPIO4 |

**Power:** 256 LEDs at high brightness can draw several amps. Use a sufficiently strong 5 V supply, a 1000 µF capacitor
across the matrix power input and a ~330 Ω resistor on the data line. An undersized supply causes brownout resets.

**Temperature sensor placement:** the ESP8266 and the LEDs heat up the enclosure. Mount the DHT11 away from them
(ideally outside or at the bottom of the case) and fine-tune the reading with the temperature correction setting.

## Getting started

### Requirements

- [PlatformIO](https://platformio.org/) (VS Code extension or CLI)
- An [OpenWeatherMap API key](https://openweathermap.org/api) (free tier) for the weather app

### Build and flash

```sh
git clone git@github.com:mafikes/mafikes-esP8266-watch.git
cd mafikes-esP8266-watch

cp data/config.example.json data/config.json   # default settings

pio run -t upload      # build and flash the firmware over USB
pio run -t uploadfs    # flash the web UI and config to LittleFS
pio device monitor     # serial log at 115200 baud
```

Adjust `upload_port` / `monitor_port` in `platformio.ini` to match your USB serial port.

### First boot

1. The clock creates a WiFi access point **`MAFIKESWATCH`**. Connect to it and choose your home WiFi in the portal.
2. After connecting, open **`http://mafikeswatch.local`** (or the IP address, shown by the IP app, see buttons below).
3. Enter your OpenWeatherMap API key and location in the web interface.

### Updating over WiFi

After the first USB flash, updates can be done wirelessly:

1. Build the firmware (`pio run`) and/or the web UI image (`pio run -t buildfs`).
2. Open `http://mafikeswatch.local/update`.
3. Upload `.pio/build/d1_mini/firmware.bin` (Firmware) or `.pio/build/d1_mini/littlefs.bin` (Web UI / filesystem).

The clock shows the progress and restarts when done. A filesystem update keeps your current settings.

## Usage

### Buttons

| Button | Action |
|---|---|
| Button 1 | Clock: change color · Brightness app: darker |
| Button 2 | Next app |
| Button 3 | Clock: change theme · Brightness app: brighter |

Apps in order: **Clock → Weather → Temperature → Humidity → Brightness → IP address**.
Brightness and IP address are only reachable with the button, automatic switching skips them.

### Web interface

The web UI (Alpine.js, served from LittleFS) lets you:

- press the virtual buttons
- set brightness or enable auto brightness
- show a scrolling text message on the display
- pick a custom clock color
- set NTP sync interval, time offset and app switching intervals
- set the weather location and API key
- set the temperature correction
- restart the clock, export the config and open the firmware update page

### HTTP API

All endpoints are `GET` unless noted:

| Endpoint | Description |
|---|---|
| `/change-view?button=1\|2\|3` | Simulate a button press |
| `/brightness?value=1-130` | Set manual brightness |
| `/show-text?value=Hello` | Show a scrolling text |
| `/show-main-app` | Return to the clock |
| `/custom-color-watch?red=&green=&blue=` | Set a custom clock color |
| `/switch-app-time?value=ms` | Set the app switching interval |
| `/save?...` | Save all settings (see `data/app.js`) |
| `/sync-time` | Sync time with NTP now |
| `/restart` | Restart the clock |
| `/info` | Free heap memory |
| `/config.json` | Current configuration |
| `/update` | Update page (`POST` uploads firmware or filesystem) |

> The web interface and API have no authentication. Anyone on your local network can change settings or upload firmware.

## Configuration

Settings are stored in `/config.json` on LittleFS and are editable from the web UI. See `data/config.example.json`:

| Key | Description |
|---|---|
| `brightness`, `brightness_auto` | Manual brightness (1–130) and auto brightness switch |
| `watch_type`, `watch_type_color` | Clock theme and preset color |
| `watch_color_custom`, `watch_color` | Use a custom RGB clock color |
| `view_auto_switch` | Enable automatic app switching |
| `view_main_switch_time` | How long the clock stays before cycling apps (ms) |
| `view_app_switch_time` | How long each app is shown while cycling (ms) |
| `weather_key`, `weather_location` | OpenWeatherMap API key and city ID |
| `time_offset` | Time zone offset in hours |
| `time_update_interval` | NTP sync interval in hours |
| `temperature_offset` | Temperature sensor correction in °C |

`data/config.json` is git-ignored because it contains your API key.

## Project structure

```
include/              headers
src/
  main.cpp            setup and main loop
  ApplicationManager  app switching, buttons, rendering, transitions
  Display             drawing into an in-memory canvas, slide transitions, icons
  Wifi                WiFi manager, mDNS, web server and API
  Config              config.json load/save
  Apps/               Time, Weather, Temperature, Brightness, ShowText apps
  Components/         RTC, NTP service, auto brightness, OTA updates
data/                 web UI (index.html, app.js, app.css) and config
```

New apps implement the `ApplicationView` interface (`beforeRender`, `render`, button handlers) and are registered
in `ApplicationManager::nextApp`.

## Built with

- [Adafruit NeoMatrix / NeoPixel / GFX](https://github.com/adafruit/Adafruit_NeoMatrix)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) and [ESPAsyncWiFiManager](https://github.com/alanswx/ESPAsyncWiFiManager)
- [ArduinoJson](https://arduinojson.org/)
- [RTClib](https://github.com/adafruit/RTClib), [DHT sensor library](https://github.com/adafruit/DHT-sensor-library)
- [Alpine.js](https://alpinejs.dev/) for the web interface
- Pixel font from the [AWTRIX](https://github.com/awtrix) project, NTP client based on [arduino-libraries/NTPClient](https://github.com/arduino-libraries/NTPClient)
