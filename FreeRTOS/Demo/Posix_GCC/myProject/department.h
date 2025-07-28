#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "event_groups.h"

#include "events.h"
#include "logging.h"
#include "resource.h"

#define deptQUEUE_SIZE              100
#define deptQUEUE_ITEM_SIZE         (sizeof(event_t))
#define deptMAX_GROUP_WAIT_TIME     500
#define deptMAX_DEPARTMENTS_AMOUNT  10

#define deptPOLICE_ID               1
#define deptAMBULANCE_ID            2
#define deptFIREFIGHTERS_ID         3
#define deptCORONA_ID               4

#define deptPOLICE_CARS_TOTAL       3
#define deptAMBULANCE_CARS_TOTAL    4
#define deptFIREFIGHTERS_CARS_TOTAL 2
#define deptCORONA_CARS_TOTAL       4

#define deptPOLICE_PRIORITY         3
#define deptAMBULANCE_PRIORITY      4
#define deptFIREFIGHTERS_PRIORITY   4
#define deptCORONA_PRIORITY         2

//#define deptPOLICE_AVAILABLE       (1 << deptPOLICE_ID)
//#define deptAMBULANCE_AVAILABLE    (1 << deptAMBULANCE_ID)
//#define deptFIRE_AVAILABLE         (1 << deptFIREFIGHTERS_ID )
//#define deptCORONA_AVAILABLE       (1 << deptCORONA_ID)

extern QueueHandle_t        xQueueEvents;
extern EventBits_t          uxBitsAvailableAll;
extern EventGroupHandle_t   xDepartmentEventGroup;

extern UBaseType_t uxDepartmentsAmount;
extern UBaseType_t uxResourcesAmount;
extern UBaseType_t uxDeptPriorityOrder[deptMAX_DEPARTMENTS_AMOUNT];

/* For debug puposes only. */
extern struct logCSV log_to_csv;

typedef struct {
    char                *psName;
    uint8_t             ucID;
    uint8_t             ucPriority;
    UBaseType_t         uxCarsTotal;
    UBaseType_t         uxCarsAvailable;
    UBaseType_t         uxCallsTotal; 
    UBaseType_t         uxBitsAvailable;
    QueueHandle_t       xQueue;
    SemaphoreHandle_t   xCountSemaphore;
} department_t;

department_t* pxInitDepartments();
void vDepartmentTask(void *pvParameters);

#endif 
