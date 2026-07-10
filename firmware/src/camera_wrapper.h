#ifndef CAMERA_WRAPPER_H
#define CAMERA_WRAPPER_H

#include <esp_camera.h>

bool initCamera();
camera_fb_t* captureImage(framesize_t size = FRAMESIZE_QVGA); // parâmetro opcional
void freeImage(camera_fb_t *fb);
bool setCameraResolution(framesize_t size);

#endif