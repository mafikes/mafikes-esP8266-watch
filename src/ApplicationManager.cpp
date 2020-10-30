#include <ApplicationManager.h>

#include <Config.h>
#include <Display.h>
#include <WString.h>

// Applications 
#include <WeatherApp.h>
#include <TimeApp.h>
#include <Temperature.h>
#include <BrightnessApp.h>

// Buttons PIN
#define BTN1_PIN 13
#define BTN2_PIN 12
#define BTN3_PIN 15

void ApplicationManager::btn1_process()
{
    application->btn1_process();
}

void ApplicationManager::btn2_process()
{
    nextApp();
}

void ApplicationManager::btn3_process()
{
    application->btn3_process();
}

void ApplicationManager::setup() 
{
    pinMode(BTN1_PIN, INPUT);
    pinMode(BTN2_PIN, INPUT);
    pinMode(BTN3_PIN, INPUT);

    application = new TimeApp();
    application->beforeRender();
}

void ApplicationManager::nextApp() 
{
    application->clear();  
    activeAppView++;

    if(activeAppView == 1) {
        application = new WeatherApp();
    } else if(activeAppView == 2) {
        application = new Temperature();
    } else if(activeAppView == 3) {
        application = new BrightnessApp();
    } else {
        application = new TimeApp();
        activeAppView = 0;
    }

    application->beforeRender();
}   

void ApplicationManager::loop()
{
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
        nextApp();
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
