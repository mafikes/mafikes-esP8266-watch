#ifndef TimeApp_h
#define TimeApp_h

#include <WString.h>
#include <ApplicationView.h>

class TimeApp : public ApplicationView
{
    private:
        int clockTheme = 1;
        int clockColorActive = 0;
        // DisplayColor clockColor = COLOR_WHITE;
        String repairDigit(int);
        void displayClock(Display& display);
    public:        
        void changeColor();
        void nextTheme();        
        void beforeRender() override;
        void render(Display& display) override;
};

#endif