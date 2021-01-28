
let _elements = {
    switchButtons: document.querySelectorAll(".js-switch-button"),
    brightness: document.querySelector(".js-slider-brightness"),
    watchType: document.querySelector(".js-watch-type"),
    watchColor: document.querySelector(".js-watch-color"),
    showText: document.querySelector(".js-show-text-show"),
    weatherApi: document.querySelector(".js-weather-api"),
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
    sendRequest('devices', "", (response) => {
        let data = JSON.parse(response);
        console.log('devices data', data);

        //TODO: set all page from config
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

// WATCH TYPE
_elements.watchType.addEventListener("change", (event) => {
    let value = event.target.value; 
    sendRequest('watch-type', `?value=${value}`, () => {});
});

// WATCH COLOR
_elements.watchColor.addEventListener("change", (event) => {
    let value = event.target.value; 
    sendRequest('watch-color', `?value=${value}`, () => {});
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
_elements.weatherApi.addEventListener("change", (event) => {
    let value = event.target.value;
    sendRequest('change-weather-api', `?value=${value}`, () => {});
});

// RELOAD 
document.querySelector(".js-reload").addEventListener("click", (event) => {
    location.reload();
});