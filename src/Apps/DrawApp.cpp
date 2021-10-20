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

void DrawApp::test(Display& display) {   
    lastShowedFragment++;

    if(lastShowedFragment == 255) {
        lastShowedFragment = 0;
        display.clear();
    }

    display.matrix.drawPixel(random(0, 32), random(0, 8), display.color({random(0, 255), random(0, 255), random(0, 255)}));    
    
    display.matrix.drawPixel(random(0, 32), random(0, 8), display.color({random(0, 255), random(0, 255), random(0, 255)}));

    display.show();

    delay(250);
}

void DrawApp::light(Display& display) {   
    display.clear();
 
    display.matrix.fillScreen(display.color(COLOR_WHITE));

    display.show();
}

void DrawApp::beforeRender()
{
    Display::getInstance().clear();

    lastShowedFragment = 0;   
    position = {x: 0, y: 0};

    activeView = random(0, 1);
}

void DrawApp::render(Display& display) 
{       
    if(activeView == 1) {
        mario(display);
    } else if(activeView == 2) {
        light(display);
    } else {
        activeView = 0;
        test(display);
    }
}

void DrawApp::btn1_process()
{
    activeView++;
}

void DrawApp::btn3_process()
{
    activeView++;
}