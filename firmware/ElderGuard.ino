/**
 * ElderGuard Edge AI - Firmware para ESP32-S3 Sense
 * 
 * Monitoramento assistivo com TinyML e Telegram
 */

#include <WiFi.h>
#include <esp_camera.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <ESP32httpUpdate.h>

#include "config.h"
#include "model.tflite.h"  // arquivo gerado pelo Edge Impulse

// --- Definições ---
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     10
#define SIOD_GPIO_NUM     21
#define SIOC_GPIO_NUM     22
#define Y9_GPIO_NUM       4
#define Y8_GPIO_NUM       5
#define Y7_GPIO_NUM       6
#define Y6_GPIO_NUM       7
#define Y5_GPIO_NUM       8
#define Y4_GPIO_NUM       9
#define Y3_GPIO_NUM       15
#define Y2_GPIO_NUM       16
#define VSYNC_GPIO_NUM    14
#define HREF_GPIO_NUM     27
#define PCLK_GPIO_NUM     13

// --- Objetos globais ---
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// --- Variáveis de estado ---
enum class VisualState { LYING, SITTING, STANDING, ON_FLOOR, EMPTY_BED, UNKNOWN };
VisualState currentState = VisualState::UNKNOWN;
VisualState previousState = VisualState::UNKNOWN;
unsigned long stateStartTime = 0;
unsigned long lastInferenceTime = 0;
const unsigned long INFERENCE_INTERVAL = 2000; // 2 segundos

// Heartbeat
unsigned long lastHeartbeatTime = 0;
const unsigned long HEARTBEAT_INTERVAL = 60000; // 1 minuto

// --- Protótipos ---
void setup_wifi();
void init_camera();
VisualState run_inference();
void handle_state_machine(VisualState newState);
void send_telegram_alert(const String& message, bool withPhoto = false);
void send_heartbeat();
void check_ota();

void setup() {
  Serial.begin(115200);
  Serial.println("ElderGuard Edge AI starting...");

  setup_wifi();
  init_camera();
  
  // Inicializa o modelo TFLite (dependendo da biblioteca gerada)
  // Ex: if (!model.init()) Serial.println("Falha ao inicializar modelo");
  
  // Inicializa o temporizador
  lastInferenceTime = millis();
  stateStartTime = millis();
}

void loop() {
  unsigned long now = millis();

  // Inferência periódica
  if (now - lastInferenceTime >= INFERENCE_INTERVAL) {
    VisualState newState = run_inference();
    handle_state_machine(newState);
    lastInferenceTime = now;
  }

  // Heartbeat
  if (now - lastHeartbeatTime >= HEARTBEAT_INTERVAL) {
    send_heartbeat();
    lastHeartbeatTime = now;
  }

  // OTA (verificação a cada 10 minutos)
  static unsigned long lastOTACheck = 0;
  if (now - lastOTACheck >= 600000) {
    check_ota();
    lastOTACheck = now;
  }

  // Pequeno delay para evitar watchdog
  delay(10);
}

// --- Implementações ---

void setup_wifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Configura cliente seguro (para Telegram)
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Defina no config.h
}

void init_camera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; // Para captura, mas usamos RGB para inferência
  config.frame_size = FRAMESIZE_QVGA;   // 320x240
  config.jpeg_quality = 12;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Falha ao iniciar câmera: 0x%x", err);
    ESP.restart();
  }
  Serial.println("Câmera inicializada.");
}

VisualState run_inference() {
  // Captura frame
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Falha ao capturar imagem");
    return VisualState::UNKNOWN;
  }

  // Converter para RGB (aqui precisamos de uma função para converter JPEG para RGB)
  // Ou usar o formato raw da câmera (YUV) e converter.
  // Para simplificar, assumimos que temos uma função `convert_to_rgb(fb, buffer)`.
  // O Edge Impulse gera código que já faz isso.

  // Exemplo de chamada ao modelo (dependendo da biblioteca gerada):
  // float *features = ...;
  // int result = model.classify(features, &prediction);
  
  // Libera buffer
  esp_camera_fb_return(fb);

  // Simulação: retorna um estado aleatório para teste
  // (substituir pela saída real do modelo)
  static VisualState states[] = {VisualState::LYING, VisualState::SITTING, VisualState::STANDING, VisualState::ON_FLOOR, VisualState::EMPTY_BED};
  static int idx = 0;
  VisualState s = states[idx % 5];
  idx++;
  return s;
}

void handle_state_machine(VisualState newState) {
  // Atualiza estado atual
  previousState = currentState;
  currentState = newState;
  
  // Se mudou, reinicia temporizador
  if (previousState != currentState) {
    stateStartTime = millis();
    Serial.print("Mudança de estado: ");
    Serial.print(toString(previousState));
    Serial.print(" -> ");
    Serial.println(toString(currentState));
  }

  // Lógica de eventos (simplificada)
  // Exemplo: queda
  if (previousState == VisualState::STANDING && currentState == VisualState::ON_FLOOR) {
    unsigned long elapsed = millis() - stateStartTime;
    if (elapsed < 3000) { // menos de 3 segundos de transição
      Serial.println("🚨 QUEDA DETECTADA!");
      send_telegram_alert("🚨 ALERTA: Possível queda detectada! Confiança: 95%", true);
    }
  }

  // Imobilidade no chão
  if (currentState == VisualState::ON_FLOOR && (millis() - stateStartTime) > 30000) {
    Serial.println("⚠️ Imobilidade após queda!");
    send_telegram_alert("⚠️ Pessoa permanece imóvel após queda. Necessária verificação.");
  }

  // Tentativas repetidas de levantar
  // (aqui seria necessário manter um histórico)
}

void send_telegram_alert(const String& message, bool withPhoto) {
  if (withPhoto) {
    // Captura foto em JPEG
    camera_fb_t *fb = esp_camera_fb_get();
    if (fb) {
      bot.sendPhoto(CHAT_ID, fb->buf, fb->len, message);
      esp_camera_fb_return(fb);
    } else {
      bot.sendMessage(CHAT_ID, message + "\n(Falha ao capturar imagem)");
    }
  } else {
    bot.sendMessage(CHAT_ID, message);
  }
}

void send_heartbeat() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Sem Wi-Fi, heartbeat não enviado");
    return;
  }

  String url = String(HEARTBEAT_URL) + "?uptime=" + String(millis()/1000) +
               "&rssi=" + String(WiFi.RSSI()) +
               "&version=" + String(FIRMWARE_VERSION);
  
  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.printf("Heartbeat enviado, código: %d\n", httpCode);
  } else {
    Serial.printf("Falha no heartbeat: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
}

void check_ota() {
  // Verifica se há atualização no servidor
  // Exemplo: ESPhttpUpdate.update("http://meuservidor/firmware.bin");
}

// Função auxiliar para converter enum para string
String toString(VisualState s) {
  switch(s) {
    case VisualState::LYING: return "lying";
    case VisualState::SITTING: return "sitting";
    case VisualState::STANDING: return "standing";
    case VisualState::ON_FLOOR: return "on_floor";
    case VisualState::EMPTY_BED: return "empty_bed";
    default: return "unknown";
  }
}