#ifndef Helper_h
#define Helper_h

#include <WString.h>

class Helper {
    private:
        Helper() {}
        Helper(Helper const&);
        void operator = (Helper const&);
    public:
        static Helper& getInstance() {
            static Helper instance;
            return instance;
        }    

        String getStringRounded(float, int, int);
};

#endif