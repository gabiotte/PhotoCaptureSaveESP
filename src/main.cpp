#include <Arduino.h>
#include <SDCard.h>
#include <camera.h>

void setup() {
  Serial.begin(115200);
  delay(8000);
  printf("Programa iniciado.\n");  

  savePhoto("/fotoCapturada.jpg", capturePhoto());
}

void loop() {
}