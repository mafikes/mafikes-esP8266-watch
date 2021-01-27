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
    activeBrightness--;
    if(activeBrightness < 0) activeBrightness = 0;
    Display::getInstance().setBrightness(brightness[activeBrightness]);
} 

void BrightnessApp::btn3_process() 
{
    activeBrightness++;
    if(activeBrightness > 10) activeBrightness = 10;
    Display::getInstance().setBrightness(brightness[activeBrightness]);
} 

void BrightnessApp::render(Display& display) 
{
    percentage = ((float)activeBrightness/(float)arrayLength(brightness))*100.0f;
    // if(BRIGHTNESS == 1) percentage = 1;

    // Serial.println(arrayLength(brightness));
    // Serial.println(percentage);

    display.clear(); 
    display.drawTextWithIcon(Helper::getInstance().getStringRounded(percentage, 5, 0) + "%", {0, 0}, COLOR_WHITE);
    display.show();
}