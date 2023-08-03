#ifndef __LORA_H_
#define __LORA_H_

#ifdef __cplusplus  //按照类C的编译和连接规约来编译和连接，而不是C++的编译的连接规约。
extern "C" {
#endif

#include <Arduino.h>
#include "freertos/freertos.h"
#include "freertos/task.h"
#include "stdio.h"

#define LORA_READ_BUFFER_SIZE 200
#define LORA_SEND_BUFFER_SIZE 50   //一次发送最大长度  发送AT指令

enum {
    LORA_ERR,

    LORA_OK,
};

typedef struct{
    unsigned char len;
    unsigned char at_data[LORA_SEND_BUFFER_SIZE];
}Lora_AT_send;

//处理从串口接收到的数据
int lora_dealSerial(unsigned char *data, unsigned char len);
void lora_sendAT(unsigned char *data, unsigned char len);
int lora_Init();


#ifdef __cplusplus
} /*extern "C"*/
#endif


#endif
