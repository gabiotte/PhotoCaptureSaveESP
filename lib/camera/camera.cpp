#include <Arduino.h>
#include <camera.h>
#include "esp_camera.h"

bool initCam() {
  printf("\nInicializando a câmera...\n");
  printf("Heap livre antes da inicialização: %d bytes\n", esp_get_free_heap_size());

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
  config.pixel_format = PIXFORMAT_RGB565;
  config.frame_size = FRAMESIZE_VGA;  
  config.jpeg_quality = 0;            
  config.fb_count = 1;                
  config.fb_location = CAMERA_FB_IN_PSRAM;


  if (esp_camera_init(&config) != ESP_OK) {
    printf("Erro: Falha ao inicializar a câmera.\n");
    return false;
  }
  printf("Câmera inicializada com sucesso!\n");
  printf("Heap livre depois da inicialização: %d bytes\n", esp_get_free_heap_size());
  return true;
}

camera_fb_t* capturePhoto() {
    if(!initCam()) initCam();
    printf("\nCapturando foto...\n");
    printf("Tamanho mínimo da pilha do cam_task: %d bytes\n", uxTaskGetStackHighWaterMark(NULL));
    camera_fb_t* fb = esp_camera_fb_get();
    printf("Cheguei aqui!\n");
    if (!fb) {
      printf("Erro: Falha ao capturar a foto.\n");
      return nullptr;
    }
    printf("Foto capturada com sucesso! Tamanho: %d bytes\n", fb->len);
    return fb;
}