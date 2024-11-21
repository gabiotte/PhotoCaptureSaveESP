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

void writeMessage(String message) {
    if(!initSDCard()) initSDCard();
    printf("\nEscrevendo mensagem...\n");
    File file = SD_MMC.open("/message.txt", FILE_WRITE);
    if (!file) {
        printf("Erro: Falha ao abrir o arquivo para escrita!\n");
        return;
    }
    file.println(message);
    file.close();
    printf("Escrita no arquivo concluída.\n");    
}

void savePhoto(String path, camera_fb_t* photo) {
    if(!initSDCard()) initSDCard();
    printf("\nSalvando foto...\n");
    if (!photo) {
      printf("Erro: Foto inválida.\n\n");
      return;
    }
    File file = SD_MMC.open(path, FILE_WRITE);
    if (!file) {
        printf("Erro: Falha ao abrir o arquivo no SD.\n");
        return;
    } 
    file.write(photo->buf, photo->len);
    file.close();
    printf("Foto salva com sucesso!\n");
    esp_camera_fb_return(photo); // Libera o buffer após salvar
}
