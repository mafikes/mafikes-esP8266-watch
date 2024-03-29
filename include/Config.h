#ifndef config_h
#define config_h

#include "ArduinoJson.h"
#include <WString.h>
#include <Display.h>

struct ConfigFile {
    int brightness;
    bool brightness_auto;
    int watch_type;
    int watch_type_color;

    bool view_auto_switch;
    int view_main_switch_time;
    int view_app_switch_time;
    int time_update_interval;
    
    String ip_address;
    String weather_key;
    String weather_location;

    int time_offset;
    bool watch_color_custom;
    int watch_color[3] = {255, 255, 255};
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
