#ifndef GazSensor_h
#define GazSensor_h

#include <MQ135.h>

class GazSensor : public MQ135 {
  private:
    float _co2;
  public:
    GazSensor();
    GazSensor(uint8_t pin);
    float retreiveCO2Concentration(float temperature, float humidity);
};

#endif