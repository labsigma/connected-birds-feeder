#include "GazSensor.h"

#include "SensorsConfiguration.h"

GazSensor::GazSensor() : MQ135(MQ135_PIN) {};

GazSensor::GazSensor(uint8_t pin) : MQ135(pin) {}

float GazSensor::retreiveCO2Concentration(float temperature, float humidity) {
  if (isnan(humidity) || isnan(temperature) ) {
    _co2 = getPPM();
  }
  else {
    _co2 = getCorrectedPPM(temperature, humidity);
  }

  return _co2;
}
