#ifndef TimeApp_h
#define TimeApp_h

#include <WString.h>
#include <ApplicationView.h>
#include <Colors.h>

class TimeApp : public ApplicationView
{
    private:
        int clockTheme = 0;
        int clockColorActive = 0;
        DisplayColor clockColor = COLOR_WHITE;
        String repairDigit(int);
        void displayClock(Display& display);
    public:        
        void btn1_process() override;        
        void btn3_process() override;        
        void beforeRender() override;
        void render(Display& display) override;
};

#endif