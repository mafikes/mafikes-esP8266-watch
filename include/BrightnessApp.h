#ifndef BrightnessApp_h
#define BrightnessApp_h

#include <WString.h>
#include <ApplicationView.h>

class BrightnessApp : public ApplicationView
{
    private:
        int activeBrightness = 1;
        uint32_t brightness[7] = {1, 15, 25, 50, 100, 125, 130};
    public:        
        void btn1_process() override;        
        void btn3_process() override;        
        void beforeRender() override;
        void render(Display& display) override;
};

#endif