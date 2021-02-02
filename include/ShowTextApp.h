#ifndef ShowTextApp_h
#define ShowTextApp_h

#include <WString.h>
#include <ApplicationView.h>

class ShowTextApp : public ApplicationView
{
    private:
        String showText;
        bool defaultFont;
        int16_t position;
        int showTextLenght;
        unsigned long animationSpeed;
        DisplayColor textColor;
    public:
        ShowTextApp(String, bool, int, DisplayColor color);       
        void beforeRender() override;
        void render(Display& display) override;   
};

#endif