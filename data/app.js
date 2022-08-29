const control = () => {
  return {
    data: {
      brightness: 10,
      brightness_auto: 1,
      view_main_switch_time: 0,
      view_app_switch_time: 0,
      time_update: 1,
      weather_location: 0,
      weather_key: 0,
      time_offset: 0,
      show_text: "",
      custom_color_watch: null,
      loading: false,
      loadingTimeout: null,
      error: false,
      error_text: "",
      errorTimeout: null,
    },

    createLoading: function () {
      this.data.loading = true;
      clearTimeout(this.data.loadingTimeout);
    },

    clearLoading: function (timeout = 400) {
      let _this = this;
      this.data.loadingTimeout = setTimeout(function () {
        _this.data.loading = false;
      }, timeout);
    },

    simulateButtonClick: function (buttonIndex) {
      this.sendRequest("change-view", `?button=${buttonIndex}`);
    },

    showText: function () {
      if (!this.data.show_text) return;
      this.sendRequest("show-text", `?value=${this.data.show_text}`);
    },

    hideText: function () {
      this.data.show_text = "";
      this.sendRequest("show-main-app");
    },

    flashError: function (text) {
      this.data.loading = false;
      this.data.error = true;
      this.data.error_text = text;

      let _this = this;
      clearTimeout(this.data.errorTimeout);
      this.data.errorTimeout = setTimeout(function () {
        _this.data.error = false;
        console.log("aa");
      }, 2000);
    },

    setWatchColor: function () {
      let color = this.data.custom_color_watch;
      let red = parseInt(color.slice(-6, -4), 16);
      let green = parseInt(color.slice(-4, -2), 16);
      let blue = parseInt(color.slice(-2), 16);

      if (red == 0 && green == 0 && blue == 0) return;

      this.sendRequest(
        "custom-color-watch",
        `?red=${red}&green=${green}&blue=${blue}`
      );
    },

    syncTime: function () {
      this.sendRequest("sync-time");
    },

    customBrightness: function () {
      this.sendRequest("brightness", `?value=${this.data.brightness}`);
    },

    save: function () {
      let data = {
        brightness_auto: this.data.brightness_auto,
        custom_color_watch: this.data.custom_color_watch,
        time_offset: this.data.time_offset,
        view_main_switch_time: this.data.view_main_switch_time,
        view_app_switch_time: this.data.view_app_switch_time,
        weather_location: this.data.weather_location,
        weather_key: this.data.weather_key,
        time_update_interval: this.data.time_update_interval,
      };

      const xhr = new XMLHttpRequest();
      let _this = this;
      this.createLoading();

      xhr.onload = () => {
        if (xhr.status >= 200 && xhr.status < 300) {
          const response = JSON.parse(xhr.responseText);
          _this.clearLoading();
        }
      };

      xhr.open("POST", "/save");
      xhr.setRequestHeader("Content-Type", "application/json");
      xhr.send(JSON.stringify(data));
    },

    loadData: function () {
      this.createLoading();
      let _this = this;

      this.sendRequest("config.json", null, (response) => {
        let data = JSON.parse(response);
        console.log("CONFIG:", data);

        _this.data.brightness = data.brightness;
        _this.data.brightnessAuto = data.brightness_auto == true ? 1 : 0;

        _this.data.view_main_switch_time = data.view_main_switch_time;
        _this.data.switchAppTime = data.view_app_switch_time;

        _this.data.weather_location = data.weather_location;
        _this.data.weather_key = data.weather_key;

        _this.data.time_offset = data.time_offset;

        _this.data.custom_color_watch = _this.rgbToHex(
          data.watch_color[0],
          data.watch_color[1],
          data.watch_color[2]
        );

        this.clearLoading();
      });
    },

    sendRequest: function (method, data = null, callback = null) {
      this.createLoading();

      const url = data ? `/${method}${data}` : `/${method}`;
      let xhr = new XMLHttpRequest();
      let _this = this;

      xhr.onreadystatechange = function (notify) {
        if (xhr.readyState == 4 && xhr.status == 200) {
          if (typeof callback === "function") callback(xhr.responseText);
          _this.clearLoading();
        }
      };

      console.log("URL", url);

      xhr.open("GET", url, true);
      xhr.send();
    },

    rgbToHex: function (r, g, b) {
      r = r.toString(16);
      g = g.toString(16);
      b = b.toString(16);

      if (r.length == 1) r = "0" + r;
      if (g.length == 1) g = "0" + g;
      if (b.length == 1) b = "0" + b;

      return "#" + r + g + b;
    },

    restartDevice: function () {
      if (confirm("Are you sure with restart watch?")) {
        this.sendRequest("restart", null, () => {
          setTimeout(function () {
            location.reload();
          }, 12000);
        });
      }
    },

    reloadPage: () => {
      location.reload();
    },
  };
};
