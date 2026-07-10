// ota_update.cpp
#include "ota_update.h"
#include "config.h"
#include "config_store.h"
#include "wifi_manager.h"
#include <HTTPUpdate.h>
#include <WiFiClient.h>
#include <Arduino.h>

static WiFiClient otaClient;

void checkForOTAUpdate() {
    const RuntimeConfig& cfg = getRuntimeConfig();

    if (!isWiFiConnected()) {
        Serial.println("Wi-Fi desconectado, pulando OTA.");
        return;
    }

    if (cfg.otaUrl.length() == 0) {
        Serial.println("OTA URL nao configurada. Pulando OTA.");
        return;
    }

    Serial.println("Verificando atualização OTA...");
    t_httpUpdate_return ret = httpUpdate.update(otaClient, cfg.otaUrl, FIRMWARE_VERSION);

    switch(ret) {
        case HTTP_UPDATE_FAILED:
            Serial.printf("Falha na atualização OTA: %s\n", httpUpdate.getLastErrorString().c_str());
            break;
        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("Nenhuma atualização disponível.");
            break;
        case HTTP_UPDATE_OK:
            Serial.println("OTA concluído com sucesso! Reiniciando...");
            ESP.restart();
            break;
    }
}