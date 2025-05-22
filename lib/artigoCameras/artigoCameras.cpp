#include <Arduino.h>
#include <artigoCameras.h>
#include <camera.h>
#include <SDCard.h>
#include <FastLED.h>

CRGB leds[NUM_LEDS];

bool deviceSetup() {
    printf("\nPrograma iniciado.\n"); 

    //esp_log_level_set("*", ESP_LOG_NONE);  
    
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(25);
    printf("\nRing LED inicializado com sucesso.\n");
    
    if (initSDCard() == false || initCam(pixformat, framesize) == false) {
        
        return false;
    }
    return true;
}

void exit() {
    printf("\nPrograma finalizado.\n");
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    digitalWrite(LED_PIN, HIGH);
}

void flash(String modo) {
    if (modo == "on") {
        fill_solid(leds, NUM_LEDS, CRGB::White);
        FastLED.show();
    } else if (modo == "off") {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show();
    }
}