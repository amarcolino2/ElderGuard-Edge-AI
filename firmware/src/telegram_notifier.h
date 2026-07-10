#ifndef TELEGRAM_NOTIFIER_H
#define TELEGRAM_NOTIFIER_H

#include <Arduino.h>

bool initTelegram();
bool sendTelegramMessage(const String& message);
bool sendTelegramPhoto(const String& message, const uint8_t* photoData, size_t photoLen);
void sendAlert(const String& message, bool withPhoto = false);

#endif