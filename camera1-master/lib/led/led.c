#include "led.h"

void led_Init()
{
    pinMode(LED_GPIO_NUM, OUTPUT);
    digitalWrite(LED_GPIO_NUM, LOW);
}

void led_Blink()
{
    if(digitalRead(LED_GPIO_NUM) == HIGH){
        digitalWrite(LED_GPIO_NUM, LOW);
    }else{
        digitalWrite(LED_GPIO_NUM, HIGH);
    }
}

void led_setValue(uint8_t val)
{
    digitalWrite(LED_GPIO_NUM, val);
}