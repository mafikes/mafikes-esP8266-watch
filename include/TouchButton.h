#ifndef TouchButton_h
#define TouchButton_h

#include <WString.h>

class TouchButton 
{
    private:
        TouchButton() {}
        TouchButton(TouchButton const&);
        void operator = (TouchButton const&);
    public:
        static TouchButton& getInstance() {
            static TouchButton instance;
            return instance;
        }  
        
        bool btn1_status = false;
        bool btn2_status = false;
        bool btn3_status = false;
        void setup();
        void loop();
};

#endif