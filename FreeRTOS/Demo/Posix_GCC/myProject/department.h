#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "logging.h"

// TODO not so sure about 10
#define deptQUEUE_SIZE 10
#define deptQUEUE_ITEM_SIZE (sizeof(int16_t))

#define deptPOLICE_ID 1
#define deptAMBULANCE_ID 2
#define deptFIREFIGHTERS_ID 3
#define deptCORONA_ID 4

#define deptPOLICE_CARS_TOTAL 3
#define deptAMBULANCE_CARS_TOTAL 4
#define deptFIREFIGHTERS_CARS_TOTAL 2
#define deptCORONA_CARS_TOTAL 4

// TODO tweak
#define deptPOLICE_PRIORITY 3
#define deptAMBULANCE_PRIORITY 3
#define deptFIREFIGHTERS_PRIORITY 3
#define deptCORONA_PRIORITY 3

typedef struct {
    char *name;
    UBaseType_t id;
    UBaseType_t priority;
    UBaseType_t cars_total;
    UBaseType_t cars_available;
    UBaseType_t cars_occupied;
    UBaseType_t calls_total; 
    QueueHandle_t queue;
    SemaphoreHandle_t countingSemaphore;
} department_t;

void vUseResource(void *pvParameters);
void vDepartmentTask(void *pvParameters);

#endif