#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

void setupWiFi();
bool isWiFiConnected();
int getRSSI();
void wifiWatchdog(); // Novo: monitora e reconecta se necessário

#endif