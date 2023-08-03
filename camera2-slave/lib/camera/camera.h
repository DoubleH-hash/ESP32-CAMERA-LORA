#ifndef __CAMERA_H_
#define __CAMERA_H_

#ifdef __cplusplus  //按照类C的编译和连接规约来编译和连接，而不是C++的编译的连接规约。
extern "C" {
#endif

#define RESET_GPIO_NUM   5
#define XCLK_GPIO_NUM    15
#define SIOD_GPIO_NUM    22
#define SIOC_GPIO_NUM    23

#define D0_GPIO_NUM      2
#define D1_GPIO_NUM      14
#define D2_GPIO_NUM      35
#define D3_GPIO_NUM      12
#define D4_GPIO_NUM      27
#define D5_GPIO_NUM      33
#define D6_GPIO_NUM      34
#define D7_GPIO_NUM      39
#define VSYNC_GPIO_NUM   18
#define HREF_GPIO_NUM    36
#define PCLK_GPIO_NUM    26



#include <Arduino.h>
#include "esp_camera.h"
#include "fb_gfx.h"


esp_err_t Camera_Init();
esp_err_t Camera_getFrame(camera_fb_t *fb);
void Camera_clearFB(camera_fb_t *fb);



#ifdef __cplusplus
} /*extern "C"*/
#endif


#endif
