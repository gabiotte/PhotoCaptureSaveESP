#pragma once

#ifndef __ARTIGO_CAMERAS_H__
#define __ARTIGO_CAMERAS_H__

#define LED_PIN 3
#define NUM_LEDS 12
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

bool deviceSetup();
void exit();
void flash(String modo);

#endif
