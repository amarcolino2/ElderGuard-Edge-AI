#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "classifier.h"

void initStateMachine();
void updateStateMachine(VisualState newState);
bool isCriticalEventDetected();
String getLastEventMessage();
bool shouldSendPhoto();
void resetCriticalEvent(); // Novo: limpa flag após envio

// Interoperabilidade: recebe eventos externos (radar, relógio, etc.)
void onExternalEvent(const String& eventType, float confidence);

// Para logging
String getStateHistory();

#endif