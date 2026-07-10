#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// -----------------------------------------------------------------------------
// CLASSIFIER MODE SWITCH
// -----------------------------------------------------------------------------
// TEST MODE (default): keep fake classifier enabled.
#define USE_FAKE_CLASSIFIER 1

// REAL INFERENCE MODE (embedded model pipeline):
// 1) Comment the line above (#define USE_FAKE_CLASSIFIER 1)
// 2) Uncomment the line below
// #define USE_REAL_INFERENCE_PIPELINE 1

// Wi-Fi
#define WIFI_SSID     "seu_ssid"
#define WIFI_PASSWORD "sua_senha"

// Telegram
#define BOT_TOKEN     "1234567890:ABCdefGHIjklMNOpqrsTUVwxyz"
#define CHAT_ID       "987654321"

// Heartbeat (Better Stack / UptimeRobot)
#define HEARTBEAT_URL "https://uptime.betterstack.com/api/v1/heartbeat/SEU_TOKEN"

// OTA
#define FIRMWARE_VERSION "1.0.1"
#define OTA_URL          "http://seu-servidor/firmware.bin"

// Defaults used by runtime config store in SPIFFS
#define DEFAULT_WIFI_SSID      WIFI_SSID
#define DEFAULT_WIFI_PASSWORD  WIFI_PASSWORD
#define DEFAULT_BOT_TOKEN      BOT_TOKEN
#define DEFAULT_CHAT_ID        CHAT_ID
#define DEFAULT_HEARTBEAT_URL  HEARTBEAT_URL
#define DEFAULT_OTA_URL        OTA_URL

// Constantes do sistema
#define INFERENCE_INTERVAL  2000   // ms entre inferências
#define HEARTBEAT_INTERVAL  60000  // ms entre heartbeats
#define OTA_CHECK_INTERVAL  600000 // 10 minutos
#define WIFI_RETRY_INTERVAL 30000  // 30s para tentar reconexão

// Modo de baixo consumo (descomente para ativar)
// #define ENABLE_DEEP_SLEEP
#define DEEP_SLEEP_INTERVAL  10000 // 10 segundos entre ciclos

// Fusão de sensores (interoperabilidade)
#define ENABLE_EXTERNAL_EVENTS

// Certificado raiz customizado (opcional). Se vazio, usa o certificado padrão da biblioteca.
static const char EG_TELEGRAM_CERTIFICATE_ROOT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
... (certificado completo aqui)
-----END CERTIFICATE-----
)EOF";

#endif