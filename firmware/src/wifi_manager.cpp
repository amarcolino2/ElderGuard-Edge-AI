#include "wifi_manager.h"
#include "config.h"
#include "config_store.h"
#include <Arduino.h>

static unsigned long lastWiFiCheck = 0;

void setupWiFi() {
    const RuntimeConfig& cfg = getRuntimeConfig();
    WiFi.begin(cfg.wifiSsid.c_str(), cfg.wifiPassword.c_str());
    Serial.print("Conectando ao Wi-Fi");
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWi-Fi conectado!");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nFalha ao conectar Wi-Fi. Tentando novamente mais tarde.");
    }
}

bool isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

int getRSSI() {
    return WiFi.RSSI();
}

void wifiWatchdog() {
    unsigned long now = millis();
    if (now - lastWiFiCheck >= WIFI_RETRY_INTERVAL) {
        if (!isWiFiConnected()) {
            const RuntimeConfig& cfg = getRuntimeConfig();
            Serial.println("Wi-Fi caiu. Tentando reconectar...");
            WiFi.disconnect();
            WiFi.begin(cfg.wifiSsid.c_str(), cfg.wifiPassword.c_str());
        }
        lastWiFiCheck = now;
    }
}