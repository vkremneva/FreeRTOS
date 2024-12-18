#ifndef EVENTS_H
#define EVENTS_H

#include <time.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "logging.h"

// TODO not so sure about 10
#define eventsQUEUE_SIZE 10
#define eventsQUEUE_ITEM_SIZE (sizeof(int16_t)) 

#define eventsPRIORITY 3

#define eventsMIN_WAIT_TIME 500
#define eventMAX_WAIT_TIME 3000

#define eventsMAX_EVENT_CODE 3
#define eventsMIN_EVENT_CODE 1

void vEventGeneratorTask(void *pvParameters);

#endif
