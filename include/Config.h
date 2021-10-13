#ifndef config_h
#define config_h

#include "ArduinoJson.h"
#include <WString.h>

struct ConfigFile {
    int brightness;
    bool brightness_auto;
    int watch_type;
    int watch_type_color;

    bool view_auto_switch;
    int view_main_switch_time;
    int view_app_switch_time;

    String ip_address;
    String weather_key;
    String weather_location;
};

class Config
{
    private:
        Config() {}
        Config(Config const&);
        void operator = (Config const&);

        const String configPath = "/config.json";        
    public:      
        static Config& getInstance() {
            static Config instance;
            return instance;
        }
        
        ConfigFile data;
            
        void setup();  
        void load();
        void save();
};

#endif