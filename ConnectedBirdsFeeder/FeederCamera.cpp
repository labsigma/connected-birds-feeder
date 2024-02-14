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

  if(psramFound()){
    _configCamera.frame_size = FRAMESIZE_UXGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    _configCamera.jpeg_quality = 10;
    _configCamera.fb_count = 2;
  } else {
    _configCamera.frame_size = FRAMESIZE_SVGA;
    _configCamera.jpeg_quality = 12;
    _configCamera.fb_count = 1;
  }
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
  doc["picture"] = "data:image/jpeg;base64," + base64Image;
  
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

void FeederCamera::sendPictureToGed(String base64Image) {
  Serial.println("Send picture to the GED");

  HTTPClient http;   
  http.setTimeout(HTTP_TIMEOUT);

  http.begin(GED_API);  
  http.setAuthorization(GED_USERNAME, GED_PASSWORD);
  http.addHeader("Content-Type", "application/json");   

  StaticJsonDocument<200> doc;

  doc["societe"] = "TEST";
  doc["metier"] = "";
  doc["typedoc"] = "feeder";
  doc["iddoc"] = String(ID_FEEDER);
  doc["nomfichier"] = "Test.jpg";
  doc["contenufichier"] = base64Image;
  
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

String FeederCamera::takePicture() {
  Serial.println("Take picture when bird detected");

  camera_fb_t *fb = esp_camera_fb_get();
  if (fb == NULL) {
    Serial.println("Photo capture error");
    return "";
  }

  String base64Image = base64::encode(fb->buf, fb->len);

  esp_camera_fb_return(fb);

  return base64Image;
}