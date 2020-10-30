#include <BrightnessApp.h>
#include <Config.h>
#include <Display.h>
#include <Colors.h>
#include <WString.h>
#include <Helper.h>
#include <Icons.h>
#include <cmath>

#define arrayLength(array) (sizeof((array))/sizeof((array)[0]))

void BrightnessApp::beforeRender() 
{

} 

void BrightnessApp::btn1_process() 
{
    int newBrightness = BRIGHTNESS - 25;
    
    if(newBrightness == 0) {
        Display::getInstance().setBrightness(1);
    } else if(newBrightness >= 1) { 
        Display::getInstance().setBrightness(newBrightness);
    }
} 

void BrightnessApp::btn3_process() 
{
    if(BRIGHTNESS == 1) BRIGHTNESS = 0;

    int newBrightness = BRIGHTNESS + 25;
    
    if(newBrightness <= 250) {
        Display::getInstance().setBrightness(newBrightness);
    }
} 

void BrightnessApp::render(Display& display) 
{
    percentage = (BRIGHTNESS/250.0f)*100.0f;
    if(BRIGHTNESS == 1) percentage = 1;

    Serial.println(percentage);

    display.clear(); 
    display.showIcon(ICON_BRIGHTNESS, arrayLength(ICON_BRIGHTNESS), 0);
    display.drawTextWithIcon(Helper::getInstance().getStringRounded(percentage, 5, 0) + "%", {0, 0}, COLOR_WHITE);
    display.show();
}