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
        void setApplication(ApplicationView* newApplication, DisplayTransition transition = TRANSITION_NONE);
        void processPendingActions();

        // Requests from web server callbacks, processed in loop()
        int pendingButton = 0;
        bool pendingShowText = false;
        bool pendingShowMainApp = false;
        bool pendingLoadSwitchTime = false;
        String pendingText;

        unsigned long prevMainSwitchMillis = 0;
        unsigned long prevAppSwitchMillis = 0;
        unsigned long prevRenderMillis = 0;
        unsigned long prevButtonMillis = 0;
        unsigned long intervalMainSwitchApp = 300000; // 5min 
        unsigned long intervalSwitchApp = 10000; // 10s

        String showTextValue; 
        ApplicationView* application = nullptr;
    public:
        static ApplicationManager& getInstance() {
            static ApplicationManager instance;
            return instance;
        }
        
        void loadSwichTime();
        void changeSwitchViewAuto(bool);
        void showMainApp();
        void showText(String text);  
        void setBrightness(int value);
        void btn1_process();
        void btn2_process();
        void btn3_process();
        void setup();
        void nextApp(bool fromButton);
        void loop();

        // Safe to call from async web server callbacks
        void requestButton(int button);
        void requestShowText(String text);
        void requestShowMainApp();
        void requestLoadSwitchTime();
};

#endif