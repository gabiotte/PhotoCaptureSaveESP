#include <Arduino.h>
#include <camera.h>
#include "esp_camera.h"



bool initCam(pixformat_t pixformat, framesize_t framesize) {
  printf("\nInicializando a câmera...\n");

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
  config.pixel_format = pixformat;
  config.frame_size = framesize;  
  config.jpeg_quality = 15;            
  config.fb_count = 1;                
  config.fb_location = CAMERA_FB_IN_PSRAM;

  if (esp_camera_init(&config) != ESP_OK) {
    printf("Erro: Falha ao inicializar a câmera.\n");
    return false;
  }
  printf("Câmera inicializada com sucesso!\n");
  return true;
}

Photo capturePhoto(const char* format) {
    // if(!initCam()) initCam();
    printf("\nCapturando foto...\n");
    camera_fb_t* fb = esp_camera_fb_get();
    
    if (fb==NULL) {
      printf("Erro: Falha ao capturar a foto.\n");
      return {nullptr,0};
    }
    printf("Foto capturada com sucesso! Tamanho: %d bytes\n", fb->len);

    if (strcmp(format, "original") == 0) {
        uint8_t* buffer = (uint8_t*)malloc(fb->len);
        if (buffer == NULL) {
            printf("Erro: Falha ao alocar memória para a foto original.\n");
            esp_camera_fb_return(fb);
            return {nullptr, 0};
        }
        memcpy(buffer, fb->buf, fb->len);
        size_t length = fb->len;
        esp_camera_fb_return(fb);
        printf("Foto original retornada.\n");
        return {buffer, length};
    } 
    else if (strcmp(format, "jpeg") == 0) {
        printf("\nConvertendo para JPEG...\n");
        Photo jpegPhoto = toJpeg(fb);
        if (jpegPhoto.buffer == nullptr) {
            printf("Erro: Falha ao converter para JPEG.\n");
        } else {
            printf("Foto convertida para JPEG com sucesso.\n");
        }
        return jpegPhoto;
    } 
    else if (strcmp(format, "bmp") == 0) {
        printf("\nConvertendo para BMP...\n");
        Photo bmpPhoto = toBmp(fb);
        if (bmpPhoto.buffer == nullptr) {
            printf("Erro: Falha ao converter para BMP.\n");
        } else {
            printf("Foto convertida para BMP com sucesso.\n");
        }
        return bmpPhoto;
    } 
    else {
        printf("Erro: Formato desconhecido '%s'.\n", format);
        esp_camera_fb_return(fb);
        return {nullptr, 0};
    }
    
    }

Photo toJpeg(camera_fb_t* fb) {
  printf("Iniciando conversão...\n");
      uint8_t* jpeg_buffer = nullptr;
      size_t jpeg_len = 0;
      bool converted = fmt2jpg(fb->buf, fb->len, fb->width, fb->height, PIXFORMAT_RGB565, 90, &jpeg_buffer, &jpeg_len);
      esp_camera_fb_return(fb);

      if (jpeg_buffer==NULL) {
        printf("Erro: Falha ao converter a foto para JPEG.\n");
        return {nullptr,0};
      } 
      printf("Conversão finalizada.\n");
      return {jpeg_buffer,jpeg_len};
}

Photo toBmp(camera_fb_t* fb) {
  printf("Iniciando conversão para BMP...\n");

  uint8_t* bmp_buffer = nullptr;
  size_t bmp_len = 0;

  bool converted = fmt2bmp(fb->buf, fb->len, fb->width, fb->height, PIXFORMAT_RGB565, &bmp_buffer, &bmp_len);

  // Retornar o framebuffer original
  esp_camera_fb_return(fb);

  // Verificar se a conversão foi bem-sucedida
  if (!converted || bmp_buffer == nullptr) {
    printf("Erro: Falha ao converter a imagem para BMP.\n");
    return {nullptr, 0};
  }

  printf("Conversão finalizada.\n");
  return {bmp_buffer, bmp_len};
}
