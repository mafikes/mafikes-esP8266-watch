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
        int nextAppView = 0;
        bool runBeforeRender = true;     
        
        int btn1LastState = LOW;
        int btn2LastState = LOW;
        int btn3LastState = LOW;

        String showTextValue; 
        ApplicationView* application;
        // ApplicationView applications[2];
        // ApplicationView* getActiveApp();   
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