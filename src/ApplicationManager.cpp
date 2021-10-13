#include <ApplicationManager.h>
#include <WString.h>

#include <Display.h>
#include <Config.h>

// Applications 
#include <WeatherApp.h>
#include <TimeApp.h>
#include <TemperatureApp.h>
#include <BrightnessApp.h>
#include <ShowTextApp.h>
#include <DrawApp.h>

// Buttons PIN
#define BTN1_PIN 13
#define BTN2_PIN 12
#define BTN3_PIN 15

void ApplicationManager::autoTimerAppSwitch() 
{    
    unsigned long currentMillis = millis();
    if ((unsigned long)(currentMillis - prevAppSwitchMillis) >= intervalSwitchApp) {
        ApplicationManager::getInstance().nextApp(false);

        if(storedAppView == activeAppView) {            
            resumeAutoSwitch();
        }

        prevAppSwitchMillis = currentMillis;
    }
}

void ApplicationManager::autoSwitchMainTimer() 
{
    unsigned long currentMillis = millis();
    if ((unsigned long)(currentMillis - prevMainSwitchMillis) >= intervalMainSwitchApp) {
        storedAppView = activeAppView;

        autoSwitchRun = true;

        prevMainSwitchMillis = currentMillis;
        prevAppSwitchMillis = currentMillis;
    }
}

void ApplicationManager::resumeAutoSwitch() 
{
    unsigned long currentMillis = millis();

    autoSwitchRun = false;
    canSwitchApp = true;
    prevMainSwitchMillis = currentMillis;
}

void ApplicationManager::setBrightness(int value)
{   
    Display::getInstance().setBrightness(value);
}

void ApplicationManager::btn1_process()
{
    resumeAutoSwitch();
    application->btn1_process();
}

void ApplicationManager::btn2_process()
{        
    resumeAutoSwitch();
    nextApp(true);
}

void ApplicationManager::btn3_process()
{
    resumeAutoSwitch();
    application->btn3_process();
}

void ApplicationManager::changeSwitchViewAuto(bool status)
{
    allowedSwitchApp = status;

    if(status) {
        resumeAutoSwitch();
    }   
} 

void ApplicationManager::loadSwichTime()
{
    Config& config = Config::getInstance();

    // Set intervals    
    intervalMainSwitchApp = config.data.view_main_switch_time;  
    intervalSwitchApp = config.data.view_app_switch_time; 

    resumeAutoSwitch();
}

void ApplicationManager::setup() 
{
    pinMode(BTN1_PIN, INPUT);
    pinMode(BTN2_PIN, INPUT);
    pinMode(BTN3_PIN, INPUT);

    Config& config = Config::getInstance();

    // Set intervals    
    loadSwichTime();

    changeSwitchViewAuto(config.data.view_auto_switch); 

    // Start first App
    application = new TimeApp();
    application->beforeRender();
}

void ApplicationManager::showText(String showText)
{
    canSwitchApp = false;

    application->clear();
    application = new ShowTextApp(showText, true, 100, COLOR_WHITE);
    application->beforeRender();
}

void ApplicationManager::showMainApp()
{
    resumeAutoSwitch();

    application->clear();
    activeAppView = 0;

    application = new TimeApp();
    application->beforeRender();
}

void ApplicationManager::nextApp(bool fromButton) 
{
    canSwitchApp = true;
    application->clear();
      
    activeAppView++;

    if(activeAppView == 1) {
        application = new WeatherApp();
    } else if(activeAppView == 2) {
        application = new TemperatureApp(0); // Local temperature
    } else if(activeAppView == 3) {
        application = new TemperatureApp(2); // Humidity
    } else if(activeAppView == 4 && fromButton) {
        application = new BrightnessApp();
    } else if(activeAppView == 5 && fromButton) {
        canSwitchApp = false;
        application = new ShowTextApp("IP:" + Config::getInstance().data.ip_address, false, 80, COLOR_RED);
    } else if(activeAppView == 6) {
        application = new DrawApp();
    } else {
        application = new TimeApp();
        activeAppView = 0;
    }

    application->beforeRender();
}   

void ApplicationManager::loop()
{
    if(allowedSwitchApp) {
        if(canSwitchApp && !autoSwitchRun) autoSwitchMainTimer();
        if(canSwitchApp && autoSwitchRun) autoTimerAppSwitch();
    }
 
    int btn1State = digitalRead(BTN1_PIN);
    int btn2State = digitalRead(BTN2_PIN);
    int btn3State = digitalRead(BTN3_PIN);

    if (btn1LastState == LOW && btn1State == HIGH)
    {
        btn1_process();
        Serial.println("TOUCHED 1");
    } 

    if (btn2LastState == LOW && btn2State == HIGH)
    {
        nextApp(true);
        Serial.println("TOUCHED 2");
    } 

    if (btn3LastState == LOW && btn3State == HIGH)
    {
        btn3_process();
        Serial.println("TOUCHED 3");
    } 

    btn1LastState = btn1State;
    btn2LastState = btn2State;
    btn3LastState = btn3State;

    application->render(Display::getInstance());
} 
