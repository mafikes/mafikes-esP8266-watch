#ifndef ApplicationManager_h
#define ApplicationManager_h

#include "Display.h"
#include <ApplicationView.h>

class ApplicationManager {
    private:
        ApplicationManager() {}
        ApplicationManager(ApplicationManager const&);
        void operator = (ApplicationManager const&);

        int activeAppView = 0;     
        int storedAppView = 0;
        bool allowedSwitchApp;
        bool canSwitchApp;
        bool autoSwitchRun;

        int btn1LastState = LOW;
        int btn2LastState = LOW;
        int btn3LastState = LOW;

        void autoTimerAppSwitch();
        void autoSwitchMainTimer();
        void resumeAutoSwitch();

        int intervalMainSwitchApp = 300000; // 5min 
        int intervalSwitchApp = 10000; // 10s

        String showTextValue; 
        ApplicationView* application;
    public:
        static ApplicationManager& getInstance() {
            static ApplicationManager instance;
            return instance;
        }
        
        void showText(String text);  
        void setBrightness(int value);
        void btn1_process();
        void btn2_process();
        void btn3_process();
        void setup();
        void nextApp(bool = false);
        void loop();
};

#endif