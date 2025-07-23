#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "events.h"
#include "department.h"
#include "logging.h"

#define dispatcherPRIORITY 3
#define dispatcherDELAY ( pdMS_TO_TICKS( 500 ))

extern QueueHandle_t xQueueEvents;

void vDispatcherTask(void *pvParameters);

extern struct logCSV log_to_csv;

#endif