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
    switchViewAuto: document.querySelector(".js-view-switch-auto"),
    switchMainTime: document.querySelector(".js-view-switch-main"),
    switchAppTime: document.querySelector(".js-view-switch-app"),
    restartWatch: document.querySelector(".js-restart-device"),
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
    sendRequest('config.json', '', (response) => {
        let data = JSON.parse(response);
        console.log('Config: success load data.', data);       
        
        _elements.brightness.value = data.brightness;
        _elements.brightnessAuto.value = data.brightness_auto == true ? 1 : 0;        
        _elements.switchViewAuto.value = data.view_auto_switch == true ? 1 : 0; 

        _elements.switchMainTime.value = data.view_main_switch_time;
        _elements.switchAppTime.value = data.view_app_switch_time;

        _elements.weatherLocation.value = data.weather_location;
        _elements.weatherApi.value = data.weather_key;
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

_elements.switchViewAuto.addEventListener("change", (event) => {
    let value = event.target.value;
    sendRequest('switch-view-auto', `?value=${value}`, () => {});
});

_elements.weatherApi.addEventListener("change", (event) => {
    let value = event.target.value;
    sendRequest('change-weather-api', `?value=${value}`, () => {});
});

_elements.weatherLocation.addEventListener("change", (event) => {
    let value = event.target.value;
    sendRequest('change-weather-location', `?value=${value}`, () => {});
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