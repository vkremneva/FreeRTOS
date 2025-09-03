/**
 * @file resource.h
 * @brief Resource management module.
 *
 * Manages allocation and release of department resources
 * in response to incoming requests.
 */

#ifndef RESOURCE_H
#define RESOURCE_H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "department.h"

#define resourceTASK_PRIORITY      4

#define resourceQUEUE_SIZE         10000
#define resourceQUEUE_ITEM_SIZE    ( sizeof( resource_request_t ))

#define resourceUSE_MIN_TIME       1000
#define resourceUSE_MAX_TIME       5000

/** @brief Global resource request queue handle. */
extern QueueHandle_t xQueueResources;

/**
 * @brief Resource request descriptor.
 */
typedef struct
{
    char             * psDepartmentName; /**< Requesting department name. */
    uint8_t          ucDepartmentID;     /**< Department ID. */
    uint8_t          ucEventCode;        /**< Associated event code. */
    TickType_t       xUsageStartTime;    /**< Resource usage start time. */
    QueueHandle_t    xDepartmentQueue;   /**< Department’s queue for callbacks. */
} resource_request_t;

/**
 * @brief FreeRTOS task for managing resources.
 * @param pvParameters Unused for this task.
 */
void vResourceTask( void * pvParameters );

#endif /* ifndef RESOURCE_H */
