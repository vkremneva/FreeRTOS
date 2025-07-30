#ifndef RESOURCE_H
#define RESOURCE_H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "department.h"

#define resourceTASK_PRIORITY       4

#define resourceQUEUE_SIZE          10
#define resourceQUEUE_ITEM_SIZE     (sizeof(resource_request_t))

#define resourceUSE_MIN_TIME        500
#define resourceUSE_MAX_TIME        3000

extern QueueHandle_t xQueueResources;

typedef struct {
    char                *psDepartmentName;
    uint8_t              ucDepartmentID;
    uint8_t              ucEventCode;
    TickType_t           xUsageStartTime;
    QueueHandle_t        xDepartmentQueue;
} resource_request_t;

void vResourceTask(void *pvParameters);

#endif