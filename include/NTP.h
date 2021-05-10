#ifndef NTP_h
#define NTP_h

class NTP 
{
    private:
        unsigned long prevTime = 0;
        void updateTime();
    public:        
        void setup();
        void loop();
};

#endif