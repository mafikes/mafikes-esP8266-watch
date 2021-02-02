#include <Display.h>
#include <Fonts/TomThumb.h>
#include <Fonts/Picopixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Config.h>
#include <Colors.h>

// MATRIX
#define MATRIX_PIN 2
#define MATRIX_WIDTH 32
#define MATRIX_HEIGHT 8
// #define MATRIX_MODE NEO_MATRIX_TOP + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE
#define MATRIX_MODE NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG
// #define MATRIX_MODE NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG
#define MATRIX_TYPE NEO_GRB + NEO_KHZ800

#define arrayLength(array) (sizeof((array))/sizeof((array)[0]))

Display::Display() : matrix(MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_PIN, MATRIX_MODE, MATRIX_TYPE) {    
    setup();
}

void Display::setup() {
    matrix.begin();  
    matrix.setTextWrap(false);

    setBrightness(30); 
}

void Display::refresh() {
   matrix.drawPixel(0, 8 - 1, matrix.Color(0, 0, 255));
   matrix.show();
}

void Display::clear() {
    matrix.fillScreen(0); 
}

void Display::setBrightness(int value) 
{
    brightness = value; 
    matrix.setBrightness(brightness);
}

uint32_t Display::color(DisplayColor color)
{
    return matrix.Color(color.red, color.green, color.blue);
}

void Display::drawText(String text, bool defaultFont, DisplayPosition pos, DisplayColor colorText) {
    if (defaultFont) {
        matrix.setFont();
        matrix.setCursor(pos.x, pos.y);
    }else{
        matrix.setFont(&TomThumb);
        matrix.setCursor(pos.x, pos.y+6);
    }

    matrix.print(text);  
    matrix.setTextWrap(false);
    matrix.setTextColor(color(colorText));    
}

void Display::fixdrawRGBBitmap(int16_t x, int16_t y, const uint32_t *bitmap, int16_t w, int16_t h) 
{
    uint16_t RGB_bmp_fixed[w * h];

    for (uint16_t pixel = 0; pixel < w * h; pixel++) {
        uint32_t a8b8g8r8 = bitmap[pixel];
        uint8_t r5 = ((a8b8g8r8 >>  0) & 0xFF) >> (8 - 5);
        uint8_t g6 = ((a8b8g8r8 >>  8) & 0xFF) >> (8 - 6);
        uint8_t b5 = ((a8b8g8r8 >> 16) & 0xFF) >> (8 - 5);
        RGB_bmp_fixed[pixel] = (r5 << 11) | (g6 << 5) | b5;
    }
    
    matrix.drawRGBBitmap(x, y, RGB_bmp_fixed, w, h);  
    // matrix.drawFastVLine(8, 0, 8, 0);
}

void Display::resetIconAnimation() 
{
    lastShowedIcon = 0;    
    repeatedAnimation = 0;
    lastIconAnimationCount = 0;
}

void Display::showIcon(const uint32_t bitmap[][64]) 
{
    fixdrawRGBBitmap(0, 0, bitmap[0], 8, 8);
}

void Display::showAnimateIcon(const uint32_t bitmap[][64], int animationCount, int animateSteps, int delayTime) 
{
    if(lastIconAnimationCount != animationCount) {
        resetIconAnimation();
    }

    if(animationCount > 1 && repeatedAnimation < animateSteps) {
        fixdrawRGBBitmap(0, 0, bitmap[lastShowedIcon], 8, 8);
        
        lastShowedIcon++;
        if (lastShowedIcon >= animationCount) { // animation is showed full, start it from zero
            lastShowedIcon = 0;
            repeatedAnimation++;
            delay(400);
        }
    } else {
        fixdrawRGBBitmap(0, 0, bitmap[0], 8, 8);
    }

    lastIconAnimationCount = animationCount;

    delay(delayTime);
}

void Display::resetLoading()
{
    loadingRepeated = 0;
}

void Display::showLoading(int delayTime)
{
    loadingRepeated++;

    int col = 0;
    int fullCol = 0;
    for(int i = 0; i < 12; i++) { 
        col++;

        if(col == 3) {
            col = 0;
            fullCol++;

            if(fullCol == loadingRepeated) {
                drawPixel(11+i-2, 3, COLOR_RED);
                drawPixel(11+i-2, 4, COLOR_RED);
                drawPixel(12+i-2, 3, COLOR_RED);
                drawPixel(12+i-2, 4, COLOR_RED);
            }
            continue;
        }

        drawPixel(11+i, 3, COLOR_WHITE);
        drawPixel(11+i, 4, COLOR_WHITE);
    }

    if(loadingRepeated == 4) { 
        loadingRepeated = 0;
    }
    
    delay(delayTime);
}

void Display::drawTextWithIcon(String text, DisplayPosition pos, DisplayColor colorText) 
{
    matrix.setFont(&TomThumb);
    matrix.setCursor(pos.x+8, pos.y+6);
    matrix.setTextColor(color(colorText));
    
    matrix.print(text);         
}

void Display::showTextWithIconAnimated(const uint32_t bitmap[][64], int iconSize, String text, DisplayPosition textPosition, DisplayColor textColor) 
{  
    fixdrawRGBBitmap(0, 0, bitmap[lastShowedIcon], 8, 8);

    lastShowedIcon++;
    if (lastShowedIcon >= iconSize) {
        lastShowedIcon = 0;
    }

    matrix.setFont(&TomThumb);
    matrix.setCursor(textPosition.x+8, textPosition.y+6);
    matrix.setTextColor(color(textColor));
    matrix.print(text);

    delay(300);
}

void Display::show() 
{
    matrix.show();
}

void Display::showLogo() {
    matrix.clear();
    matrix.setBrightness(40);

    matrix.setFont(&TomThumb);
    matrix.setTextColor(color(COLOR_WHITE));
    matrix.setCursor(2, 6);
    matrix.print("M");

    matrix.setTextColor(color(COLOR_RED));
    matrix.setCursor(8, 6);
    matrix.print("A");

    matrix.setTextColor(color(COLOR_WHITE));
    matrix.setCursor(12, 6);
    matrix.print("FIKES");

    drawLine(28, 6, 2, 6, {200,200,200});

    drawPixel(0, 0, COLOR_RED);
    drawPixel(0, 7, COLOR_RED);
    drawPixel(31, 0, COLOR_RED);
    drawPixel(31, 7, COLOR_RED);

    matrix.show();
}

void Display::drawPixel(uint16_t  x, uint16_t  y, DisplayColor pixelColor) {
    matrix.drawPixel(x, y, color(pixelColor));
}

void Display::drawLine(uint16_t  x0, uint16_t  y0,uint16_t  X1,uint16_t  Y1, DisplayColor lineColor) {
    matrix.drawLine(x0, y0, X1, Y1, color(lineColor));
    
}