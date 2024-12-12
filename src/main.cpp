#include <Arduino.h>
#include <SDCard.h>
#include <camera.h>

unsigned int count = 0;

pixformat_t pixformat = PIXFORMAT_RGB565;
framesize_t framesize = FRAMESIZE_HQVGA;
const char* formato_arquivo = "bmp"; // jpeg, bmp, original

void setup() {
  Serial.begin(115200);
  delay(6000);
  printf("Programa iniciado.\n");  
  initCam(pixformat, framesize);
  initSDCard();
  delay((1000));
}

void loop() {
  if(count < 5) {
    printf("\nCapturando e salvando foto %d -----------------------------\n", count+1);

  const char* extension;
    if (strcmp(formato_arquivo, "jpeg") == 0) {
      extension = "jpg";
    } else if (strcmp(formato_arquivo, "bmp") == 0) {
      extension = "bmp";
    } else if (strcmp(formato_arquivo, "original") == 0) {
      if (pixformat == PIXFORMAT_RGB565) {
        extension = "rgb565";
      } else if (pixformat == PIXFORMAT_JPEG) {
        extension = "jpg"; // Mesmo sendo original, JPEG já é um formato final
      } else {
        extension = "bin"; // Caso não reconhecido
      }
    } else {
      printf("Erro: Formato de arquivo desconhecido '%s'.\n", formato_arquivo);
      delay(7000);
      return;
    }

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
    

    char path[64];
    snprintf(path, sizeof(path), "/%s_%d.%s", framesize_name, count + 1, extension);

    Photo photo = capturePhoto(formato_arquivo);
    savePhoto(path, photo.buffer, photo.len);
    count++;
  } else {
    printf("\nFim...\n");
    delay(60000);
  }
  
}