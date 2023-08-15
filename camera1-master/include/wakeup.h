#ifndef __WAKEUP_H_
#define __WAKEUP_H_

#include <Arduino.h>
#include "freertos/freertos.h"
#include "freertos/task.h"
#include "stdio.h"
#include "common.h"

esp_sleep_wakeup_cause_t print_wakeup_reason();
void sleep_Start();


#endif
