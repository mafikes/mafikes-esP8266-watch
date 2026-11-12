#ifndef WeatherApp_h
#define WeatherApp_h

#include <WString.h>
#include <ApplicationView.h>

class WeatherApp : public ApplicationView
{
    private:
        float temperature = 0;      
        bool downloaded = false;    
        bool requested = false;
        unsigned long lastRequestTime = 0;
        String temperatureIcon;     
        String lastTemperatureIcon;      
    public:
        void showIcon(Display& display);
        void askServer();
        void getIcon();
        void prepareIcon();
        void beforeRender() override;
        void render(Display& display) override;
};

#endif