#ifndef ServiceNTP_h
#define ServiceNTP_h

class ServiceNTP {
    private:
        ServiceNTP() {}
        ServiceNTP(ServiceNTP const&);
        void operator = (ServiceNTP const&);

        unsigned long prevTime = 0;
        bool firstStart = true;        
        bool pendingUpdate = false;
    public:
        static ServiceNTP& getInstance()
        {
            static ServiceNTP instance;
            return instance;
        }
        
        void setup();
        void updateTime();
        void requestUpdate();
        void loop();
};

#endif
