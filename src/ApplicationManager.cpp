#include <ApplicationManager.h>

#include <Config.h>
#include <Display.h>
#include <WString.h>

// Applications 
#include <WeatherApp.h>
#include <TimeApp.h>

ApplicationView* ApplicationManager::getActiveApp() 
{
    return applications[activeAppView];
}

void ApplicationManager::setup() 
{
    applications[0] = new TimeApp();
    applications[1] = new WeatherApp();
}

void ApplicationManager::nextApp() 
{
    nextAppView++;
}   

void ApplicationManager::loop()
{
    // if(nextAppView != activeAppView) {
        // application->clear();  
        // activeAppView = nextAppView;
        // runBeforeRender = true;
    // }

    // application = getActiveApp();
    
    // if(application) {        
    //     if(runBeforeRender) {
    //         application->beforeRender();
    //         runBeforeRender = false;
    //     }

    //     application->render(Display::getInstance());
    // } else {
    //     Display::getInstance().drawText("ERROR", true, {0,0}, {255, 0, 0});
    // }
} 
