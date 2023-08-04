#include <Arduino.h>
#include "esp_http_server.h"
#include <WiFi.h>
#include "freertos/freertos.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "camera.h"
#include "led.h"

// 睡眠模式为外部唤醒
// 需要三个IO，一个作为唤醒IO，一个作为拍照帧同步IO，一个作为数据发送完成IO

#define GPIO_PIN_Wakeup 33   //唤醒专用IO
#define GPIO_PIN_Camera 23   //拍照帧同步IO   下降沿触发拍照
#define GPIO_PIN_Finish 24   //数据传输完成IO   未完成：0  完成：1

int camera_cmd = 0;   // 1:触发拍照
unsigned int inter_num = 0;  //触发中断次数

esp_sleep_wakeup_cause_t print_wakeup_reason()
{
    esp_sleep_wakeup_cause_t wakeup_reason;

    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch (wakeup_reason)
    {
    case ESP_SLEEP_WAKEUP_EXT0:
        Serial.println("Wakeup caused by external signal using RTC_IO");
        break;
    case ESP_SLEEP_WAKEUP_EXT1:
        Serial.println("Wakeup caused by external signal using RTC_CNTL");
        break;
    case ESP_SLEEP_WAKEUP_TIMER:
        Serial.println("Wakeup caused by timer");
        break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
        Serial.println("Wakeup caused by touchpad");
        break;
    case ESP_SLEEP_WAKEUP_ULP:
        Serial.println("Wakeup caused by ULP program");
        break;
    default:
        Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
        break;
    }
    return wakeup_reason;
}

// 执行即进入休眠模式
void sleep_Start()
{
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 1); // 1 = High, 0 = Low
    Serial.println("Going to sleep now");
    esp_deep_sleep_start();
}

//帧同步IO中断服务函数
void ARDUINO_ISR_ATTR GPIO_Camera_interrupt() {
    camera_cmd = 1;
    inter_num ++;
}

//获取frame
void Task_Camera(void *pvParameters)
{
    camera_fb_t *fb;
    int fb_ret = 0;
    while(1){
        if(camera_cmd != 0){
            fb_ret = Camera_getFrame(fb);
            if(fb_ret == ESP_OK){
                Serial.printf("\r\n[Task_Camera2]: get a frame! size:%d,raw data:",fb->len);
                Serial.write(fb->buf, fb->len);

                camera_cmd = 0;
            }
        }
        delay(50); //多任务延时
    }
}

void setup()
{
    esp_err_t err;
    Serial.begin(115200); // 作为调试口

    print_wakeup_reason(); //输出唤醒原因

    err = Camera_Init();
    if (err != ESP_OK){
        Serial.printf("Camera init failed with error 0x%x\r\n", err);
    }

    pinMode(GPIO_PIN_Camera, INPUT_PULLUP);

    attachInterrupt(GPIO_PIN_Camera, GPIO_Camera_interrupt, FALLING);   //下降沿 触发中断 后拍照

    xTaskCreate(Task_Camera, "Camera_controler", 1024, NULL, 1, NULL);

    Serial.printf("setup finish!\r\n");
}

void loop()
{
}
