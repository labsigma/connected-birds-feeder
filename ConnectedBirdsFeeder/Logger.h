#ifndef Logger_h
#define Logger_h

#include "FeederConfiguration.h"
#include <WebSerial.h>

class Logger {
private:
    static Logger* instance;
    bool _isWebSerialActivated;
public:
    static Logger* getInstance() {
        if (instance == nullptr) {
            instance = new Logger();
        }
        return instance;
    }

    Logger() {
      _isWebSerialActivated = false;
    }

    void activateWebSerial() {
      _isWebSerialActivated = true;
    }

    // Method to initialize serial communication
    void begin(long baudRate) {
        Serial.begin(baudRate);
    }

    // Method to print a message without newlines
    size_t print(const String &message) {
        Serial.print(message);
        if (_isWebSerialActivated) {
          WebSerial.print(message);
        }
    }

    // Method to print a message with line break
    size_t println(const String &message) {
        Serial.println(message);
        if (_isWebSerialActivated) {
          WebSerial.println(message);
        }
    }

    // Method to print a formatted message (like printf)
    size_t printf(const char *format, ...) {
        char buffer[128];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        Serial.print(buffer);
        if (_isWebSerialActivated) {
          WebSerial.println(buffer);
        }
    }

    // Method to print float
    size_t print(const float &number) {
      Serial.print(number);
      if (_isWebSerialActivated) {
        WebSerial.println(number);
      }
    }
};

#endif