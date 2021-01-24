#ifndef TemperatureApp_h
#define TemperatureApp_h

#include <WString.h>
#include <ApplicationView.h>

class TemperatureApp : public ApplicationView
{
    private:
        float temperature = 0;
        int humidity = 0;    
        float heatIndex = 0;
        int view = 0;
    public:
        TemperatureApp(int);
     
        void beforeRender() override;
        void render(Display& display) override;
        void showTemperature(Display& display);
        void showHumidity(Display& display);
        void showHeatIndex(Display& display);        
        void readData();
};

#endif