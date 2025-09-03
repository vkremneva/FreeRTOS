/**
 * @file events.h
 * @brief Event generation module.
 *
 * Defines the event entity and the FreeRTOS task responsible
 * for generating random events for departments to process.
 */

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

/** @brief Event type: Sending generated event code. */
#define eventsCODE                    1
/** @brief Event type: To free the resource taken by an event. */
#define eventsFREE_RESOURCE           2

/** @brief Maximum number of times an event may be rejected. */
#define eventsCOUNTER_REJECTED_MAX    10


/**
 * @brief Event descriptor.
 */
typedef struct
{
    uint8_t       ucCode;              /**< Generated event code. */
    bool          xRejected;           /**< Flag if event was rejected. */
    BaseType_t    uxType;              /**< Event type: Sending code or Freeing the resource. */
    BaseType_t    uxCounterRejected;   /**< Number of times rejected. */
    TickType_t    xUsageStartTime;     /**< Timestamp when resource use began. */
} event_t;

/** @brief Global event queue handle. */
extern QueueHandle_t xQueueEvents;

/**
 * @brief FreeRTOS task for generating random events.
 * @param pvParameters Pointer to task parameters (if any).
 */
void vEventGeneratorTask( void * pvParameters );

#endif /* ifndef EVENTS_H */
