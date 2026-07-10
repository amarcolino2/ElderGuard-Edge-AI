#ifndef CONFIG_H
#define CONFIG_H

// Wi-Fi
#define WIFI_SSID     "SeuSSID"
#define WIFI_PASSWORD "SuaSenha"

// Telegram
#define BOT_TOKEN     "1234567890:ABCdefGHIjklMNOpqrsTUVwxyz"
#define CHAT_ID       "123456789"

// Heartbeat
#define HEARTBEAT_URL "https://uptime.betterstack.com/api/v1/heartbeat/SEU_TOKEN"

// OTA
#define FIRMWARE_VERSION "1.0.0"

// Certificado root do Telegram (para WiFiClientSecure)
static const char TELEGRAM_CERTIFICATE_ROOT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
...
-----END CERTIFICATE-----
)EOF";

#endif