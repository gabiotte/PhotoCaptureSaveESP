#pragma once

#ifndef __SD_CARD_H__
#define __SD_CARD_H__

#define CLK GPIO_NUM_39
#define CMD GPIO_NUM_38
#define D0 GPIO_NUM_40

bool initSDCard();
void savePhoto(String path, uint8_t* photo_buffer, size_t photo_len);
void create_dir(const char* path);

#endif
