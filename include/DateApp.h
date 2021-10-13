#ifndef DateApp_h
#define DateApp_h

#include <WString.h>
#include <ApplicationView.h>

class DateApp : public ApplicationView
{     
    public:        
        void render(Display& display) override;
};

#endif