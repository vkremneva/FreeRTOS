#ifndef EVENTS_H
#define EVENTS_H

#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#define eventsQUEUE_SIZE              10000
#define eventsQUEUE_ITEM_SIZE         ( sizeof( event_t ))

#define eventsPRIORITY                3

#define eventsMIN_WAIT_TIME           200
#define eventsMAX_WAIT_TIME           1000

#define eventsMAX_EVENT_CODE          3
#define eventsMIN_EVENT_CODE          1

#define eventsCODE                    1
#define eventsFREE_RESOURCE           2

#define eventsCOUNTER_REJECTED_MAX    10

typedef struct
{
    uint8_t       ucCode;
    bool          xRejected;
    BaseType_t    uxType;
    BaseType_t    uxCounterRejected;
    TickType_t    xUsageStartTime;
} event_t;

extern QueueHandle_t xQueueEvents;
extern void vLogQueueSendError( char * pcQueueName );

/* For debug puposes only. */
extern struct logCSV log_to_csv;

void vEventGeneratorTask( void * pvParameters );

#endif /* ifndef EVENTS_H */
