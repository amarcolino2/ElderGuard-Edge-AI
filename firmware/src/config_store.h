#ifndef CONFIG_STORE_H
#define CONFIG_STORE_H

#include <Arduino.h>

struct RuntimeConfig {
    String wifiSsid;
    String wifiPassword;
    String botToken;
    String chatId;
    String heartbeatUrl;
    String otaUrl;
};

bool initConfigStore();
bool loadRuntimeConfig();
bool saveRuntimeConfig();
const RuntimeConfig& getRuntimeConfig();
bool updateRuntimeConfig(const RuntimeConfig& newConfig);
String getConfigSummary();

#endif