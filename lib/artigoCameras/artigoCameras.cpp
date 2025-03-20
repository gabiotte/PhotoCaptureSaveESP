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
    FastLED.setBrightness(50);
    fill_solid(leds, NUM_LEDS, CRGB::White);
    FastLED.show();
    
    if (initSDCard() == false || initCam(pixformat, framesize) == false) {
        
        return false;
    }
    
    delay((1000));
    return true;
}
