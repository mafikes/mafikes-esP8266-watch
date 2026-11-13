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

// Buttons PIN
#define BTN1_PIN 13
#define BTN2_PIN 12
#define BTN3_PIN 15

#define FRAME_TIME 20 // ms, max 50 FPS
#define BUTTON_READ_TIME 30 // ms, debounce

void ApplicationManager::autoTimerAppSwitch() 
{    
    unsigned long currentMillis = millis();

    if ((unsigned long)(currentMillis - prevAppSwitchMillis) >= intervalSwitchApp) {
        ApplicationManager::getInstance().nextApp(false);

        Serial.println("Fired: switch app view");
        Serial.println(storedAppView);
        Serial.println(activeAppView);

        if(storedAppView == activeAppView) {            
            resumeAutoSwitch();
        } else {
            prevAppSwitchMillis = currentMillis;
        }
    }
}

void ApplicationManager::autoSwitchMainTimer() 
{
    unsigned long currentMillis = millis();

    if ((unsigned long)(currentMillis - prevMainSwitchMillis) >= intervalMainSwitchApp) {
        storedAppView = activeAppView;

        Serial.println("Fired: auto main timer switch view.");

        autoSwitchRun = true;

        prevMainSwitchMillis = currentMillis;
        prevAppSwitchMillis = currentMillis;
    }
}

void ApplicationManager::resumeAutoSwitch() 
{
    Serial.println("Fired: resume auto switch.");

    unsigned long currentMillis = millis();

    autoSwitchRun = false;
    canSwitchApp = true;

    prevMainSwitchMillis = currentMillis;    
}

void ApplicationManager::setApplication(ApplicationView* newApplication, DisplayTransition transition)
{
    if(application) {
        application->clear();
        delete application;
    }

    application = newApplication;
    application->beforeRender();

    if(transition != TRANSITION_NONE) {
        Display::getInstance().startTransition(transition);
    }
}

void ApplicationManager::requestButton(int button)
{
    pendingButton = button;
}

void ApplicationManager::requestShowText(String text)
{
    pendingText = text;
    pendingShowText = true;
}

void ApplicationManager::requestShowMainApp()
{
    pendingShowMainApp = true;
}

void ApplicationManager::requestLoadSwitchTime()
{
    pendingLoadSwitchTime = true;
}

void ApplicationManager::processPendingActions()
{
    if(pendingLoadSwitchTime) {
        pendingLoadSwitchTime = false;
        loadSwichTime();
    }

    if(pendingShowMainApp) {
        pendingShowMainApp = false;
        showMainApp();
    }

    if(pendingShowText) {
        pendingShowText = false;
        showText(pendingText);
    }

    if(pendingButton) {
        int button = pendingButton;
        pendingButton = 0;

        if(button == 1) {
            btn1_process();
        } else if(button == 2) {
            btn2_process();
        } else if(button == 3) {
            btn3_process();
        }
    }
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
    setApplication(new TimeApp(), TRANSITION_SLIDE_DOWN); // Logo falls out
}

void ApplicationManager::showText(String showText)
{
    canSwitchApp = false;

    setApplication(new ShowTextApp(showText, true, 100, COLOR_WHITE), TRANSITION_SLIDE_DOWN);
}

void ApplicationManager::showMainApp()
{
    resumeAutoSwitch();

    activeAppView = 0;
    setApplication(new TimeApp(), TRANSITION_SLIDE_DOWN);
}

void ApplicationManager::nextApp(bool fromButton) 
{
    Config& config = Config::getInstance();

    canSwitchApp = true;
      
    activeAppView++;

    ApplicationView* nextApplication;

    if(activeAppView == 1) {
        nextApplication = new WeatherApp();
    } else if(activeAppView == 2) {
        nextApplication = new TemperatureApp(0); // Local temperature
    } else if(activeAppView == 3) {
        nextApplication = new TemperatureApp(2); // Humidity
    } else if(activeAppView == 4 && fromButton) {
        nextApplication = new BrightnessApp();
    } else if(activeAppView == 5 && fromButton) {
        canSwitchApp = false;
        nextApplication = new ShowTextApp("IP:" + Config::getInstance().data.ip_address, false, 80, COLOR_RED);
    } else {
        nextApplication = new TimeApp();
        activeAppView = 0;
    }

    if(config.data.view_auto_switch && fromButton) {
        resumeAutoSwitch();        
    }
    
    setApplication(nextApplication, TRANSITION_SLIDE_DOWN);
}   

void ApplicationManager::loop()
{
    processPendingActions();

    if(allowedSwitchApp) {
        if(canSwitchApp && !autoSwitchRun) {
            autoSwitchMainTimer();
        }
        
        if(canSwitchApp && autoSwitchRun) {
            autoTimerAppSwitch();
        }
    }

    unsigned long currentMillis = millis();

    // Sampling buttons only every few ms works as debounce
    if(currentMillis - prevButtonMillis >= BUTTON_READ_TIME) {
        prevButtonMillis = currentMillis;

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
    }
    if(currentMillis - prevRenderMillis >= FRAME_TIME) {
        prevRenderMillis = currentMillis;
        application->render(Display::getInstance());
    }
} 
