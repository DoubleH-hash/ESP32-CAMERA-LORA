#include <Arduino.h>
#include "esp_http_server.h"
#include <WiFi.h>
#include "freertos/freertos.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "nvs_flash.h"

#include "camera.h"
#include "led.h"
#include "wakeup.h"
#include "lora.h"
#include "common.h"

QueueHandle_t LORA_SendQueue; // 专门用来发送AT命令给LORA的队列
const int LORA_QueueSize = 10;

QueueHandle_t Camera_ControlQueue; // 专门用来控制相机拍照,传递相机拍照命令
const int Camera_QueueSize = 10;

unsigned char readBuffer[LORA_READ_BUFFER_SIZE];

// 向LORA发送数据
void Task_sendSerial1(void *pvParameters)
{
    (void)pvParameters;
    int queue_ret = 0;
    Lora_AT_send send;
    while (1)
    {
        if (LORA_SendQueue != NULL)
        {
            queue_ret = xQueueReceive(LORA_SendQueue, &send, 0);
            if (queue_ret == pdPASS){ // 收到消息队列的数据
                // 输出调试信息
                Serial.printf("\r\n[Task LORA AT SEND] send len:%d,raw:", send.len);
                Serial.write(send.at_data, send.len);
                // 串口2发送给lora
                //Serial1.write(send.at_data, send.len);
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS); //多任务延时
    }
}

//获取frame
void Task_Camera(void *pvParameters)
{
    camera_fb_t *fb;
    int camera_cmd = 0;   // 1:手动触发拍照  2：休眠唤醒后拍照
    while(1){
        if(Camera_ControlQueue != NULL){
            xQueueReceive(Camera_ControlQueue, &camera_cmd, 0); 
            if(camera_cmd != 0){
               fb = esp_camera_fb_get();

            if(fb != NULL){
                //Serial.printf("\r\n[Task_Camera2]: get a frame! size:%d,raw data:",fb->len);
                Serial.write(fb->buf, fb->len);

                esp_camera_fb_return(fb);
                fb = NULL;
            }
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS); //多任务延时
      }
    }
}

void setup()
{
    esp_err_t err;
    Serial.begin(115200); // 作为调试口
    //Serial1.begin(115200, SERIAL_8N1, GPIO_LORA_RX, GPIO_LORA_TX);

    print_wakeup_reason(); //输出唤醒原因
    led_Init();

    err = Camera_Init();
    if (err != ESP_OK){
        Serial.printf("Camera init failed with error 0x%x\r\n", err);
    }else{
      Serial.printf("Camera init OK!\r\n");
    }

    xTaskCreate(Task_Camera, "Camera_controler", 1024, NULL, 2, NULL);
    Camera_ControlQueue = xQueueCreate(Camera_QueueSize, sizeof(int));

    xTaskCreate(Task_sendSerial1, "Serial1_send", 1024, NULL, 3, NULL);
    LORA_SendQueue = xQueueCreate(LORA_QueueSize, sizeof(Lora_AT_send));

    Serial.printf("setup finish!\r\n");
}

void loop()
{
    int camera_cmd;
    Lora_AT_send send_test;

    camera_cmd = 1;
    //xQueueSend(Camera_ControlQueue, &camera_cmd, 0);

     send_test.at_data[0] = 'o';
     send_test.at_data[1] = 'k';
     send_test.len = 2;
    xQueueSend(LORA_SendQueue, &send_test, 0);

    led_Blink();

    delay(5000);
}
