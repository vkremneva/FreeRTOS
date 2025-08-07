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

#define dispPRIORITY  3
#define dispDELAY     ( pdMS_TO_TICKS( 1000 ) )

extern QueueHandle_t xQueueEvents;

/* For debug puposes only. */
extern struct logCSV log_to_csv;

void vDispatcherTask(void *pvParameters);

#endif