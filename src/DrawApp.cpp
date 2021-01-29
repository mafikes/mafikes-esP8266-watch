#include <DrawApp.h>
#include <Config.h>
#include <Display.h>
#include <Colors.h>
#include <WString.h>
#include <Icons.h>

#define arrayLength(array) (sizeof((array))/sizeof((array)[0]))

void DrawApp::mario(Display& display) 
{
    int animationCount = arrayLength(RUN_MARIO);
    position.x++;
 
    display.clear();
    
    display.fixdrawRGBBitmap(position.x, position.y, RUN_MARIO[lastShowedFragment], 8, 8);
    
    lastShowedFragment++;
    if (lastShowedFragment >= animationCount) { // animation is showed full, start it from zero
        lastShowedFragment = 0;
    }

    if(position.x == 32) {
        position.x = 0;
    }   

    display.show();

    delay(80);
}

void DrawApp::beforeRender()
{
    lastShowedFragment = 0;   
    position = {x: 0, y: 0};
}

void DrawApp::render(Display& display) 
{       
    if(activeView == 0) {
        mario(display);
    }    
}