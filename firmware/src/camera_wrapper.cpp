#include "camera_wrapper.h"
#include <Arduino.h>

// XIAO ESP32S3 Sense (OV2640)
// If you use another board revision, adjust these pins.
static constexpr int PWDN_GPIO_NUM = -1;
static constexpr int RESET_GPIO_NUM = -1;
static constexpr int XCLK_GPIO_NUM = 10;
static constexpr int SIOD_GPIO_NUM = 40;
static constexpr int SIOC_GPIO_NUM = 39;

static constexpr int Y9_GPIO_NUM = 48;
static constexpr int Y8_GPIO_NUM = 11;
static constexpr int Y7_GPIO_NUM = 12;
static constexpr int Y6_GPIO_NUM = 14;
static constexpr int Y5_GPIO_NUM = 16;
static constexpr int Y4_GPIO_NUM = 18;
static constexpr int Y3_GPIO_NUM = 17;
static constexpr int Y2_GPIO_NUM = 15;
static constexpr int VSYNC_GPIO_NUM = 38;
static constexpr int HREF_GPIO_NUM = 47;
static constexpr int PCLK_GPIO_NUM = 13;

static framesize_t currentResolution = FRAMESIZE_QVGA;
static bool cameraReady = false;

bool initCamera() {
    if (cameraReady) {
        return true;
    }

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
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_QVGA; // padrão
    config.jpeg_quality = 12;
    config.fb_count = psramFound() ? 2 : 1;
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Falha ao iniciar câmera: 0x%x\n", err);
        return false;
    }

    sensor_t *s = esp_camera_sensor_get();
    if (s != nullptr) {
        s->set_brightness(s, 0);
        s->set_contrast(s, 0);
        s->set_saturation(s, 0);
        s->set_whitebal(s, 1);
        s->set_gain_ctrl(s, 1);
        s->set_exposure_ctrl(s, 1);
    }

    cameraReady = true;
    Serial.println("Câmera inicializada (QVGA).");
    return true;
}

bool setCameraResolution(framesize_t size) {
    if (!cameraReady) {
        return false;
    }

    sensor_t *s = esp_camera_sensor_get();
    if (s == nullptr) return false;
    int ret = s->set_framesize(s, size);
    if (ret == 0) {
        currentResolution = size;
        return true;
    }
    return false;
}

camera_fb_t* captureImage(framesize_t size) {
    if (!cameraReady) {
        return nullptr;
    }

    if (size != currentResolution) {
        if (!setCameraResolution(size)) {
            Serial.println("Falha ao mudar resolução da câmera.");
            return nullptr;
        }
    }

    camera_fb_t* fb = nullptr;
    for (int attempt = 0; attempt < 3 && fb == nullptr; attempt++) {
        fb = esp_camera_fb_get();
        if (fb == nullptr) {
            delay(30);
        }
    }
    return fb;
}

void freeImage(camera_fb_t *fb) {
    if (fb) esp_camera_fb_return(fb);
}