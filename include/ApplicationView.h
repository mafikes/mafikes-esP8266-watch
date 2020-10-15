#ifndef ApplicationView_h
#define ApplicationView_h

#include "Display.h"

class ApplicationView {
    private:

    public:
        virtual ~ApplicationView() {}
        virtual void btn1_process() {}
        virtual void btn3_process() {}
        virtual void beforeRender(){}     
        virtual void render(Display&) = 0;     
        virtual void clear() {}     
};

#endif