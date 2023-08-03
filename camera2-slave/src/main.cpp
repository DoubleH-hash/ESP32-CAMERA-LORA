#include <Arduino.h>
#include "esp_http_server.h"
#include <WiFi.h>
#include "freertos/freertos.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "camera.h"
#include "led.h"


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
