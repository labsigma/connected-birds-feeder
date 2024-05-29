#include <WiFi.h>
#include <WiFiMulti.h>

#include "FeederConfiguration.h"
#include "TemperatureHumiditySensor.h"
#include "GazSensor.h"
#include "WifiConfiguration.h"
#include "FeederCamera.h"
#include "esp_sleep.h"

#include <InfluxDbClient.h>

#define uS_TO_S_FACTOR 1000000

TemperatureHumiditySensor temperatureHumiditySensor(DHT_PIN, DHT_TYPE);
GazSensor gazSensor(MQ135_PIN);

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);
Point sensor("airSensors");

WiFiMulti wifiMulti;
FeederCamera feederCamera;

struct Statement {
  float temperature;
  float humidity;
  float co2;
  bool isTemperatureCorrect;
  bool isCo2Correct;
};

RTC_DATA_ATTR int bootCount = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Start of setup");

   ++bootCount;
      Serial.println("----------------------");
      Serial.println(String(bootCount)+ "eme Boot ");  

    // Get measurements if it's the first boot
    if (bootCount == 1) {
      manageMeasurements();
    }

    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

    if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT0) { // Something detected by the camera
        Serial.println("Wakeup caused by external signal using RTC_IO"); 
        feederCamera.initializeCamera();
        
        if (feederCamera.isCameraInitialized()) {
          String image = feederCamera.takePicture();
          bool wifiConnected = initializeWifi();
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
        Serial.println("Wakeup caused by timer");
        manageMeasurements();
    }

    esp_sleep_enable_ext0_wakeup(PIR_PIN, HIGH);
    esp_sleep_enable_timer_wakeup(MEASUREMENT_INTERVAL * uS_TO_S_FACTOR);
    Serial.println("Setup ESP32 to sleep for every " + String(MEASUREMENT_INTERVAL) + " Seconds");

    Serial.println("Going to sleep now");
    Serial.println("----------------------"); 
    esp_deep_sleep_start();
  
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
}

bool initializeWifi() {
  Serial.println("WiFi initialization");
  bool wifiConnected = false; 

  for (int i = 0; i < NB_WIFI_ENDPOINTS; i++) {
    wifiMulti.addAP(wifi_endpoints[i].ssid, wifi_endpoints[i].cle);
  }

  Serial.println("WiFi connection...");
  if(wifiMulti.run() == WL_CONNECTED) {
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      Serial.println("");
      wifiConnected = true;
  }

  return wifiConnected;
}

void initializeClientInfluxDb() {

  Serial.print("Client influxDb initialization");
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

  statement.isTemperatureCorrect = true;
  statement.isCo2Correct = true;

  Serial.println("Temperature and humidity recovery");
  if (temperatureHumiditySensor.isInitialized()) {
    statement.humidity = temperatureHumiditySensor.retreiveHumidity();
    statement.temperature = temperatureHumiditySensor.retreiveTemperature();  
  }

  // We test that the datas retrieved are corrects
  if (isnan(statement.humidity) || isnan(statement.temperature) ) {
    Serial.println("Unable to recover from temperature and humidity sensor !");
    statement.isTemperatureCorrect = false;
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

  if (isnan(statement.co2)) {
    Serial.println("Unable to recover from co2 sensor !");
    statement.isCo2Correct = false;
    statement.co2 = 0;
  }
  else {
    Serial.print("CO2 Concentration : ");
    Serial.print(statement.co2);
    Serial.println(" ppm");
  }

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



