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

QueueHandle_t LORA_SendQueue; // 专门用来发送AT命令给LORA的队列
const int LORA_QueueSize = 10;

QueueHandle_t Camera_ControlQueue; // 专门用来控制相机拍照,传递相机拍照命令
const int Camera_QueueSize = 10;

//与slave需要   1：唤醒IO(master唤醒后，就要唤醒slave) 2：帧同步IO, 3.数据传输完成状态IO

// 接收LORA的数据
void Task_getSerial2(void *pvParameters)
{
    (void)pvParameters;
    unsigned char num = 0, lastnum = 0, i = 0;
    while (1)
    {
        num = Serial2.available();

        if (num != 0 && lastnum != num){  // 收到串口数据
            lastnum = num;
        }
        else if (num != 0 && lastnum == num){   //收到的串口数据不变，说明接收完成
            while (i < num){
                readBuffer[i] = (unsigned char)Serial2.read();
                i++;
            }
            lora_dealSerial(readBuffer, num);

            num = 0;lastnum = 0;
            i = 0;
        }
        delay(100); // 每100ms采集一次串口处的数据
    }
}

// 向LORA发送数据
void Task_sendSerial2(void *pvParameters)
{
    (void)pvParameters;
    int queue_ret = 0, i = 0;
    Lora_AT_send send;
    while (1)
    {
        if (LORA_SendQueue != NULL)
        {
            queue_ret = xQueueReceive(LORA_SendQueue, &send, portMAX_DELAY);
            if (queue_ret == pdPASS)
            { // 收到消息队列的数据
                // 输出调试信息
                Serial.printf("\r\n[Task LORA AT SEND] send:%s,len:%d,raw:", send.at_data, send.len);
                Serial.write(send.at_data, send.len);
                // 串口2发送给lora
                Serial2.write(send.at_data, send.len);
            }
        }
        delay(100); //多任务延时
    }
}

void Task_Camera(void *pvParameters)
{
    camera_fb_t *fb;
    int queue_ret = 0;
    int camera_cmd = 0;   // 1:手动触发拍照  2：休眠唤醒后拍照
    int fb_ret = 0;
    while(1){
        if(Camera_ControlQueue != NULL){
            queue_ret = xQueueReceive(LORA_SendQueue, &camera_cmd, portMAX_DELAY); 
            if(camera_cmd != 0){
               fb_ret = Camera_getFrame(fb);
               if(fb_ret == ESP_OK){
                    Serial.printf("\r\n[Task_Camera]: get a frame! size:%d,raw data:",fb->len);
                    Serial.write(fb->buf, fb->len);
               }
            }
        }
        delay(50); //多任务延时
    }
}

void setup()
{
    esp_err_t err;
    Serial.begin(115200); // 作为调试口

    Serial2.begin(115200); // 串口2与LORA连接

    err = Camera_Init();
    if (err != ESP_OK)
    {
        Serial.printf("Camera init failed with error 0x%x", err);
    }

    xTaskCreate(Task_getSerial2, "Serial2_read", 1024, NULL, 1, NULL);
    xTaskCreate(Task_sendSerial2, "Serial2_send", 1024, NULL, 3, NULL);
    xTaskCreate(Task_Camera, "Camera_controler", 1024, NULL, 2, NULL);

    LORA_SendQueue = xQueueCreate(LORA_QueueSize, sizeof(Lora_AT_send));

    Camera_ControlQueue = xQueueCreate(Camera_QueueSize, sizeof(int));

}

void loop()
{

    
}
