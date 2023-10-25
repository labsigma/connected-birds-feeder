#include <WiFi.h>
#include <WiFiMulti.h>

#include "SensorsConfiguration.h"
#include "TemperatureHumiditySensor.h"
#include "GazSensor.h"
#include "WifiConfiguration.h"

#include <InfluxDbClient.h>

TemperatureHumiditySensor temperatureHumiditySensor;
GazSensor gazSensor;

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);
Point sensor("airSensors");

WiFiMulti wifiMulti;
float start;

struct Statement {
  float temperature;
  float humidity;
  float co2;
};

void setup() {
  Serial.begin(115200);
  Serial.println("Start of setup");

  initializeWifi();
  temperatureHumiditySensor.initialize();

  initializeClientInfluxDb();

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
  Serial.println(F("Retrieval of measurements"));
  Statement statement;

  #if MOCK
    statement.humidity = 64.6;
    statement.temperature = 19.6;  
    statement.co2 = 45;
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


