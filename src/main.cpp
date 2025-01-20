#include <Arduino.h>
#include <artigoCameras.h>
#include <camera.h>

int num_photos = 15;

void setup() {
  Serial.begin(115200);
  delay(6000);
  if (deviceSetup() == true) {
    captureMultiPhotos(num_photos, framesize, pixformat, FORMATO_ARQUIVO_JPEG);
  }
  printf("\nPrograma finalizado.\n");
}


void loop() {
}