#ifndef TemperatureHumiditySensor_h
#define TemperatureHumiditySensor_h

#include "DHT.h"
#include "Logger.h"

class TemperatureHumiditySensor : public DHT {
  private:
    float _temperature;
    float _humidity;
    bool _isInitialized;
    Logger* logger = Logger::getInstance();
  public:
    TemperatureHumiditySensor();
    TemperatureHumiditySensor(uint8_t pin, uint8_t type);
    void initialize();
    float retreiveHumidity();
    float retreiveTemperature();
    bool isInitialized();
};

#endif