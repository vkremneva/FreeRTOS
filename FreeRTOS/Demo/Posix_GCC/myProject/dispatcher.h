#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#define dispatcherPRIORITY 3
#define dispatcherDELAY ( pdMS_TO_TICKS( 1000 ))

extern QueueHandle_t xPoliceQueue;

void vDispatcherTask(void *param);

#endif