#include <Display.h>
#include <Fonts/TomThumb.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Config.h>

// MATRIX
#define MATRIX_PIN 2
#define MATRIX_WIDTH 32
#define MATRIX_HEIGHT 8
#define MATRIX_MODE NEO_MATRIX_TOP + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE
#define MATRIX_TYPE NEO_GRB + NEO_KHZ800

Display::Display() : matrix(MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_PIN, MATRIX_MODE, MATRIX_TYPE) {    
    setup();
}

void Display::setup() {
    matrix.begin();  
    matrix.setTextWrap(false);
    
    matrix.setFont(&TomThumb);
    matrix.setTextColor(matrix.Color(255,255,255));
    setBrightness(BRIGHTNESS);

    clear();
}

void Display::refresh() {
   matrix.drawPixel(0, 8 - 1, matrix.Color(0, 0, 255));
   matrix.show();
}

void Display::clear() {
    matrix.fillScreen(0); 
}

void Display::setBrightness(int value) {
    BRIGHTNESS = value;
    matrix.setBrightness(BRIGHTNESS);
}

uint32_t Display::color(DisplayColor color)
{
  return matrix.Color(color.red, color.green, color.blue);
    
}

void Display::drawText(String text, bool smallText, DisplayPosition pos, DisplayColor colorText) {
    if (smallText) {
        matrix.setFont(&TomThumb);
        matrix.setCursor(pos.x, pos.y+6);
    }else{
        matrix.setFont();
        matrix.setCursor(pos.x, pos.y);
    }

    matrix.print(text);  

    matrix.setTextColor(color(colorText));
    matrix.show();
}

void Display::showLogo() {
    int height = 6;
    matrix.setFont(&TomThumb);
    matrix.setTextColor(matrix.Color(255,255,255));
    matrix.setCursor(2, height);
    matrix.print("M");

    matrix.setTextColor(matrix.Color(255,0,0));
    matrix.setCursor(6, height);
    matrix.print("A");

    matrix.setTextColor(matrix.Color(255,255,255));
    matrix.setCursor(10, height);
    matrix.print("F");

    matrix.setTextColor(matrix.Color(255,255,255));
    matrix.setCursor(14, height);
    matrix.print("I");

    matrix.setTextColor(matrix.Color(255,255,255));
    matrix.setCursor(18, height);
    matrix.print("K");

    matrix.setTextColor(matrix.Color(255,255,255));
    matrix.setCursor(22, height);
    matrix.print("E");   

    matrix.setTextColor(matrix.Color(255,255,255));
    matrix.setCursor(26, height);
    matrix.print("S"); 

    drawLine(28, 6, 2, 6, {200,200,200});
    drawPixel(0, 0, {255,0,0});
    drawPixel(0, 7, {255,0,0});
    drawPixel(31, 0, {255,0,0});
    drawPixel(31, 7, {255,0,0});

    matrix.show();  

    matrix.setFont();
}

void Display::scrollText(String text, DisplayColor textColor) {
    int x = matrix.width();

    // Account for 6 pixel wide characters plus a space
    int pixelsInText = text.length() * 7;

    matrix.setTextColor(color(textColor));
    matrix.setCursor(x, 0);
    matrix.print(text);
    matrix.show();

    while(x > (matrix.width() - pixelsInText)) {
        matrix.fillScreen(0);
        matrix.setCursor(--x, 0);
        matrix.print(text);   
        matrix.show();
        delay(150);
    }
}

void Display::drawPixel(uint16_t  x, uint16_t  y, DisplayColor pixelColor) {
    matrix.drawPixel(x, y, color(pixelColor));
}

void Display::drawLine(uint16_t  x0, uint16_t  y0,uint16_t  X1,uint16_t  Y1, DisplayColor lineColor) {
    matrix.drawLine(x0, y0, X1, Y1, color(lineColor));
    
}