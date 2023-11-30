#ifndef FeederCamera_h
#define FeederCamera_h

#include "esp_camera.h"
#include <Arduino.h>

class FeederCamera {
  private:
    bool _isCameraInitialized;
    camera_config_t _configCamera;
  public:
    FeederCamera();
    void initializeCamera(); 
    bool isCameraInitialized();
    void sendPicture(String base64Image);
    String takePicture();
};

#endif