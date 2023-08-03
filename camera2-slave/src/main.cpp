#include <Arduino.h>
#include "esp_http_server.h"
#include <WiFi.h>
#include "freertos/freertos.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "camera.h"
#include "led.h"

//需要两个IO，一个作为唤醒IO，一个作为拍照帧同步IO
//睡眠模式为外部唤醒

void setup()
{
    esp_err_t err;
    Serial.begin(115200); // 作为调试口

    err = Camera_Init();
    if (err != ESP_OK)
    {
        Serial.printf("Camera init failed with error 0x%x", err);
    }


}

void loop()
{
}
