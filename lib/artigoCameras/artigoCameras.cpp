#include <Arduino.h>
#include <artigoCameras.h>
#include <camera.h>
#include <SDCard.h>

bool deviceSetup() {
    printf("\nPrograma iniciado.\n"); 

    //esp_log_level_set("*", ESP_LOG_NONE);  
    
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    
    if (initCam(pixformat, framesize) == false || initSDCard() == false) {
        return false;
    }
    
    delay((1000));
    return true;
}
