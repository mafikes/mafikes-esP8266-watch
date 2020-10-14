#ifndef ApplicationManager_h
#define ApplicationManager_h

#include <ApplicationView.h>

class ApplicationManager {
    private:
        // ApplicationManager() {}
        // ApplicationManager(ApplicationManager const&);
        // void operator = (ApplicationManager const&);

        int activeAppView = 0;    
        int nextAppView = 0;
        bool runBeforeRender = true;     
        
        ApplicationView** applications;
        ApplicationView* getActiveApp();   
    public:
        // static ApplicationManager& getInstance() {
        //     static ApplicationManager instance;
        //     return instance;
        // }    
        void setup();
        void nextApp();
        void loop();
};

#endif