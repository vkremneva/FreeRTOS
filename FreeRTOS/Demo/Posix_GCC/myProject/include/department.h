/**
 * @file department.h
 * @brief Department management module.
 *
 * This module defines the department entity, its configuration, and
 * FreeRTOS task responsible for managing department queues and resources.
 */

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

#define deptQUEUE_SIZE                 10000
#define deptQUEUE_ITEM_SIZE            ( sizeof( event_t ))
#define deptMAX_GROUP_WAIT_TIME        500
#define deptMAX_DEPARTMENTS_AMOUNT     10

#define deptPOLICE_ID                  1
#define deptAMBULANCE_ID               2
#define deptFIREFIGHTERS_ID            3
#define deptCORONA_ID                  4

#define deptPOLICE_CARS_TOTAL          3
#define deptAMBULANCE_CARS_TOTAL       4
#define deptFIREFIGHTERS_CARS_TOTAL    2
#define deptCORONA_CARS_TOTAL          4

#define deptPOLICE_PRIORITY            3
#define deptAMBULANCE_PRIORITY         4
#define deptFIREFIGHTERS_PRIORITY      4
#define deptCORONA_PRIORITY            2

/** @brief Bitmask of currently available resources across all departments. */
extern EventBits_t uxBitsAvailableAll;

/** @brief Event group handle used for department synchronization. */
extern EventGroupHandle_t xDepartmentEventGroup;

/** @brief Number of initialized departments. */
extern UBaseType_t uxDepartmentsAmount;

/** @brief Number of resources managed across departments. */
extern UBaseType_t uxResourcesAmount;

/** @brief Array containing department execution priority order. */
extern UBaseType_t uxDeptPriorityOrder[ deptMAX_DEPARTMENTS_AMOUNT ];

/**
 * @brief Department entity descriptor.
 */
typedef struct
{
    char             * psName;          /**< Human-readable department name. */
    uint8_t          ucID;              /**< Unique department ID. */
    uint8_t          ucPriority;        /**< Department priority. */
    UBaseType_t      uxCarsTotal;       /**< Total number of vehicles. */
    UBaseType_t      uxCarsAvailable;   /**< Number of available vehicles. */
    UBaseType_t      uxBitsAvailable;   /**< Event group bits allocated. */
    QueueHandle_t    xQueue;            /**< Department’s message queue. */
} department_t;

/**
 * @brief Initialize all departments and return array of department descriptors.
 * @return Pointer to the first department_t element.
 */
department_t* pxInitDepartments();

/**
 * @brief FreeRTOS task for managing department operations.
 * @param pvParameters Pointer to the department_d descriptor of this department.
 */
void vDepartmentTask( void * pvParameters );

#endif /* ifndef DEPARTMENT_H */
