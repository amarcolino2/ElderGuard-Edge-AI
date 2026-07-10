#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <Arduino.h>
#include <esp_camera.h>

enum class VisualState {
    LYING,
    SITTING,
    STANDING,
    ON_FLOOR,
    EMPTY_BED,
    UNKNOWN
};

bool initClassifier();

// The active implementation is selected via config.h:
// - USE_FAKE_CLASSIFIER (test mode)
// - USE_REAL_INFERENCE_PIPELINE (embedded model pipeline)
VisualState runInference(camera_fb_t *fb);
const char* visualStateToString(VisualState s);

// Para simular padrões mais realistas
void setFakeStatePattern(int pattern); // 0=aleatório, 1=ciclo normal, 2=queda, etc.

#endif