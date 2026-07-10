#include <Arduino.h>
#include "config.h"
#include "wifi_manager.h"
#include "camera_wrapper.h"
#include "classifier.h"
#include "config_store.h"
#include "state_machine.h"
#include "telegram_notifier.h"
#include "heartbeat.h"
#include "ota_update.h"

// Variáveis globais
unsigned long lastInference = 0;
unsigned long lastHeartbeat = 0;
unsigned long lastOTA = 0;
unsigned long lastDeepSleep = 0;

// Flag para controle de envio
bool alertSent = false;

void setup() {
    Serial.begin(115200);
    Serial.println("\n\n=== ELDERGUARD EDGE AI ===");
    Serial.printf("Versão: %s\n", FIRMWARE_VERSION);

    if (initConfigStore() && loadRuntimeConfig()) {
        Serial.println("Configuracao carregada do SPIFFS.");
        Serial.println(getConfigSummary());
    } else {
        Serial.println("Falha ao carregar config persistente. Usando defaults.");
    }

    setupWiFi();
    if (!initCamera()) {
        Serial.println("Erro crítico na câmera. Reiniciando...");
        ESP.restart();
    }
    if (!initClassifier()) {
        Serial.println("Erro ao inicializar classificador.");
    }
    initStateMachine();
    initTelegram();

    lastInference = millis();
    lastHeartbeat = millis();
    lastOTA = millis();

    Serial.println("Sistema iniciado.");
}

void loop() {
    unsigned long now = millis();

    // Watchdog Wi-Fi
    wifiWatchdog();

    // Inferência periódica
    if (now - lastInference >= INFERENCE_INTERVAL) {
        // Captura em QVGA para inferência
        camera_fb_t *fb = captureImage(FRAMESIZE_QVGA);
        if (fb) {
            VisualState state = runInference(fb);
            freeImage(fb);
            updateStateMachine(state);

            // Se evento crítico e ainda não enviado
            if (isCriticalEventDetected() && !alertSent) {
                String msg = getLastEventMessage();
                bool withPhoto = shouldSendPhoto();

                if (withPhoto) {
                    // Captura em UXGA para foto
                    camera_fb_t *photo = captureImage(FRAMESIZE_UXGA);
                    if (photo) {
                        sendTelegramPhoto(msg, photo->buf, photo->len);
                        freeImage(photo);
                    } else {
                        sendTelegramMessage(msg + "\n(Falha ao capturar foto)");
                    }
                } else {
                    sendTelegramMessage(msg);
                }

                // Reseta flags após envio
                resetCriticalEvent();
                alertSent = true;
            }
        } else {
            Serial.println("Falha ao capturar imagem.");
        }
        lastInference = now;
    }

    // Se não houver evento crítico, permite reset do alertSent após um tempo
    if (!isCriticalEventDetected() && alertSent) {
        // Aguarda 10 segundos para resetar o flag
        static unsigned long alertResetTime = 0;
        if (alertResetTime == 0) alertResetTime = millis();
        if (millis() - alertResetTime > 10000) {
            alertSent = false;
            alertResetTime = 0;
        }
    }

    // Heartbeat
    if (now - lastHeartbeat >= HEARTBEAT_INTERVAL) {
        sendHeartbeat();
        lastHeartbeat = now;
    }

    // OTA
    if (now - lastOTA >= OTA_CHECK_INTERVAL) {
        checkForOTAUpdate();
        lastOTA = now;
    }

#ifdef ENABLE_DEEP_SLEEP
    // Deep sleep: após um período sem eventos, entra em sono
    if (now - lastDeepSleep >= DEEP_SLEEP_INTERVAL) {
        Serial.println("Entrando em deep sleep por 10s...");
        esp_sleep_enable_timer_wakeup(10000000); // 10 segundos
        esp_deep_sleep_start();
    }
#endif

    // Pequeno delay para evitar watchdog
    delay(10);
}