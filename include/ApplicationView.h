#ifndef ApplicationView_h
#define ApplicationView_h

#include "Display.h"

class ApplicationView {
    private:

    public:
        virtual ~ApplicationView() {}
        virtual void beforeRender(){}     
        virtual void render(Display&) = 0;     
        virtual void clear() {}     
};

#endif