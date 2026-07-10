#include "telegram_notifier.h"
#include "config.h"
#include "config_store.h"
#include "wifi_manager.h"
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

static WiFiClientSecure client;
static UniversalTelegramBot* bot = nullptr;
static const uint8_t* g_photoData = nullptr;
static size_t g_photoLen = 0;
static size_t g_photoIndex = 0;

static bool photoMoreDataAvailable() {
    return g_photoData != nullptr && g_photoIndex < g_photoLen;
}

static byte photoGetNextByte() {
    if (!photoMoreDataAvailable()) {
        return 0;
    }
    return g_photoData[g_photoIndex++];
}

bool initTelegram() {
    const RuntimeConfig& cfg = getRuntimeConfig();
    if (cfg.botToken.length() == 0 || cfg.chatId.length() == 0) {
        Serial.println("Bot token/chat id nao configurados.");
        return false;
    }

    if (strlen(EG_TELEGRAM_CERTIFICATE_ROOT) > 0) {
        client.setCACert(EG_TELEGRAM_CERTIFICATE_ROOT);
    }
    bot = new UniversalTelegramBot(cfg.botToken, client);
    // Teste: verifica se o bot responde
    if (bot->getMe()) {
        Serial.println("Telegram Bot conectado.");
        return true;
    } else {
        Serial.println("Falha ao conectar ao Telegram.");
        return false;
    }
}

bool sendTelegramMessage(const String& message) {
    const RuntimeConfig& cfg = getRuntimeConfig();
    if (!bot || !isWiFiConnected()) return false;
    for (int attempt = 0; attempt < 3; attempt++) {
        if (bot->sendMessage(cfg.chatId, message, "")) {
            return true;
        }
        delay(1000);
    }
    Serial.println("Falha ao enviar mensagem Telegram.");
    return false;
}

bool sendTelegramPhoto(const String& message, const uint8_t* photoData, size_t photoLen) {
    const RuntimeConfig& cfg = getRuntimeConfig();
    if (!bot || !isWiFiConnected() || photoData == nullptr) return false;

    g_photoData = photoData;
    g_photoLen = photoLen;

    for (int attempt = 0; attempt < 3; attempt++) {
        g_photoIndex = 0;
        String response = bot->sendPhotoByBinary(cfg.chatId, "image/jpeg", static_cast<int>(photoLen),
                                                 photoMoreDataAvailable, photoGetNextByte,
                                                 nullptr, nullptr);
        if (response.indexOf("\"ok\":true") != -1) {
            g_photoData = nullptr;
            g_photoLen = 0;
            g_photoIndex = 0;
            return true;
        }
        delay(1000);
    }

    g_photoData = nullptr;
    g_photoLen = 0;
    g_photoIndex = 0;
    Serial.println("Falha ao enviar foto Telegram.");
    return false;
}

void sendAlert(const String& message, bool withPhoto) {
    if (withPhoto) {
        // A foto será capturada externamente, aqui apenas enviamos a mensagem
        sendTelegramMessage(message);
    } else {
        sendTelegramMessage(message);
    }
}