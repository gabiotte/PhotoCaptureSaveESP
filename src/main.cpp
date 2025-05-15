#include <Arduino.h>
#include <artigoCameras.h>
#include <camera.h>

int num_photos = 10;

void setup() {
  Serial.begin(115200);
  delay(6000);
  
  if (deviceSetup() == true) {
    delay(2500);
    captureMultiPhotos(num_photos, framesize, pixformat, FORMATO_ARQUIVO_JPEG);
  }
  
  exit();
}

void loop() {
}