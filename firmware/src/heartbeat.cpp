// heartbeat.cpp
#include "heartbeat.h"
#include "config.h"
#include "config_store.h"
#include "wifi_manager.h"
#include <HTTPClient.h>
#include <Arduino.h>

void sendHeartbeat() {
    const RuntimeConfig& cfg = getRuntimeConfig();

    if (!isWiFiConnected()) {
        Serial.println("Sem Wi-Fi, heartbeat não enviado.");
        return;
    }

    if (cfg.heartbeatUrl.length() == 0) {
        Serial.println("Heartbeat URL nao configurada. Pulando envio.");
        return;
    }

    String url = cfg.heartbeatUrl + 
                 "?uptime=" + String(millis()/1000) +
                 "&rssi=" + String(getRSSI()) +
                 "&version=" + String(FIRMWARE_VERSION);

    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
        Serial.printf("Heartbeat enviado, código: %d\n", httpCode);
    } else {
        Serial.printf("Falha no heartbeat: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
}