#include <Display.h>
#include <AwtrixFont.h>
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
#define NUMPIXELS MATRIX_WIDTH*MATRIX_HEIGHT

// #define MATRIX_MODE NEO_MATRIX_TOP + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE
#define MATRIX_MODE NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG
// #define MATRIX_MODE NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG
#define MATRIX_TYPE NEO_GRB + NEO_KHZ800

#define TRANSITION_TIME 350 // ms

#define arrayLength(array) (sizeof((array))/sizeof((array)[0]))

Display::Display() : canvas(MATRIX_WIDTH, MATRIX_HEIGHT), matrix(MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_PIN, MATRIX_MODE, MATRIX_TYPE) {    
    setup();
}

void Display::setup() {
    matrix.begin();  
    canvas.setTextWrap(false);

    setBrightness(30); 
}

void Display::clear() {
    canvas.fillScreen(0); 
}

void Display::setBrightness(int value) 
{
    brightness = value; 
    matrix.setBrightness(brightness);
    forceShow = true;
}

uint32_t Display::color(DisplayColor color)
{
    return matrix.Color(color.red, color.green, color.blue);
}

void Display::drawText(String text, bool defaultFont, DisplayPosition pos, DisplayColor colorText) {
    if (defaultFont) {
        canvas.setFont();
        canvas.setCursor(pos.x, pos.y);
    }else{
        canvas.setFont(&AwtrixFont);
        canvas.setCursor(pos.x, pos.y+6);
    }

    canvas.setTextWrap(false);
    canvas.setTextColor(color(colorText)); 
    canvas.print(text);     
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
    
    canvas.drawRGBBitmap(x, y, RGB_bmp_fixed, w, h);  
}

void Display::resetIconAnimation() 
{
    lastShowedIcon = 0;    
    repeatedAnimation = 0;
    lastIconAnimationCount = 0;
    iconAnimationStarted = false;
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

    lastIconAnimationCount = animationCount;

    if(animationCount > 1 && repeatedAnimation < animateSteps) {
        unsigned long currentMillis = millis();

        if(!iconAnimationStarted) {
            iconAnimationStarted = true;
            lastIconFrameTime = currentMillis;
            iconFrameDelay = delayTime;
        }

        if(currentMillis - lastIconFrameTime >= iconFrameDelay) {
            lastIconFrameTime = currentMillis;

            lastShowedIcon++;
            if (lastShowedIcon >= animationCount) { // animation is showed full, start it from zero
                lastShowedIcon = 0;
                repeatedAnimation++;
            }

            // Last frame of animation stays longer
            iconFrameDelay = lastShowedIcon == animationCount - 1 ? delayTime + 400 : delayTime;
        }

        if(repeatedAnimation < animateSteps) {
            fixdrawRGBBitmap(0, 0, bitmap[lastShowedIcon], 8, 8);
            return;
        }
    }

    fixdrawRGBBitmap(0, 0, bitmap[0], 8, 8);
}

void Display::resetLoading()
{
    loadingRepeated = 0;
}

void Display::showLoading(int delayTime)
{
    unsigned long currentMillis = millis();

    if(loadingRepeated == 0 || currentMillis - lastLoadingTime >= (unsigned long)delayTime) {
        lastLoadingTime = currentMillis;

        loadingRepeated++;
        if(loadingRepeated > 4) { 
            loadingRepeated = 1;
        }
    }

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
}

void Display::drawTextWithIcon(String text, DisplayPosition pos, DisplayColor colorText) 
{
    canvas.setFont(&AwtrixFont);
    canvas.setCursor(pos.x+8, pos.y+6);
    canvas.setTextColor(color(colorText));
    
    canvas.print(text);         
}

void Display::startTransition(DisplayTransition newTransition)
{
    // Last frame of the previous application slides out
    memcpy(transitionFrame, canvas.getBuffer(), sizeof(transitionFrame));
    transition = newTransition;
    transitionStart = millis();
}

int Display::transitionOffset()
{
    if(transition == TRANSITION_NONE) {
        return MATRIX_HEIGHT;
    }

    unsigned long elapsed = millis() - transitionStart;

    if(elapsed >= TRANSITION_TIME) {
        transition = TRANSITION_NONE;
        return MATRIX_HEIGHT;
    }

    // Ease out, fast start and slow end
    float progress = 1.0f - (float)elapsed / TRANSITION_TIME;
    progress = 1.0f - progress * progress * progress;

    return (int)(progress * MATRIX_HEIGHT);
}

void Display::show() 
{
    uint16_t frame[NUMPIXELS];
    uint16_t *current = canvas.getBuffer();
    int offset = transitionOffset();

    for(int y = 0; y < MATRIX_HEIGHT; y++) {
        for(int x = 0; x < MATRIX_WIDTH; x++) {
            uint16_t pixel;

            if(offset >= MATRIX_HEIGHT) {
                pixel = current[y * MATRIX_WIDTH + x];
            } else if(transition == TRANSITION_SLIDE_UP) {
                // Old frame moves up, new frame comes from bottom
                int source = y + offset;
                pixel = source < MATRIX_HEIGHT
                    ? transitionFrame[source * MATRIX_WIDTH + x]
                    : current[(source - MATRIX_HEIGHT) * MATRIX_WIDTH + x];
            } else {
                // Old frame moves down, new frame comes from top
                int source = y - offset;
                pixel = source >= 0
                    ? transitionFrame[source * MATRIX_WIDTH + x]
                    : current[(source + MATRIX_HEIGHT) * MATRIX_WIDTH + x];
            }

            frame[y * MATRIX_WIDTH + x] = pixel;
        }
    }

    // Sending data to LEDs blocks interrupts, skip it when nothing changed
    if(!forceShow && memcmp(frame, shownFrame, sizeof(shownFrame)) == 0) {
        return;
    }

    memcpy(shownFrame, frame, sizeof(shownFrame));
    forceShow = false;

    for(int y = 0; y < MATRIX_HEIGHT; y++) {
        for(int x = 0; x < MATRIX_WIDTH; x++) {
            matrix.drawPixel(x, y, frame[y * MATRIX_WIDTH + x]);
        }
    }

    matrix.show();
}

void Display::showLogo() {
    canvas.fillScreen(0);
    matrix.setBrightness(40);
    forceShow = true;

    canvas.setFont(&AwtrixFont);
    canvas.setTextColor(color(COLOR_WHITE));
    canvas.setCursor(2, 6);
    canvas.print("M");

    canvas.setTextColor(color(COLOR_RED));
    canvas.setCursor(8, 6);
    canvas.print("A");

    canvas.setTextColor(color(COLOR_WHITE));
    canvas.setCursor(12, 6);
    canvas.print("FIKES");

    drawLine(28, 6, 2, 6, {200,200,200});

    drawPixel(0, 0, COLOR_RED);
    drawPixel(0, 7, COLOR_RED);
    drawPixel(31, 0, COLOR_RED);
    drawPixel(31, 7, COLOR_RED);

    show();
}


void Display::drawPixel(uint16_t  x, uint16_t  y, DisplayColor pixelColor) {
    canvas.drawPixel(x, y, color(pixelColor));
}

void Display::fillScreen(DisplayColor lineColor) {
    canvas.fillScreen(color(lineColor));
}

void Display::drawLine(uint16_t  x0, uint16_t  y0, uint16_t  X1,uint16_t  Y1, DisplayColor lineColor) {
    canvas.drawLine(x0, y0, X1, Y1, color(lineColor));
    
}
