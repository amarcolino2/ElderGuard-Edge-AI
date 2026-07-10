#include "state_machine.h"
#include <Arduino.h>

static VisualState currentState = VisualState::UNKNOWN;
static VisualState previousState = VisualState::UNKNOWN;
static unsigned long stateStartTime = 0;
static unsigned long lastStateChange = 0;

static VisualState history[10];
static int historyIndex = 0;

static bool criticalEvent = false;
static String eventMessage = "";
static bool sendPhotoFlag = false;

// Para fusão de sensores
static float externalConfidence = 0.0;
static String externalEventType = "";

void initStateMachine() {
    currentState = VisualState::UNKNOWN;
    previousState = VisualState::UNKNOWN;
    stateStartTime = millis();
    lastStateChange = millis();
    for (int i = 0; i < 10; i++) history[i] = VisualState::UNKNOWN;
    historyIndex = 0;
    criticalEvent = false;
    eventMessage = "";
    sendPhotoFlag = false;
    externalConfidence = 0.0;
}

void updateStateMachine(VisualState newState) {
    // Atualiza histórico
    history[historyIndex % 10] = newState;
    historyIndex++;

    if (newState != currentState) {
        previousState = currentState;
        currentState = newState;
        stateStartTime = millis();
        lastStateChange = millis();

        Serial.print("Mudança de estado: ");
        Serial.print(visualStateToString(previousState));
        Serial.print(" -> ");
        Serial.println(visualStateToString(currentState));
    }

    // Regras internas (já existentes)
    // 1. Queda
    if (previousState == VisualState::STANDING && currentState == VisualState::ON_FLOOR) {
        if (millis() - lastStateChange < 3000) {
            criticalEvent = true;
            eventMessage = "🚨 ALERTA: Possível queda detectada!";
            sendPhotoFlag = true;
        }
    }

    // 2. Imobilidade no chão
    if (currentState == VisualState::ON_FLOOR && (millis() - stateStartTime) > 30000) {
        criticalEvent = true;
        eventMessage = "⚠️ Pessoa imóvel no chão por 30s. Verificação necessária!";
        sendPhotoFlag = true;
    }

    // 3. Tentativa de levantar
    if (historyIndex >= 3) {
        VisualState last3[3] = {
            history[(historyIndex-3) % 10],
            history[(historyIndex-2) % 10],
            history[(historyIndex-1) % 10]
        };
        if (last3[0] == VisualState::LYING && last3[1] == VisualState::SITTING && last3[2] == VisualState::STANDING) {
            criticalEvent = true;
            eventMessage = "🔄 Tentativa de levantar detectada.";
            sendPhotoFlag = false;
        }
    }

    // 4. Saiu da cama
    if (currentState == VisualState::EMPTY_BED && (millis() - stateStartTime) > 300000) {
        criticalEvent = true;
        eventMessage = "🛏️ Pessoa fora da cama por 5+ min.";
        sendPhotoFlag = false;
    }

    // Verifica se há evento externo de alta confiança
    if (externalConfidence > 0.8) {
        if (externalEventType == "FALL") {
            criticalEvent = true;
            eventMessage = "🔴 Radar detectou queda! Confiança: " + String(externalConfidence * 100) + "%";
            sendPhotoFlag = true;
        } else if (externalEventType == "MOVEMENT") {
            // Pode usar para complementar ou descartar falsos positivos
            // Exemplo: se câmera vê empty_bed mas radar detecta movimento, pode ser falso alarme
        }
    }
}

bool isCriticalEventDetected() {
    return criticalEvent;
}

String getLastEventMessage() {
    return eventMessage;
}

bool shouldSendPhoto() {
    return sendPhotoFlag;
}

void resetCriticalEvent() {
    criticalEvent = false;
    eventMessage = "";
    sendPhotoFlag = false;
    externalConfidence = 0.0;
}

void onExternalEvent(const String& eventType, float confidence) {
    externalEventType = eventType;
    externalConfidence = confidence;
    Serial.printf("Evento externo recebido: %s, confiança: %.2f\n", eventType.c_str(), confidence);
}

String getStateHistory() {
    String hist = "Histórico: ";
    for (int i = 0; i < 10; i++) {
        hist += visualStateToString(history[i]);
        hist += " ";
    }
    return hist;
}