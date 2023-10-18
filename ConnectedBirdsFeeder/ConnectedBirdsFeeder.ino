#include <WiFi.h>
#include <WiFiMulti.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <array>

#include "SensorsConfiguration.h"
#include "TemperatureHumiditySensor.h"
#include "GazSensor.h"
#include "WifiConfiguration.h"

#define MOCK false // true : allows you to simulate the recovery of measurements without sensors

TemperatureHumiditySensor temperatureHumiditySensor;
GazSensor gazSensor;

WiFiMulti wifiMulti;
float start;

struct Statement {
  int idFeeder;
  float temperature;
  float humidity;
  float co2;
};

void setup() {
  Serial.begin(115200);
  Serial.println("Start of setup");

  initializeWifi();
  temperatureHumiditySensor.initialize();

  Serial.println("End of setup");
  start = 0;
}

void loop() {

  bool wifiConnecte = true; 
  if(wifiMulti.run() != WL_CONNECTED) {
    wifiConnecte = false;
    Serial.println("WiFi not connected !");
  }
  
  if ((start == 0) || (start + MEASUREMENT_INTERVAL < millis())) {
    start = millis();
    Statement statement = retreiveMeasurements();
    if (wifiConnecte) {
      sendStatement(statement);
    }
  }

}

void initializeWifi() {
  Serial.println(F("WiFi initialization"));

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
  }
}

Statement retreiveMeasurements() {
  Serial.println(F("Retrieval of measurements"));
  Statement statement;
  statement.idFeeder = ID_FEEDER;

  #if MOCK
    statement.humidity = 46.3;
    statement.temperature = 25.9;  
    statement.co2 = 123;
  #else
    Serial.println(F("Temperature and humidity recovery"));
    if (temperatureHumiditySensor.isInitialized()) {
      statement.humidity = temperatureHumiditySensor.retreiveHumidity();
      statement.temperature = temperatureHumiditySensor.retreiveTemperature();  
    }

    // We test that the datas retrieved are corrects
    if (isnan(statement.humidity) || isnan(statement.temperature) ) {
      Serial.println(F("Unable to recover from temperature and humidity sensor !"));
    }
    else {
      Serial.print("Humidity: ");
      Serial.println(statement.humidity);
      Serial.print("Temperature: ");
      Serial.print(statement.temperature);
      Serial.println("°C ");
    }

    Serial.println(F("CO2 concentration recovery"));
    statement.co2 = gazSensor.retreiveCO2Concentration(statement.temperature, statement.humidity);

    Serial.print("CO2 Concentration : ");
    Serial.print(statement.co2);
    Serial.println(" ppm");

  #endif

  return statement;
}

void sendStatement(Statement statement) {
  Serial.println(F("Sending of the statement"));
  HTTPClient http;   
  http.setTimeout(30000);

  http.begin(URL_ENPOINT);  
  http.addHeader("Content-Type", "application/json");   

  StaticJsonDocument<200> doc;

  doc["idFeeder"] = statement.idFeeder;
  doc["temperature"] = statement.temperature;
  doc["humidity"] = statement.humidity;
  doc["co2"] = statement.co2;

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
    Serial.printf("Error sending statement : %s\n", http.errorToString(httpResponseCode).c_str());
  }
  Serial.println("");

  http.end();
  
}


