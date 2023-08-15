#ifndef _COMMON_H_
#define _COMMON_H_

#include "Arduino.h"


//板载Camera接口
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

//与slave板连接需要   1：唤醒IO(master唤醒后，就要唤醒slave) 2：帧同步IO, 3.数据传输完成状态IO
#define GPIO_PIN_Camera 23   //拍照帧同步IO   下降沿触发拍照
#define GPIO_PIN_Finish 24   //数据传输完成IO   未完成：0  完成：1


//与LORA连接的serial1 使用的TX RX
#define GPIO_LORA_RX 3
#define GPIO_LORA_TX 4

//板载LED
#define LED_GPIO_NUM 25

//唤醒专用IO
#define GPIO_PIN_Wakeup GPIO_NUM_33

#endif
