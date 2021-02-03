#include <ShowTextApp.h>
#include <Config.h>
#include <Display.h>
#include <Colors.h>
#include <WString.h>

#define MATRIX_WIDTH 32;
unsigned long previousAnimation = 0;

ShowTextApp::ShowTextApp(String _showText, bool useDefaultFont, int speed, DisplayColor color)
{
    defaultFont = useDefaultFont;
    textColor = color;
    animationSpeed = speed;
    showText = _showText;
    
    if(defaultFont) {
        showTextLenght = _showText.length() * 7;
    } else {
        showTextLenght = _showText.length() * 5;
    }

    position = MATRIX_WIDTH;
}

void ShowTextApp::beforeRender()
{
    
}

void ShowTextApp::render(Display& display) 
{  
    if(position == -showTextLenght) {
        position = MATRIX_WIDTH;
    }
    
    display.clear();
    display.drawText(showText, defaultFont, {position, 0}, textColor);    
    display.show();

    position--;

    delay(animationSpeed);
}