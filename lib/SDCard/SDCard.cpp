#include <Arduino.h>
#include "SDCard.h"
#include "driver/sdmmc_host.h"
#include "SD_MMC.h"
#include "esp_camera.h"

bool init_sdcard() {
  printf("\nInicializando o cartão SD...\n");

  pinMode(CLK, INPUT_PULLUP); 
  pinMode(CMD, INPUT_PULLUP); 
  pinMode(D0, INPUT_PULLUP); 

  SD_MMC.setPins(CLK, CMD, D0);

  if (!SD_MMC.begin("/sdcard", true)) { 
    printf("Erro: Falha ao inicializar o cartão SD.\n");
    return false;
  }
  printf("SD card pronto.\n");
  return true;
}

void save_photo(String path, uint8_t* photo_buffer, size_t photo_len) {
    printf("\nSalvando foto...\n");

    if (!photo_buffer || photo_len == 0) {
      printf("Erro: Foto inválida.\n\n");
      return;
    }

    File file = SD_MMC.open(path, FILE_WRITE);
    if (!file) {
        printf("Erro: Falha ao abrir o arquivo %s no cartão SD.\n", path);
        return;
    } 

    file.write(photo_buffer, photo_len);
    file.close();

    printf("Foto salva com sucesso.\n");
    free(photo_buffer);
}

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

void create_csv(const char* path, const char* header) {
  printf("\nCriando aqruivo CSV em '%s'...\n", path);
  File csvFile = SD_MMC.open(path, FILE_WRITE);
  if (csvFile) {
    printf("Arquivo criado com sucesso.\n");
    csvFile.println(header);
    csvFile.close();
  } else {
    printf("Erro ao criar o arquivo CSV!\n");
  }
}

void save_time(const char* csv_path, const char* name, unsigned long time) {
  File file = SD_MMC.open(csv_path, FILE_APPEND);
  if (!file) {
    printf("Erro: Não foi possível abrir o CSV em %s\n", csv_path);
    return;
  }
  file.printf("%s,%lu\n", name, time);
  file.close();
}