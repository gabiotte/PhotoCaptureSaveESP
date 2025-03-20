#include "esp_camera.h"

#pragma once

#ifndef __CAMERA_H__
#define __CAMERA_H__

#define gc0308

#if defined(nt99141)
    inline pixformat_t pixformat = PIXFORMAT_JPEG;
    inline framesize_t framesize = FRAMESIZE_HD;
    inline const char* camera = "nt99141";

#elif defined(ov7670)
    inline pixformat_t pixformat = PIXFORMAT_RGB565;
    inline framesize_t framesize = FRAMESIZE_UXGA;
    inline const char* camera = "ov7670";

#elif defined(gc0308)
    inline pixformat_t pixformat = PIXFORMAT_RGB565;
    inline framesize_t framesize = FRAMESIZE_UXGA;
    inline const char* camera = "gc0308";

#elif defined(ov2640)
    inline pixformat_t pixformat = PIXFORMAT_JPEG;
    inline framesize_t framesize = FRAMESIZE_UXGA;
    inline const char* camera = "ov2640";

#elif defined(ov7725)
    inline pixformat_t pixformat = PIXFORMAT_RGB565;
    inline framesize_t framesize = FRAMESIZE_UXGA;
    inline const char* camera = "ov7725";
    
#else 
    #error "Nenhuma câmera definida."
#endif

inline const char* FORMATO_ARQUIVO_JPEG = "jpeg"; // jpeg, bmp, rgb565

struct Photo {
    uint8_t* buffer; 
    size_t len;
    unsigned long capture_time;
};

#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    15
#define SIOD_GPIO_NUM    4
#define SIOC_GPIO_NUM    5
#define Y9_GPIO_NUM      16
#define Y8_GPIO_NUM      17
#define Y7_GPIO_NUM      18
#define Y6_GPIO_NUM      12
#define Y5_GPIO_NUM      10
#define Y4_GPIO_NUM      8
#define Y3_GPIO_NUM      9
#define Y2_GPIO_NUM      11
#define VSYNC_GPIO_NUM   6
#define HREF_GPIO_NUM    7
#define PCLK_GPIO_NUM    13

bool initCam(pixformat_t pixformat, framesize_t framesize);
Photo capturePhoto(const char* format);
Photo toJpeg(camera_fb_t* fb, unsigned long capture_time);
Photo toBmp(camera_fb_t* fb, unsigned long capture_time);
void captureMultiPhotos(int num_fotos, framesize_t framesize, pixformat_t pixformat, const char* extension);
const char* framesize_name(framesize_t framesize);

#endif
