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

#define dispPRIORITY    3
#define dispDELAY       ( pdMS_TO_TICKS( 1000 ))

extern QueueHandle_t xQueueEvents;

void vDispatcherTask( void * pvParameters );

#endif /* ifndef DISPATCHER_H */
