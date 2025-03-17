#include <Arduino.h>
#include <artigoCameras.h>
#include <camera.h>
#include <SDCard.h>

bool deviceSetup() {
    printf("\nPrograma iniciado.\n"); 

    //esp_log_level_set("*", ESP_LOG_NONE);  
    
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    
    if (initSDCard() == false || initCam(pixformat, framesize) == false) {
        return false;
    }
    
    delay((1000));
    return true;
}
