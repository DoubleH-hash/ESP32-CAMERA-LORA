#ifndef __LED_H_
#define __LED_H_



#define LED_GPIO_NUM 25

#include <Arduino.h>



void led_Init();
void led_Blink();
void led_setValue(uint8_t val);




#endif
