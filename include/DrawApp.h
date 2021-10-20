#ifndef DrawApp_h
#define DrawApp_h

#include <WString.h>
#include <ApplicationView.h>

typedef struct { int16_t x; int16_t y; } Position;

class DrawApp : public ApplicationView
{
    private:
        int activeView = 0;
        int lastShowedFragment = 0;        
        Position position = {x: 0, y: 0};
    public:
        void mario(Display& display);
        void light(Display& display);  
        void test(Display& display);       
        void beforeRender() override;
        void render(Display& display) override;   
        void btn1_process() override;        
        void btn3_process() override;      
};

#endif