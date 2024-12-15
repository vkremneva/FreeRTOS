#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <stdio.h>
#include <unistd.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

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

//#define deptUSING_RESOURCE_DELAY ( pdMS_TO_TICKS( 500 ) ) 

typedef struct {
    size_t id;
    size_t cars_total;
    size_t cars_available;
    size_t cars_occupied;
    size_t calls_total; 
} department_t;

extern department_t xPolice;
extern QueueHandle_t xPoliceQueue;
extern SemaphoreHandle_t xPoliceCountingSemaphore;

extern department_t xAmbulance;
extern QueueHandle_t xAmbulanceQueue;
extern SemaphoreHandle_t xAmbulanceCountingSemaphore;

extern department_t xFirefighters;
extern QueueHandle_t xPFirefightersQueue;
extern SemaphoreHandle_t xFirefightersCountingSemaphore;

extern department_t xCorona;
extern QueueHandle_t xCoronaQueue;
extern SemaphoreHandle_t xCoronaCountingSemaphore;

void vUseResource(void *param);

void vDeptPoliceTask(void *param);
void vDeptAmbulanceTask(void *param);
void vDeptFirefightersTask(void *param);
void vDeptCoronaTask(void *param);

#endif