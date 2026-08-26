#ifndef KRYPT_HAL_H
#define KRYPT_HAL_H

#include <Arduino.h>
#include <Preferences.h>
#include "KryptConfig.h"

class KryptHAL {
private:
  Preferences prefs;

public:
  void init() {
    // Inicialización del hardware base
  }

  void reboot() {
    ESP.restart();
  }

  // Métodos NVS genéricos para almacenamiento persistente
  bool setKeyBool(const char* ns, const char* key, bool val) {
    prefs.begin(ns, false);
    bool res = prefs.putBool(key, val);
    prefs.end();
    return res;
  }

  bool getKeyBool(const char* ns, const char* key, bool defaultVal = false) {
    prefs.begin(ns, true);
    bool res = prefs.getBool(key, defaultVal);
    prefs.end();
    return res;
  }

  bool setKeyString(const char* ns, const char* key, String val) {
    prefs.begin(ns, false);
    size_t res = prefs.putString(key, val);
    prefs.end();
    return res > 0;
  }

  String getKeyString(const char* ns, const char* key, String defaultVal = "") {
    prefs.begin(ns, true);
    String res = prefs.getString(key, defaultVal);
    prefs.end();
    return res;
  }

  void clearNamespace(const char* ns) {
    prefs.begin(ns, false);
    prefs.clear();
    prefs.end();
  }

  // Telemetría de hardware
  uint32_t getFreeRAM() { return ESP.getFreeHeap(); }
  uint32_t getTotalRAM() { return ESP.getHeapSize(); }
  uint32_t getCPUFreq() { return ESP.getCpuFreqMHz(); }
  uint8_t getCores() { return ESP.getChipCores(); }
  uint32_t getUptime() { return millis() / 1000; }
};

#endif
