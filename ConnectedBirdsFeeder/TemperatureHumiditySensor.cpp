#include "TemperatureHumiditySensor.h"

#include "FeederConfiguration.h"

#include <iostream>  
#include <exception>
#include <Arduino.h>  
#include "Logger.h"

TemperatureHumiditySensor::TemperatureHumiditySensor() : DHT(DHT_PIN, DHT_TYPE) {
  _isInitialized = false;
};

TemperatureHumiditySensor::TemperatureHumiditySensor(uint8_t pin, uint8_t type) : DHT(pin, type) {
    _isInitialized = false;
}

void TemperatureHumiditySensor::initialize() {
  try {
    logger->println("Temperature and humidity sensor initialization");
    begin();
    _isInitialized = true;
  }
  catch(std::exception& e) {
    logger->println(e.what());
  }
}

float TemperatureHumiditySensor::retreiveHumidity() {
  _humidity = readHumidity();
  return _humidity;
}

float TemperatureHumiditySensor::retreiveTemperature() {
  _temperature = readTemperature();
  return _temperature;
}

bool TemperatureHumiditySensor::isInitialized() {
  return _isInitialized;
}
