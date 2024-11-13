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

    // Méthode pour initialiser la communication série
    void begin(long baudRate) {
        Serial.begin(baudRate);
    }

    // Méthode pour imprimer un message sans retour à la ligne
    void print(const String &message) {
        Serial.print(message);
        if (_isWebSerialActivated) {
          WebSerial.print(message);
        }
    }

    // Méthode pour imprimer un message avec retour à la ligne
    void println(const String &message) {
        Serial.println(message);
        if (_isWebSerialActivated) {
          WebSerial.println(message);
        }
    }

    // Méthode pour imprimer un message formaté (comme printf)
    void printf(const char *format, ...) {
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
};

#endif