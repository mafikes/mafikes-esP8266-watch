#ifndef WeatherApp_h
#define WeatherApp_h

#include <WString.h>
#include <ApplicationView.h>

class WeatherApp : public ApplicationView
{
    private:
        float temperature = 0;        
        String temperature_icon;    
    public:
        void askServer();
        void getIcon();
        void beforeRender() override;
        void render(Display& display) override;
};

#endif