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

    config.frame_size = FRAMESIZE_VGA;
    config.pixel_format = PIXFORMAT_JPEG;
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 12;
    config.fb_count = 1;

    err = esp_camera_init(&config);
    if (err != ESP_OK)
    {
        //Serial.printf("Camera init failed with error 0x%x", err);
        return err;
    }

    //从相机中获取相机参数对应的指针
    sensor_t *s = esp_camera_sensor_get();
    //修改相机参数寄存器中的值
    s->set_vflip(s, 1);

    return err;
}

//获取图片
esp_err_t Camera_getFrame(camera_fb_t *fb)
{
    esp_err_t ret = ESP_OK;

    fb = esp_camera_fb_get();

    if(fb == NULL){
        ret = ESP_FAIL;
    }
    return ret;
}

//清除framebuffer
void Camera_clearFB(camera_fb_t *fb)
{
    esp_camera_fb_return(fb);
}
