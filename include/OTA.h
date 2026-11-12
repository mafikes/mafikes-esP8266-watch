#ifndef OTA_h
#define OTA_h

#include <ESPAsyncWebServer.h>

class OTA
{
    private:
        OTA() {}
        OTA(OTA const&);
        void operator = (OTA const&);

        // Set from web server callbacks, processed in loop()
        bool running = false;
        bool finished = false;
        bool filesystem = false;
        bool remountFilesystem = false;
        AsyncWebServerRequest* uploadRequest = nullptr;
        size_t uploadedSize = 0;
        size_t totalSize = 0;
        unsigned long lastDataTime = 0;
        unsigned long finishedTime = 0;

        void handleUpload(AsyncWebServerRequest*, size_t, uint8_t*, size_t, bool);
        void handleUploadDone(AsyncWebServerRequest*);
        void uploadFailed();
    public:
        static OTA& getInstance() {
            static OTA instance;
            return instance;
        }

        void setup(AsyncWebServer& server);
        void loop();
        bool isRunning();
        int progress();
};

#endif
