#ifndef BrightnessApp_h
#define BrightnessApp_h

#include <WString.h>
#include <ApplicationView.h>

class BrightnessApp : public ApplicationView
{
    private:
        float percentage;
        uint32_t brightness[9] = {1, 25, 50, 100, 125, 175, 200, 225, 255};
    public:        
        void btn1_process() override;        
        void btn3_process() override;        
        void beforeRender() override;
        void render(Display& display) override;
};

#endif