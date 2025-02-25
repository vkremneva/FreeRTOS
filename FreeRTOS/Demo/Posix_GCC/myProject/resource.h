#ifndef RESOURCE_H
#define RESOURCE_H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "department.h"

#define resourceTASK_PRIORITY 3

#define resourceQUEUE_SIZE 10
#define resourceQUEUE_ITEM_SIZE (sizeof(resource_request_t))

#define resourceUSE_MIN_TIME 500
#define resourceUSE_MAX_TIME 3000

extern QueueHandle_t xQueueResources;

typedef struct {
    char *department_name;
    int16_t event_code;
    TickType_t usage_start_time;
    SemaphoreHandle_t *xDepartmentSemaphore;
} resource_request_t;

void vResourceTask(void *pvParameters);

#endif