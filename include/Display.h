#ifndef Display_h
#define Display_h

#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoMatrix.h>

typedef struct { char red; char green; char blue; } DisplayColor;
typedef struct { char x; char y; } DisplayPosition;

class Display {
    private:
        Display();
        Display(Display const&);
        void operator = (Display const&);
        Adafruit_NeoMatrix matrix;
        uint32_t color(DisplayColor);

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
        void showLogo();
        void scrollText(String, DisplayColor);
        void getColor(DisplayColor);
        void drawPixel(uint16_t, uint16_t, DisplayColor);
        void drawLine(uint16_t ,uint16_t ,uint16_t ,uint16_t , DisplayColor);
};

#endif