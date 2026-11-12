#ifndef TimeApp_h
#define TimeApp_h

#include <WString.h>
#include <ApplicationView.h>
#include <Colors.h>
#include <RTClib.h>

class TimeApp : public ApplicationView
{
    private:
        int clockTheme = 0;
        int clockColorActive = 0;
        DisplayColor clockColor = COLOR_WHITE;
        DisplayColor getColor();

        // Rolling digits animation (HH:MM:SS), old digit slides up, new comes from bottom
        bool digitsInitialized = false;
        char currentDigits[6];
        char previousDigits[6];
        unsigned long digitChangeTime[6];
        void drawTime(Display& display, const DateTime& now, int16_t x, bool showSeconds);
    public:        
        void btn1_process() override;        
        void btn3_process() override;        
        void beforeRender() override;
        void render(Display& display) override;
};

#endif