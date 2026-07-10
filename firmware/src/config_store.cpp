#include "config_store.h"

#include "config.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <FS.h>

namespace {
const char* kConfigPath = "/config.json";
RuntimeConfig g_config;

void applyDefaults(RuntimeConfig& cfg) {
    cfg.wifiSsid = DEFAULT_WIFI_SSID;
    cfg.wifiPassword = DEFAULT_WIFI_PASSWORD;
    cfg.botToken = DEFAULT_BOT_TOKEN;
    cfg.chatId = DEFAULT_CHAT_ID;
    cfg.heartbeatUrl = DEFAULT_HEARTBEAT_URL;
    cfg.otaUrl = DEFAULT_OTA_URL;
}

String readStringOrDefault(JsonVariantConst value, const String& fallback) {
    return value.is<const char*>() ? String(value.as<const char*>()) : fallback;
}
}  // namespace

bool initConfigStore() {
    if (SPIFFS.begin(true)) {
        return true;
    }

    Serial.println("Falha ao inicializar SPIFFS.");
    return false;
}

bool saveRuntimeConfig() {
    DynamicJsonDocument doc(1024);
    doc["wifi_ssid"] = g_config.wifiSsid;
    doc["wifi_password"] = g_config.wifiPassword;
    doc["bot_token"] = g_config.botToken;
    doc["chat_id"] = g_config.chatId;
    doc["heartbeat_url"] = g_config.heartbeatUrl;
    doc["ota_url"] = g_config.otaUrl;

    File file = SPIFFS.open(kConfigPath, FILE_WRITE);
    if (!file) {
        Serial.println("Nao foi possivel abrir config.json para escrita.");
        return false;
    }

    size_t written = serializeJsonPretty(doc, file);
    file.close();

    if (written == 0) {
        Serial.println("Falha ao salvar config.json.");
        return false;
    }

    return true;
}

bool loadRuntimeConfig() {
    applyDefaults(g_config);

    if (!SPIFFS.exists(kConfigPath)) {
        Serial.println("config.json nao encontrado. Gravando valores padrao.");
        return saveRuntimeConfig();
    }

    File file = SPIFFS.open(kConfigPath, FILE_READ);
    if (!file) {
        Serial.println("Nao foi possivel abrir config.json para leitura.");
        return false;
    }

    DynamicJsonDocument doc(1024);
    DeserializationError err = deserializeJson(doc, file);
    file.close();

    if (err) {
        Serial.printf("Erro ao ler config.json: %s\n", err.c_str());
        Serial.println("Recriando config.json com valores padrao.");
        applyDefaults(g_config);
        return saveRuntimeConfig();
    }

    g_config.wifiSsid = readStringOrDefault(doc["wifi_ssid"], DEFAULT_WIFI_SSID);
    g_config.wifiPassword = readStringOrDefault(doc["wifi_password"], DEFAULT_WIFI_PASSWORD);
    g_config.botToken = readStringOrDefault(doc["bot_token"], DEFAULT_BOT_TOKEN);
    g_config.chatId = readStringOrDefault(doc["chat_id"], DEFAULT_CHAT_ID);
    g_config.heartbeatUrl = readStringOrDefault(doc["heartbeat_url"], DEFAULT_HEARTBEAT_URL);
    g_config.otaUrl = readStringOrDefault(doc["ota_url"], DEFAULT_OTA_URL);

    return true;
}

const RuntimeConfig& getRuntimeConfig() {
    return g_config;
}

bool updateRuntimeConfig(const RuntimeConfig& newConfig) {
    g_config = newConfig;
    return saveRuntimeConfig();
}

String getConfigSummary() {
    String summary;
    summary += "wifi_ssid=" + g_config.wifiSsid;
    summary += ", chat_id=" + g_config.chatId;
    summary += ", heartbeat_configured=" + String(g_config.heartbeatUrl.length() > 0 ? "true" : "false");
    summary += ", ota_configured=" + String(g_config.otaUrl.length() > 0 ? "true" : "false");
    return summary;
}
