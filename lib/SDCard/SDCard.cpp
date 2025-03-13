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
    printf("Erro: Falha ao inicializar o cartão SD.\n");
    return false;
  }
  printf("SD card pronto!\n");
  return true;
}

void savePhoto(String path, uint8_t* photo_buffer, size_t photo_len) {
    printf("\nSalvando foto...\n");
    printf("path: %s\n",path);
    if (!photo_buffer || photo_len == 0) {
      printf("Erro: Foto inválida.\n\n");
      return;
    }

    File file = SD_MMC.open(path, FILE_WRITE);
    if (!file) {
        printf("Erro: Falha ao abrir o arquivo %s no SD.\n", path);
        return;
    } 

    file.write(photo_buffer, photo_len);
    file.close();

    printf("Foto salva com sucesso em %s.\n", path);
    free(photo_buffer);
}

// void create_dir(const char* path) {
//   struct stat st;
//   if (stat(path, &st) != 0) {
//     mkdir(path, 0777);
//   } if (stat(path, &st) != 0) {
//     printf("Diretório %s não foi criado.\n", path);
//     return;
//   } printf("Diretório %s criado.\n");
  
// }

void create_dir(const char* path) {
  if (!SD_MMC.exists(path)) {
    SD_MMC.mkdir(path);
    if (SD_MMC.exists(path)) {
      printf("Pasta %s criada.\n\n", path);
      return;
    }
    printf("ERRO: Pasta %s não foi criada.\n\n", path);
  }
}