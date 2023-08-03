#ifndef __LED_H_
#define __LED_H_

#ifdef __cplusplus  //按照类C的编译和连接规约来编译和连接，而不是C++的编译的连接规约。
extern "C" {
#endif


#define LED_GPIO_NUM 25

#include <Arduino.h>



void led_Init();
void led_Blink();
void led_setValue(uint8_t val);



#ifdef __cplusplus
} /*extern "C"*/
#endif


#endif
