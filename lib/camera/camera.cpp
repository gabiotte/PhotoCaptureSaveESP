#include <Arduino.h>
#include <camera.h>
#include "esp_camera.h"
#include <SDCard.h>
#include <artigoCameras.h>


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
  config.jpeg_quality = 8;            
  config.fb_count = 1;                
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.grab_mode = CAMERA_GRAB_LATEST;
  unsigned long start = micros();
  if (esp_camera_init(&config) != ESP_OK) {
    printf("Erro: Falha ao inicializar a câmera.\n");
    return false;
  }
  init_time = micros() - start;
  printf("Câmera %s inicializada com sucesso! Tempo: %lu µs\n", camera, init_time);
  return true;
}

Photo capturePhoto(const char* format) {

  printf("\nCapturando foto...\n");
  flash("on");  
  delay(250);

  unsigned long start = micros();  
  camera_fb_t* fb = esp_camera_fb_get();
  unsigned long capture_time = micros() - start;
  
  delay(250);
  flash("off");
  
  if (fb==NULL) {
    printf("Erro: Falha ao capturar a foto.\n");
    return {nullptr,0,0};
  }
  printf("Foto capturada com sucesso! Tamanho: %d bytes; Tempo: %lu µs\n", fb->len, capture_time);

  // Deixa o arquivo no formato desejado, realizando ou não a conversão:
  if (fb->format == PIXFORMAT_RGB565) {

    if (strcmp(format, "rgb565") == 0) {
      uint8_t* buffer = (uint8_t*)malloc(fb->len);
      if (buffer == NULL) {
        printf("Erro: Falha ao alocar memória para a foto RGB565.\n");
        esp_camera_fb_return(fb);
        return {nullptr, 0, 0};
      }
      memcpy(buffer, fb->buf, fb->len);
      size_t length = fb->len;
      esp_camera_fb_return(fb);
      printf("Foto original em RGB565 retornada.\n");
      return {buffer, length, capture_time};

    } else if (strcmp(format, "jpeg") == 0) {
      Photo jpegPhoto = toJpeg(fb, capture_time);
      esp_camera_fb_return(fb);
      return jpegPhoto;

    } else if (strcmp(format, "bmp") == 0) {
      Photo bmpPhoto = toBmp(fb, capture_time);
      esp_camera_fb_return(fb);
      return bmpPhoto;
    } 

  } else if (fb->format == PIXFORMAT_JPEG) {
    uint8_t* buffer = (uint8_t*)malloc(fb->len);
    if (buffer == NULL) {
      printf("Erro: Falha ao alocar memória para a foto JPEG.\n");
      esp_camera_fb_return(fb);
      return {nullptr, 0, 0};
    }
    memcpy(buffer, fb->buf, fb->len);
    size_t length = fb->len;
    esp_camera_fb_return(fb);
    printf("Foto original em JPEG retornada.\n");
    return {buffer, length, capture_time};

  } else {
    printf("Erro: Formato desconhecido.\n");
    esp_camera_fb_return(fb);
    return {nullptr, 0, 0};
    }
   
}

Photo toJpeg(camera_fb_t* fb, unsigned long capture_time) {
  printf("Iniciando conversão para JPEG...\n");
  uint8_t* jpeg_buffer = nullptr;
  size_t jpeg_len = 0;
  bool converted = fmt2jpg(fb->buf, fb->len, fb->width, fb->height, fb->format, 80, &jpeg_buffer, &jpeg_len);
  esp_camera_fb_return(fb);

  if (jpeg_buffer==NULL) {
    printf("Erro: Falha ao converter a foto para JPEG.\n");
    return {nullptr,0};
  } 
  printf("Conversão finalizada. Tamanho: %d\n", jpeg_len);
  return {jpeg_buffer,jpeg_len, capture_time};
}

Photo toBmp(camera_fb_t* fb, unsigned long capture_time ) {
  printf("Iniciando conversão para BMP...\n");

  uint8_t* bmp_buffer = nullptr;
  size_t bmp_len = 0;

  bool converted = fmt2bmp(fb->buf, fb->len, fb->width, fb->height, fb->format, &bmp_buffer, &bmp_len);

  esp_camera_fb_return(fb);

  if (!converted || bmp_buffer == nullptr) {
    printf("Erro: Falha ao converter a imagem para BMP.\n");
    return {nullptr, 0, capture_time};
  }

  printf("Conversão finalizada.\n");
  return {bmp_buffer, bmp_len};
}

void captureMultiPhotos(int num_fotos, framesize_t framesize, pixformat_t pixformat, const char* extension) {

  char cameraDir[64];
  snprintf(cameraDir, sizeof(cameraDir), "/%s", camera);
  create_dir(cameraDir);

  char csvPath[256];
  snprintf(csvPath, sizeof(csvPath), "%s/tempo.csv", cameraDir);

  create_csv(csvPath, "Imagem, Tempo (µs)");
  save_time(csvPath, "camera", init_time);

  unsigned long discard_start = micros();  
  camera_fb_t* fb = esp_camera_fb_get();
  unsigned long discard_capture_time = micros() - discard_start;

  if (fb != nullptr ){
    printf("\nPrimeira foto descartada com sucesso. Tempo: %lu µs\n", discard_capture_time);
  } else {
    printf("\nNão foi possível capturar foto de descarte. \n");
  }
  esp_camera_fb_return(fb);

  delay(80); // +/- tempo necessário para gerar um novo frame
  for (int count = 0; count <= num_fotos -1; count++) {

    printf("\nCapturando e salvando foto %d -----------------------------\n", count);

    char path[256];
    
    char photo_name[64];
    snprintf(photo_name, sizeof(photo_name), "%d.%s", count, extension);

    //snprintf(path, sizeof(path), "/%s_%d.%s", framesize_name(framesize), count, extension);
    snprintf(path, sizeof(path), "/%s/%s", cameraDir, photo_name);
    
    Photo photo = capturePhoto(extension);
    if (photo.buffer != nullptr && photo.len > 0) {
      savePhoto(path, photo.buffer, photo.len);
      save_time(csvPath, photo_name, photo.capture_time);
    }

  }
  
  printf("\nFim.\n");

} 

const char* framesize_name(framesize_t framesize) {
  const char* framesize_name;
    switch (framesize) {
      case FRAMESIZE_96X96:    framesize_name = "96x96"; break;
      case FRAMESIZE_QQVGA:    framesize_name = "QQVGA"; break;
      case FRAMESIZE_QCIF:     framesize_name = "QCIF"; break;
      case FRAMESIZE_HQVGA:    framesize_name = "HQVGA"; break;
      case FRAMESIZE_240X240:  framesize_name = "240x240"; break;
      case FRAMESIZE_QVGA:     framesize_name = "QVGA"; break;
      case FRAMESIZE_CIF:      framesize_name = "CIF"; break;
      case FRAMESIZE_HVGA:     framesize_name = "HVGA"; break;
      case FRAMESIZE_VGA:      framesize_name = "VGA"; break;
      case FRAMESIZE_SVGA:     framesize_name = "SVGA"; break;
      case FRAMESIZE_XGA:      framesize_name = "XGA"; break;
      case FRAMESIZE_HD:       framesize_name = "HD"; break;
      case FRAMESIZE_SXGA:     framesize_name = "SXGA"; break;
      case FRAMESIZE_UXGA:     framesize_name = "UXGA"; break;
      default:                 framesize_name = "UNKNOWN"; break;
    }
  return framesize_name;
}
