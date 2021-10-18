#ifndef Anniversary_h
#define Anniversary_h

#include <WString.h>
#include <ApplicationView.h>

class Anniversary : public ApplicationView
{
    private:
        int days = 0;
    public:           
        void beforeRender() override;
        void render(Display& display) override;
};

#endif