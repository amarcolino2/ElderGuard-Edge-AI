#include "classifier.h"
#include "camera_wrapper.h"
#include <Arduino.h>

#if defined(USE_REAL_INFERENCE_PIPELINE)
#include "model.tflite.h"
#endif

static int pattern = 0; // 0=aleatório, 1=ciclo normal, 2=queda, 3=imobilidade
static unsigned long lastStateChange = 0;
static VisualState fakeState = VisualState::UNKNOWN;

bool initClassifier() {
#if defined(USE_FAKE_CLASSIFIER)
    // Fake classifier path for tests.
    // To switch to real embedded inference pipeline:
    // 1) Comment USE_FAKE_CLASSIFIER in config.h
    // 2) Uncomment USE_REAL_INFERENCE_PIPELINE in config.h
    Serial.println("Classificador fake inicializado.");
    fakeState = VisualState::LYING;
    lastStateChange = millis();
    return true;
#elif defined(USE_REAL_INFERENCE_PIPELINE)
    // Real pipeline bootstrap placeholder.
    // model_tflite[] is linked from model.tflite.h; next step is to wire a
    // TensorFlow Lite Micro interpreter and preprocessing from camera frame.
    Serial.printf("Pipeline real selecionado. Modelo embutido carregado (%u bytes).\n",
                  static_cast<unsigned>(model_tflite_len));
    return true;
#else
    Serial.println("Nenhum modo de classificador definido. Verifique config.h.");
    return false;
#endif
}

void setFakeStatePattern(int p) {
    pattern = p;
}

VisualState runInference(camera_fb_t *fb) {
#if defined(USE_FAKE_CLASSIFIER)
    (void)fb;
    unsigned long now = millis();
    
    // Simula mudanças de estado em intervalos aleatórios ou baseados em padrão
    if (now - lastStateChange > random(3000, 8000)) {
        lastStateChange = now;
        
        switch (pattern) {
            case 0: // Aleatório
                fakeState = static_cast<VisualState>(random(0, 5));
                break;
            case 1: // Ciclo normal: lying -> sitting -> standing -> empty_bed -> lying
                // Implementar ciclo
                break;
            case 2: // Queda: standing -> on_floor rapidamente
                // Implementar sequência de queda
                break;
            case 3: // Imobilidade: on_floor por longo tempo
                fakeState = VisualState::ON_FLOOR;
                break;
            default:
                fakeState = static_cast<VisualState>(random(0, 5));
        }
    }
    return fakeState;
#elif defined(USE_REAL_INFERENCE_PIPELINE)
    // Real embedded inference pipeline entry point.
    // TODO:
    // - Decode/rescale fb into model input tensor
    // - Run interpreter->Invoke()
    // - Convert output logits/probabilities to VisualState
    (void)fb;
    return VisualState::UNKNOWN;
#else
    (void)fb;
    return VisualState::UNKNOWN;
#endif
}

const char* visualStateToString(VisualState s) {
    switch(s) {
        case VisualState::LYING: return "lying";
        case VisualState::SITTING: return "sitting";
        case VisualState::STANDING: return "standing";
        case VisualState::ON_FLOOR: return "on_floor";
        case VisualState::EMPTY_BED: return "empty_bed";
        default: return "unknown";
    }
}