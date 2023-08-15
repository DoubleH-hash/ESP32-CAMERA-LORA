#include "camera.h"
#include <Arduino.h>
#include "esp_camera.h"
#include "fb_gfx.h"

//初始化相机
esp_err_t Camera_Init()
{
    esp_err_t err;
    camera_config_t config;

    config.pin_d0 = D0_GPIO_NUM;
    config.pin_d1 = D1_GPIO_NUM;
    config.pin_d2 = D2_GPIO_NUM;
    config.pin_d3 = D3_GPIO_NUM;
    config.pin_d4 = D4_GPIO_NUM;
    config.pin_d5 = D5_GPIO_NUM;
    config.pin_d6 = D6_GPIO_NUM;
    config.pin_d7 = D7_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = -1;
    config.pin_reset = RESET_GPIO_NUM;
    
    config.xclk_freq_hz = 20000000;
    config.ledc_timer = LEDC_TIMER_0;
    config.ledc_channel = LEDC_CHANNEL_0;

    config.frame_size = FRAMESIZE_SVGA;
    config.pixel_format = PIXFORMAT_JPEG;
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 10;
    config.fb_count = 1;

    err = esp_camera_init(&config);
    if (err != ESP_OK)
    {
        return err;
    }

    //从相机中获取相机参数对应的指针
    sensor_t *s = esp_camera_sensor_get();
    //修改相机参数寄存器中的值
    s->set_vflip(s, 1);

    
    return err;
}