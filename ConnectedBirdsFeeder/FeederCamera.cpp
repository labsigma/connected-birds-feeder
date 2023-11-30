#include <string>
#include "FeederCamera.h"

#include "FeederConfiguration.h"

#include <iostream>
#include <exception>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "base64.h"

#include "esp_camera.h"

FeederCamera::FeederCamera() {
  _isCameraInitialized = false;

  _configCamera.ledc_channel = LEDC_CHANNEL_0;
  _configCamera.ledc_timer = LEDC_TIMER_0;
  _configCamera.pin_d0 = Y2_GPIO_NUM;
  _configCamera.pin_d1 = Y3_GPIO_NUM;
  _configCamera.pin_d2 = Y4_GPIO_NUM;
  _configCamera.pin_d3 = Y5_GPIO_NUM;
  _configCamera.pin_d4 = Y6_GPIO_NUM;
  _configCamera.pin_d5 = Y7_GPIO_NUM;
  _configCamera.pin_d6 = Y8_GPIO_NUM;
  _configCamera.pin_d7 = Y9_GPIO_NUM;
  _configCamera.pin_xclk = XCLK_GPIO_NUM;
  _configCamera.pin_pclk = PCLK_GPIO_NUM;
  _configCamera.pin_vsync = VSYNC_GPIO_NUM;
  _configCamera.pin_href = HREF_GPIO_NUM;
  _configCamera.pin_sscb_sda = SIOD_GPIO_NUM;
  _configCamera.pin_sscb_scl = SIOC_GPIO_NUM;
  _configCamera.pin_pwdn = PWDN_GPIO_NUM;
  _configCamera.pin_reset = RESET_GPIO_NUM;
  _configCamera.xclk_freq_hz = 20000000;
  _configCamera.pixel_format = PIXFORMAT_JPEG;
  _configCamera.frame_size = FRAMESIZE_UXGA; 
  _configCamera.jpeg_quality = JPEG_QUALITY;
  _configCamera.fb_count = FB_COUNT;
}

void FeederCamera::sendPicture(String base64Image) {
  Serial.println("Send picture to the ia recognition api");

  HTTPClient http;   
  http.setTimeout(HTTP_TIMEOUT);

  http.begin(IA_RECOGNITION_API);  
  http.setAuthorization(IA_RECOGNITION_USERNAME, IA_RECOGNITION_PASSWORD);
  http.addHeader("Content-Type", "application/json");   

  StaticJsonDocument<200> doc;

  doc["idFeeder"] = ID_FEEDER;
  doc["picture"] = base64Image;
  
  String requestBody;
  serializeJson(doc, requestBody);
  
  Serial.println(requestBody);
     
  int httpResponseCode = http.POST(requestBody);
 
  if(httpResponseCode > 0){
    String response = http.getString();                       
    Serial.println(response);
    Serial.println(httpResponseCode);   
  }
  else {
    Serial.print("Error sending statement : ");
    Serial.println(http.errorToString(httpResponseCode).c_str());
  }

  http.end();
}

void FeederCamera::initializeCamera() {
  Serial.println("Feeder camera initialization");
  esp_err_t err = esp_camera_init(&_configCamera);
  if (err != ESP_OK) {
    Serial.println("Error while initialize camera : " +  err);
    return;
  }
  _isCameraInitialized = true;
} 

bool FeederCamera::isCameraInitialized() {
  return _isCameraInitialized;
}

String urlencode(String str) {
  const char *msg = str.c_str();
  const char *hex = "0123456789ABCDEF";
  String encodedMsg = "";
  while (*msg != '\0') {
    if (('a' <= *msg && *msg <= 'z') || ('A' <= *msg && *msg <= 'Z') || ('0' <= *msg && *msg <= '9') || *msg == '-' || *msg == '_' || *msg == '.' || *msg == '~') {
      encodedMsg += *msg;
    } else {
      encodedMsg += '%';
      encodedMsg += hex[(unsigned char)*msg >> 4];
      encodedMsg += hex[*msg & 0xf];
    }
    msg++;
  }
  return encodedMsg;
}

String FeederCamera::takePicture() {
  Serial.println("Take picture when bird detected");

  camera_fb_t *fb = esp_camera_fb_get();
  if (fb == NULL) {
    Serial.println("Photo capture error");
    return "";
  }

  String base64image = base64::encode(fb->buf, fb->len);

  esp_camera_fb_return(fb);
    
  return base64image;
  
}