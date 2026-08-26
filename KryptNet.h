#ifndef KRYPT_NET_H
#define KRYPT_NET_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

class KryptNet {
public:
  bool connect(String ssid, String pass, uint32_t timeoutMs = 10000) {
    WiFi.begin(ssid.c_str(), pass.c_str());
    uint32_t start = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - start) < timeoutMs) {
      delay(250);
    }
    return (WiFi.status() == WL_CONNECTED);
  }

  bool isConnected() {
    return (WiFi.status() == WL_CONNECTED);
  }

  String getIP() {
    return WiFi.localIP().toString();
  }

  int fetchURL(String url, String &payload) {
    if (!isConnected()) return -1;
    HTTPClient http;
    http.begin(url);
    int code = http.GET();
    if (code > 0) {
      payload = http.getString();
    }
    http.end();
    return code;
  }
};

#endif
