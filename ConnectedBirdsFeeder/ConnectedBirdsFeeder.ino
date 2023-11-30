#include <WiFi.h>
#include <WiFiMulti.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#include "FeederConfiguration.h"
#include "TemperatureHumiditySensor.h"
#include "GazSensor.h"
#include "WifiConfiguration.h"
#include "FeederCamera.h"

#include <InfluxDbClient.h>

#define uS_TO_S_FACTOR 1000000

TemperatureHumiditySensor temperatureHumiditySensor;
GazSensor gazSensor;

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);
Point sensor("airSensors");

WiFiMulti wifiMulti;
FeederCamera feederCamera;

struct Statement {
  float temperature;
  float humidity;
  float co2;
};

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  Serial.begin(115200);
  Serial.println("Start of setup");

  bool wifiConnecte = initializeWifi();
  temperatureHumiditySensor.initialize();

  initializeClientInfluxDb();

  feederCamera.initializeCamera();

  Serial.println("End of setup");
  
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT0) { // Something detected by the camera
      Serial.println("Wakeup caused by external signal using RTC_IO"); 
      if (feederCamera.isCameraInitialized()) {
        String image = feederCamera.takePicture();
        if ((image != "") && wifiConnecte){
          feederCamera.sendPicture(image); 
        }
      }
  }
  else if (wakeup_reason == ESP_SLEEP_WAKEUP_TIMER) { // Retreive and send measurements periodically
      Serial.println("Wakeup caused by timer");
      Statement statement = retreiveMeasurements();
      if (wifiConnecte) {
        sendStatement(statement);
      }
  }

  esp_sleep_enable_ext0_wakeup(PIR_PIN, 0);
  esp_sleep_enable_timer_wakeup(MEASUREMENT_INTERVAL * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(MEASUREMENT_INTERVAL) +
  " Seconds");

  Serial.println("Going to sleep now");
  Serial.flush(); 
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
  
}

void loop() {
  // Nothing is execute in the loop function because of the deep sleep mode
  Serial.println("Normally never printed !!!");
}

bool initializeWifi() {
  Serial.println("WiFi initialization");
  bool wifiConnecte = false; 

  for (int i; i < NB_WIFI_ENDPOINTS; i++) {
    wifiMulti.addAP(wifi_endpoints[i].ssid, wifi_endpoints[i].cle);
  }

  Serial.println("WiFi connection...");
  if(wifiMulti.run() == WL_CONNECTED) {
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      Serial.println("");
      wifiConnecte = true;
  }

  
  return wifiConnecte;
}

void initializeClientInfluxDb() {

  sensor.addTag("sensor_id", "IDFEEDER_" + String(ID_FEEDER));

  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}

Statement retreiveMeasurements() {
  Serial.println("Retrieval of measurements");
  Statement statement;

  Serial.println("Temperature and humidity recovery");
  if (temperatureHumiditySensor.isInitialized()) {
    statement.humidity = temperatureHumiditySensor.retreiveHumidity();
    statement.temperature = temperatureHumiditySensor.retreiveTemperature();  
  }

  // We test that the datas retrieved are corrects
  if (isnan(statement.humidity) || isnan(statement.temperature) ) {
    Serial.println("Unable to recover from temperature and humidity sensor !");
  }
  else {
    Serial.print("Humidity: ");
    Serial.println(statement.humidity);
    Serial.print("Temperature: ");
    Serial.print(statement.temperature);
    Serial.println("°C ");
  }

  Serial.println("CO2 concentration recovery");
  statement.co2 = gazSensor.retreiveCO2Concentration(statement.temperature, statement.humidity);

  Serial.print("CO2 Concentration : ");
  Serial.print(statement.co2);
  Serial.println(" ppm");

  return statement;
}

void sendStatement(Statement statement) {
  Serial.println("Sending of the statement");

  // Store measured value into point
  sensor.clearFields();
  sensor.addField("temperature", statement.temperature);
  sensor.addField("humidity", statement.humidity);
  sensor.addField("co", statement.co2);

  // Print what are we exactly writing
  Serial.print("Writing: ");
  Serial.println(client.pointToLineProtocol(sensor));
  // Write point
  if (!client.writePoint(sensor)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }
  
}




