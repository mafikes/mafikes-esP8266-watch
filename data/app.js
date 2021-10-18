document.addEventListener("DOMContentLoaded", function(event) { 
    loadData();
});

let _elements = {
    switchButtons: document.querySelectorAll(".js-switch-button"),
    brightness: document.querySelector(".js-slider-brightness"),
    brightnessAuto: document.querySelector(".js-brightness-auto"),
    showText: document.querySelector(".js-show-text-show"),
    weatherLocation: document.querySelector(".js-weather-location"),
    weatherApi: document.querySelector(".js-weather-api"),
    switchMainTime: document.querySelector(".js-view-switch-main"),
    switchAppTime: document.querySelector(".js-view-switch-app"),
    restartWatch: document.querySelector(".js-restart-device"),
    customColorWatch: document.querySelector('.js-custom-color-watch')
};

/**
 * Send Request Data
 * @param {*} method 
 * @param {*} data 
 * @param {*} callback 
 */
function sendRequest(method, data, callback) {
    let notify = document.querySelector('.modal_notification');
    notify.classList.add('show');

    let url =`/${method}${data}`;
    let xmlHttp = new XMLHttpRequest();
    xmlHttp.onreadystatechange = function (notify) {
   
    if (xmlHttp.readyState == 4 && xmlHttp.status == 200) {
        callback(xmlHttp.responseText);
        setTimeout(function() {
            document.querySelector('.modal_notification').classList.remove("show");
        }, 300);
    }
    };

    console.log(url);

    xmlHttp.open("GET", url, true);
    xmlHttp.send(null);
}

/**
 * Load data from device
 */
function loadData() {
    function rgbaToHex(rgba) {
        var inParts = rgba.substring(rgba.indexOf("(")).split(","),
            r = parseInt(trim(inParts[0].substring(1)), 10),
            g = parseInt(trim(inParts[1]), 10),
            b = parseInt(trim(inParts[2]), 10),
            a = parseFloat(trim(inParts[3].substring(0, inParts[3].length - 1))).toFixed(2);
        var outParts = [
          r.toString(16),
          g.toString(16),
          b.toString(16),
          Math.round(a * 255).toString(16).substring(0, 2)
        ];
      
        // Pad single-digit output values
        outParts.forEach(function (part, i) {
          if (part.length === 1) {
            outParts[i] = '0' + part;
          }
        })
      
        return ('#' + outParts.join(''));
    }

    sendRequest('config.json', '', (response) => {
        let data = JSON.parse(response);
        console.log('Config: success load data.', data);       
        
        _elements.brightness.value = data.brightness;
        _elements.brightnessAuto.value = data.brightness_auto == true ? 1 : 0;        

        _elements.switchMainTime.value = data.view_main_switch_time;
        _elements.switchAppTime.value = data.view_app_switch_time;

        _elements.weatherLocation.value = data.weather_location;
        _elements.weatherApi.value = data.weather_key;

        _elements.customColorWatch.value = rgbaToHex(data.watch_color);
    });
}

// SWITCH BUTTONS
_elements.switchButtons.forEach( (button, key) => {
    button.addEventListener("click", (event) => {
      let buttonIndex = event.target.dataset.button;
      if(buttonIndex) sendRequest('change-view', `?button=${buttonIndex}`, () => {});
      console.log(event.target.dataset.button);
    });
});

// BRIGHTNESS
_elements.brightness.addEventListener("change", (event) => {
    let value = event.target.value; 
    _elements.brightnessAuto.value = 0;
    sendRequest('brightness', `?value=${value}`, () => {});
});

// SHOW TEXT
_elements.showText.addEventListener("click", (event) => {
    let value = document.querySelector(".js-show-text-value").value; 
    if(value) sendRequest('show-text', `?value=${value}`, () => {});
});
document.querySelector(".js-show-text-clear").addEventListener("click", (event) => {
    if(document.querySelector(".js-show-text-value").value) {
        document.querySelector(".js-show-text-value").value = "";
        sendRequest('show-main-app', '', () => {});
    }
});

// SETTINGS
_elements.brightnessAuto.addEventListener("change", (event) => {
    let value = event.target.value;
    sendRequest('brightness-auto', `?value=${value}`, () => {});
});

_elements.weatherApi.addEventListener("change", (event) => {
    let value = event.target.value;
    sendRequest('change-weather-api', `?value=${value}`, () => {});
});

_elements.weatherLocation.addEventListener("change", (event) => {
    let value = event.target.value;
    sendRequest('change-weather-location', `?value=${value}`, () => {});
});

_elements.switchMainTime.addEventListener("change", (event) => {
    let value = event.target.value;
    sendRequest('switch-main-time', `?value=${value}`, () => {});
});

_elements.switchAppTime.addEventListener("change", (event) => {
    let value = event.target.value;
    sendRequest('switch-app-time', `?value=${value}`, () => {});
});

_elements.customColorWatch.addEventListener("change", (event) => {
    let color = event.target.value;

    let red = parseInt(color.slice(-6, -4), 16);
    let green = parseInt(color.slice(-4, -2), 16);
    let blue = parseInt(color.slice(-2), 16);

    if(red == 0 && green == 0 && blue == 0) return;

    sendRequest('custom-color-watch', `?red=${red}&green=${green}&blue=${blue}`, () => {});
});

// RELOAD 
_elements.restartWatch.addEventListener("click", (event) => {
    if(confirm('Are you sure with restart watch?')) {
        sendRequest('restart', ``, () => {});
    }
});

document.querySelector(".js-reload").addEventListener("click", (event) => {
    location.reload();
});