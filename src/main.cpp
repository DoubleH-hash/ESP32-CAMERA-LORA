#include <Arduino.h>
#include "esp_http_server.h"
#include <WiFi.h>
#include "freertos/freertos.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "camera.h"
#include "led.h"
#include "lora.h"

unsigned char readBuffer[LORA_READ_BUFFER_SIZE];

QueueHandle_t LORA_SendQueue;   //专门用来发送AT命令给LORA的队列
const int QueueElementSize = 10;

//接收LORA的数据
void Task_getSerial2(void *pvParameters)
{
    (void) pvParameters;
    unsigned char num = 0, i = 0;
    while(1){
        num = Serial2.available();   //收到串口数据
        if(num != 0 && num < LORA_READ_BUFFER_SIZE){
            while(i < num){
                readBuffer[i] = (unsigned char)Serial2.read();
                i ++;
            }

            lora_dealSerial(readBuffer, num);

            num = 0;i = 0;
        }
        delay(1000);  //每秒采集一次串口处的数据
    }
}

// 向LORA发送数据
void Task_sendSerial2(void *pvParameters)
{
    (void) pvParameters;
    int queue_ret = 0, i = 0;
    Lora_AT_send send;
    while(1){
        if(LORA_SendQueue != NULL){
            queue_ret = xQueueReceive(LORA_SendQueue, &send, portMAX_DELAY);
            if(queue_ret == pdPASS){   //收到消息队列的数据
                //输出调试信息
                Serial.printf("\r\n[Task LORA AT SEND] send:%s,len:%d,raw:", send.at_data, send.len);
                Serial.write(send.at_data, send.len);
                //串口2发送给lora
                Serial2.write(send.at_data, send.len);
            }
        }
    }
}

void setup()
{
    esp_err_t err;
    Serial.begin(115200); // 作为调试口

    Serial2.begin(115200); // 串口2与LORA连接

    err = Camera_Init();
    if(err != ESP_OK){
        Serial.printf("Camera init failed with error 0x%x", err);
    }

    xTaskCreate(Task_getSerial2, "Serial2_read", 2048, NULL, 1, NULL);
    xTaskCreate(Task_sendSerial2, "Serial2_send", 1024, NULL, 2, NULL);

    LORA_SendQueue = xQueueCreate(QueueElementSize, sizeof(Lora_AT_send));

}

void loop()
{


}
