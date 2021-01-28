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
    display.clear(); 
    display.showIcon(ICON_BRIGHTNESS);
    display.drawTextWithIcon("BRIGHT", {2, 0}, COLOR_WHITE);
    display.show();
}