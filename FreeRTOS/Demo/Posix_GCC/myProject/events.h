#ifndef EVENTS_H
#define EVENTS_H

#include <time.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "logging.h"

#define eventsQUEUE_SIZE        10
#define eventsQUEUE_ITEM_SIZE   (sizeof(event_t)) 

#define eventsPRIORITY          5

#define eventsMIN_WAIT_TIME     200
#define eventMAX_WAIT_TIME      800

#define eventsMAX_EVENT_CODE    3
#define eventsMIN_EVENT_CODE    1

#define eventCODE           1
#define eventFREE_RESOURCE  2

#define eventCOUNTER_REJECTED_MAX 10

typedef struct {
    uint8_t     ucCode;
    bool        xRejected;
    BaseType_t  uxType;
    BaseType_t  uxCounterRejected;
} event_t;

extern QueueHandle_t xQueueEvents;

/* For debug puposes only. */
extern struct logCSV log_to_csv;

void vEventGeneratorTask(void *pvParameters);

#endif
