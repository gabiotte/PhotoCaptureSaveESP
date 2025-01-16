#include <Arduino.h>
#include "SDCard.h"
#include "driver/sdmmc_host.h"
#include "SD_MMC.h"
#include "esp_camera.h"

bool initSDCard() {
  printf("\nIniciando o cartão SD...\n");

  pinMode(CLK, INPUT_PULLUP); 
  pinMode(CMD, INPUT_PULLUP); 
  pinMode(D0, INPUT_PULLUP); 

  SD_MMC.setPins(CLK, CMD, D0);

  if (!SD_MMC.begin("/sdcard", true)) { 
    printf("Erro: Falha ao inicializar o cartão SD!\n");
    return false;
  }
  printf("SD card pronto!\n");
  return true;
}

void savePhoto(String path, uint8_t* photo_buffer, size_t photo_len) {
    printf("\nSalvando foto...\n");
    if (!photo_buffer || photo_len == 0) {
      printf("Erro: Foto inválida.\n\n");
      return;
    }

    File file = SD_MMC.open(path, FILE_WRITE);
    if (!file) {
        printf("Erro: Falha ao abrir o arquivo no SD.\n");
        return;
    } 

    file.write(photo_buffer, photo_len);
    file.close();

    printf("Foto salva com sucesso!\n");
    free(photo_buffer);
}
