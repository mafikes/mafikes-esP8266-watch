#ifndef Display_h
#define Display_h

#include <Adafruit_NeoMatrix.h>

typedef struct { int red; int green; int blue; } DisplayColor;
typedef struct { int16_t x; int16_t y; } DisplayPosition;

enum DisplayTransition { TRANSITION_NONE, TRANSITION_SLIDE_UP, TRANSITION_SLIDE_DOWN };

class Display {
    private:
        Display();
        Display(Display const&);
        void operator = (Display const&);

        int lastShowedIcon = 0;
        int lastIconAnimationCount = 0;
        bool iconAnimationStarted = false;
        unsigned long lastIconFrameTime = 0;
        unsigned long iconFrameDelay = 0;

        int loadingRepeated = 0;
        unsigned long lastLoadingTime = 0;

        int repeatedAnimation = 0;
        int brightness = 30;

        // Everything is drawn into canvas, show() composes it to the matrix
        GFXcanvas16 canvas;
        uint16_t shownFrame[32 * 8];
        bool forceShow = true;

        // Slide transition between applications
        uint16_t transitionFrame[32 * 8];
        DisplayTransition transition = TRANSITION_NONE;
        unsigned long transitionStart = 0;

        void setup();
        int transitionOffset();
    public:
        static Display& getInstance() {
            static Display instance;
            return instance;
        }

        Adafruit_NeoMatrix matrix;
        uint32_t color(DisplayColor);

        void clear();
        void setBrightness(int);
        void drawText(String, bool, DisplayPosition, DisplayColor = {255, 255, 255});
        void drawChar(char, DisplayPosition, DisplayColor);
        void fixdrawRGBBitmap(int16_t, int16_t, const uint32_t*, int16_t, int16_t);
        void resetLoading();
        void showLoading(int = 300);
        void showAnimateIcon(const uint32_t[][64], int, int = 4, int = 250);
        void showIcon(const uint32_t[][64]);
        void drawTextWithIcon(String, DisplayPosition, DisplayColor = {255, 255, 255});
        void showLogo();
        void showUpdateProgress(int);
        void show();
        void resetIconAnimation();
        void drawPixel(uint16_t, uint16_t, DisplayColor);
        void fillScreen(DisplayColor);
        void drawLine(uint16_t ,uint16_t ,uint16_t ,uint16_t , DisplayColor);
        void startTransition(DisplayTransition);
};

#endif
