#include "lora.h"
#include <Arduino.h>
#include "freertos/freertos.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "soc/soc_caps.h"
#include "driver/uart.h"



//extern QueueHandle_t LORA_SendQueue;
char *strx;


// 清空缓存
void Clear_Buffer(unsigned char *data, unsigned char len)
{
    unsigned char i;
    for (i = 0; i < len; i++)
        data[i] = 0;
}

// 处理从串口接收到的数据
int lora_dealSerial(unsigned char *data, unsigned char len)
{
    unsigned char ret = LORA_RX_EMPTY;

    strx = strstr((const char*)data,(const char*)"OK");//返回OK
    if(strx != NULL){ret = LORA_RX_OK;}

    Clear_Buffer(data, len);
    return ret;
}

void lora_sendAT(unsigned char *data, unsigned char len)
{
    int ret = 0, i = 0;
    Lora_AT_send *send;

    send->len = len;
    for (; i < len; i++)
    {
        send->at_data[i] = data[i];
    }
    //ret = xQueueSend(LORA_SendQueue, send, 0);
}

int lora_Init()
{
    int ret = 0;

    return ret;
}
