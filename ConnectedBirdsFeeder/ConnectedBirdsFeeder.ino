#include <WiFi.h>
#include <WiFiMulti.h>

#include "FeederConfiguration.h"
#include "TemperatureHumiditySensor.h"
#include "GazSensor.h"
#include "WifiConfiguration.h"
#include "FeederCamera.h"

#include <InfluxDbClient.h>
#include <LiquidCrystal_I2C.h>

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

RTC_DATA_ATTR int bootCount = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2);
byte degree_symbol[8] = {0b01100, 0b10010, 0b01100, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000};


void setup() {
  Serial.begin(115200);
  Serial.println("Start of setup");

  pinMode(GPIO_NUM_33, OUTPUT);
  //digitalWrite(GPIO_NUM_33, LOW);

  pinMode(PORT_LED_FLASH, OUTPUT);

   ++bootCount;
      Serial.println("----------------------");
      Serial.println(String(bootCount)+ "eme Boot ");  

  if (USE_LCD) {
    // initialize LCD
    lcd.init();
    // turn on LCD backlight                      
    lcd.backlight();

    lcd.createChar(0, degree_symbol);
  }

  if (DEEP_SLEEP) {
    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

    if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT0) { // Something detected by the camera
        Serial.println("Wakeup caused by external signal using RTC_IO"); 
        digitalWrite(GPIO_NUM_33, HIGH);
        printMessage("Bird detected");
        feederCamera.initializeCamera();
        
        if (feederCamera.isCameraInitialized()) {
          String image = feederCamera.takePicture();
          bool wifiConnecte = initializeWifi();
          if ((image != "") && wifiConnecte){
            if (SEND_TO_GED) {
              feederCamera.sendPictureToGed(image);
            }
            else {
              feederCamera.sendPicture(image); 
            }
          }
        }
        delay(2000);
    }
    else if (wakeup_reason == ESP_SLEEP_WAKEUP_TIMER) { // Retreive and send measurements periodically
        Serial.println("Wakeup caused by timer");
        digitalWrite(PORT_LED_FLASH, HIGH);
        
        temperatureHumiditySensor.initialize();
        initializeClientInfluxDb();
        
        Statement statement = retreiveMeasurements();
        printStatement(statement);
        bool wifiConnecte = initializeWifi();
        if (wifiConnecte) {
          sendStatement(statement);
        }
        delay(500);
        digitalWrite(PORT_LED_FLASH, LOW);
    }

    esp_sleep_enable_ext0_wakeup(PIR_PIN, HIGH);
    esp_sleep_enable_timer_wakeup(MEASUREMENT_INTERVAL * uS_TO_S_FACTOR);
    Serial.println("Setup ESP32 to sleep for every " + String(MEASUREMENT_INTERVAL) + " Seconds");

    Serial.println("Going to sleep now");
    Serial.println("----------------------"); 
    esp_deep_sleep_start();
  }
  else {
    initializeWifi();
    temperatureHumiditySensor.initialize();

    initializeClientInfluxDb();

    feederCamera.initializeCamera();
  }
}

void loop() {
  // DEBUG
  delay(10000);
  bool wifiConnecte = false;

  if(wifiMulti.run() == WL_CONNECTED) {
    wifiConnecte = true;
  }
  
  digitalWrite(PORT_LED_FLASH, HIGH);
  if (feederCamera.isCameraInitialized()) {
    String image = feederCamera.takePicture();
    digitalWrite(PORT_LED_FLASH, LOW);
    if ((image != "") && wifiConnecte){
      feederCamera.sendPictureToGed(image); 
    }
  }
  delay(60000);
}

bool initializeWifi() {
  Serial.println("WiFi initialization");
  bool wifiConnecte = false; 

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

void printStatement(Statement statement) {
  if (USE_LCD) {
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("T : ");
    lcd.print(statement.temperature);
    lcd.write(byte(0)); // print the degree symbol
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("H : ");
    lcd.print(statement.humidity);
    lcd.print("%");
  }
}

void printMessage(String message) {
  if (USE_LCD) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(message);
  }
}




