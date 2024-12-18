#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "department.h"
#include "logging.h"

#define dispatcherPRIORITY 3
#define dispatcherDELAY ( pdMS_TO_TICKS( 1000 ))

extern QueueHandle_t xQueueEvents;
extern const int DEPARTMENTS_AMOUNT;

void vDispatcherTask(void *pvParameters);

#endif