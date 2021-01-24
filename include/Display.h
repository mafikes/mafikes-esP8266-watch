#ifndef Display_h
#define Display_h

#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoMatrix.h>

typedef struct { char red; char green; char blue; } DisplayColor;
typedef struct { int16_t x; int16_t y; } DisplayPosition;

class Display {
    private:
        Display();
        Display(Display const&);
        void operator = (Display const&);

        Adafruit_NeoMatrix matrix;
        uint32_t color(DisplayColor);

        int lastShowedIcon = 0;
        int iconAnimationRepeated = 0;

        void setup();
    public:
        static Display& getInstance() {
            static Display instance;
            return instance;
        }

        void clear();
        void refresh();
        void setBrightness(int);
        void drawText(String, bool, DisplayPosition, DisplayColor = {255, 255, 255});
        void drawBitmap(unsigned char[], DisplayPosition, DisplayColor, int16_t, int16_t);
        void fixdrawRGBBitmap(int16_t, int16_t, const uint32_t*, int16_t, int16_t);
        void showTextWithIconAnimated(const uint32_t[][64], int, String, DisplayPosition, DisplayColor);
        void showIcon(const uint32_t[][64], int, int);
        void drawTextWithIcon(String, DisplayPosition, DisplayColor = {255, 255, 255});
        void showLogo();
        void show();
        void reset();
        void printText(String, DisplayPosition, DisplayColor);
        void getColor(DisplayColor);
        void drawPixel(uint16_t, uint16_t, DisplayColor);
        void drawLine(uint16_t ,uint16_t ,uint16_t ,uint16_t , DisplayColor);
};

#endif