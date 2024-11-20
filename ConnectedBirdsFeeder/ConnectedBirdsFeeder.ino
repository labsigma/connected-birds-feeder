#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#endif

#include "FeederConfiguration.h"
#include "TemperatureHumiditySensor.h"
#include "GazSensor.h"
#include "FeederCamera.h"
#include "esp_sleep.h"
#include "Logger.h"

#include <InfluxDbClient.h>

#if DEBUG_MODE
  #if defined(ESP8266)
    #include <ESPAsyncTCP.h>
  #elif defined(ESP32)
    #include <AsyncTCP.h>
  #endif
  #include <AsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #include <WebSerial.h>
  AsyncWebServer server(80);
#endif

#define uS_TO_S_FACTOR 1000000

TemperatureHumiditySensor temperatureHumiditySensor(DHT_PIN, DHT_TYPE);
GazSensor gazSensor(MQ135_PIN);

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);
Point sensor("airSensors");
bool wifiConnected;

FeederCamera feederCamera;
Logger* Logger::instance = nullptr;
Logger* logger = Logger::getInstance();

struct Statement {
  float temperature;
  float humidity;
  float co2;
  bool isTemperatureCorrect;
  bool isCo2Correct;
};

RTC_DATA_ATTR int bootCount = 0;

void setup() {
  logger->begin(115200);
  logger->println("Start of setup");

  if (DEBUG_MODE) {
    wifiConnected = initializeWifi();
    feederCamera.initializeCamera();
    temperatureHumiditySensor.initialize();
    if (wifiConnected) {
      initializeClientInfluxDb();
      // Web server
      server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "You can access the WebSerial interface at http://" + WiFi.localIP().toString() + "/webserial");
      });
      WebSerial.begin(&server);
      // Start the server
      server.begin();
      logger->activateWebSerial();
    }
  }
  else {
    ++bootCount;
    logger->println("----------------------");
    logger->println(String(bootCount)+ "eme Boot ");  

    // Get measurements if it's the first boot
    if (bootCount == 1) {
      manageMeasurements();
    }

    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

    if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT0) { // Something detected by the camera
        logger->println("Wakeup caused by external signal using RTC_IO"); 
        feederCamera.initializeCamera();
          
        if (feederCamera.isCameraInitialized()) {
          String image = feederCamera.takePicture();
          wifiConnected = initializeWifi();
          if ((image != "") && wifiConnected){
            if (SEND_TO_GED) {
              feederCamera.sendPictureToGed(image);
            }
            else {
              feederCamera.sendPicture(image); 
            }
          }
        }
    }
    else if (wakeup_reason == ESP_SLEEP_WAKEUP_TIMER) { // Retreive and send measurements periodically
        logger->println("Wakeup caused by timer");
        manageMeasurements();
    }

    esp_sleep_enable_ext0_wakeup(PIR_PIN, HIGH);
    esp_sleep_enable_timer_wakeup(MEASUREMENT_INTERVAL * uS_TO_S_FACTOR);
    logger->println("Setup ESP32 to sleep for every " + String(MEASUREMENT_INTERVAL) + " Seconds");

    logger->println("Going to sleep now");
    logger->println("----------------------"); 
    esp_deep_sleep_start();

  }
  
}

void manageMeasurements() {
  temperatureHumiditySensor.initialize();
  
  Statement statement = retreiveMeasurements();
  if (statement.isTemperatureCorrect) {
    bool wifiConnected = initializeWifi();
    if (wifiConnected) {
      initializeClientInfluxDb();
      sendStatement(statement);
    }
  }
}

void loop() {
  // Only loop on debug mode

/*  int pirState = digitalRead(PIR_PIN);  // State of PIR Module
  if (pirState == HIGH) {
    if (feederCamera.isCameraInitialized()) {
      String image = feederCamera.takePicture();
      if ((image != "") && wifiConnected){
        if (SEND_TO_GED) {
          feederCamera.sendPictureToGed(image);
        }
        else {
          feederCamera.sendPicture(image); 
        }
      }
    }
  }
  */

  static unsigned long last_print_time = millis();

  if ((unsigned long)(millis() - last_print_time) > MEASUREMENT_INTERVAL * 1000) {
    ++bootCount;
    logger->println("----------------------");
    logger->println(String(bootCount)+ "eme loop ");  

    logger->printf("Uptime: %lums\n", millis());
    last_print_time = millis();

    Statement statement = retreiveMeasurements();
    if (statement.isTemperatureCorrect) {
      if (wifiConnected) {
        sendStatement(statement);
      }
    }
  }

  WebSerial.loop();

}

bool initializeWifi() {
  logger->println("WiFi initialization");
  bool wifiConnected = false; 


  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      logger->printf("WiFi Failed!\n");
  }
  else {
    wifiConnected = true;
    logger->print("IP Address: ");
    logger->println(WiFi.localIP().toString());
  }
  
  return wifiConnected;
}

void initializeClientInfluxDb() {

  logger->println("Client influxDb initialization");
  try {
    sensor.addTag("sensor_id", "IDFEEDER_" + String(ID_FEEDER));

    // Check server connection
    if (client.validateConnection()) {
      logger->print("Connected to InfluxDB: ");
      logger->println(client.getServerUrl());
    } else {
      logger->print("InfluxDB connection failed: ");
      logger->println(client.getLastErrorMessage());
    }
  }
  catch(std::exception& e) {
    logger->println(e.what());
  }
}

Statement retreiveMeasurements() {
  logger->println("Retrieval of measurements");
  Statement statement;

  statement.isTemperatureCorrect = true;
  statement.isCo2Correct = true;

  logger->println("Temperature and humidity recovery");
  if (temperatureHumiditySensor.isInitialized()) {
    statement.humidity = temperatureHumiditySensor.retreiveHumidity();
    statement.temperature = temperatureHumiditySensor.retreiveTemperature();  
  }

  // We test that the datas retrieved are corrects
  if (isnan(statement.humidity) || isnan(statement.temperature) ) {
    logger->println("Unable to recover from temperature and humidity sensor !");
    statement.isTemperatureCorrect = false;
  }
  else {
    logger->print("Humidity: ");
    logger->print(statement.humidity);
    logger->println("%");
    logger->print("Temperature: ");
    logger->print(statement.temperature);
    logger->println("°C ");
  }

  
  if (statement.isTemperatureCorrect) {
    logger->println("CO2 concentration recovery");
    statement.co2 = gazSensor.retreiveCO2Concentration(statement.temperature, statement.humidity);

    if (isnan(statement.co2)) {
      logger->println("Unable to recover from co2 sensor !");
      statement.isCo2Correct = false;
      statement.co2 = 0;
    }
    else {
      logger->print("CO2 Concentration : ");
      logger->print(statement.co2);
      logger->println(" ppm");
    }
  }
  
  return statement;
}

void sendStatement(Statement statement) {
  logger->println("Sending of the statement");

  // Store measured value into point
  sensor.clearFields();
  sensor.addField("temperature", statement.temperature);
  sensor.addField("humidity", statement.humidity);
  sensor.addField("co", statement.co2);

  // Print what are we exactly writing
  logger->print("Writing: ");
  logger->println(client.pointToLineProtocol(sensor));
  // Write point
  if (!client.writePoint(sensor)) {
    logger->print("InfluxDB write failed: ");
    logger->println(client.getLastErrorMessage());
  }
}



