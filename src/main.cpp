#include <Arduino.h>
#include <SDCard.h>
#include <camera.h>

#define LED_PIN 3

int num_photos = 10;


void setup() {
  Serial.begin(115200);
  delay(6000);
  printf("Programa iniciado.\n");  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  initCam(pixformat, framesize);
  initSDCard();
  delay((1000));  
  capture(num_photos, framesize, pixformat, FORMATO_ARQUIVO_JPEG);
  
}

void loop() {
}