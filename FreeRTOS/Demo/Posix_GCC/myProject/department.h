#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <stdio.h>
#include <unistd.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#define deptPOLICE_ID 1
#define deptAMBULANCE_ID 2
#define deptFIREFIGHTERS_ID 3
#define deptCORONA_ID 4

#define deptPOLICE_CARS_TOTAL 3
#define deptAMBULANCE_CARS_TOTAL 4
#define deptFIREFIGHTERS_CARS_TOTAL 2
#define deptCORONA_CARS_TOTAL 4

#define deptPOLICE_PRIORITY 3
#define deptAMBULANCE_PRIORITY 3
#define deptFIREFIGHTERS_PRIORITY 3
#define deptCORONA_PRIORITY 3

typedef struct {
    size_t id;
    size_t cars_total;
    size_t cars_available;
    size_t cars_occupied;
    size_t calls_total; 
} department_t;

void vDeptPoliceTask(void *param);
void vDeptAmbulanceTask(void *param);
void vDeptFirefightersTask(void *param);
void vDeptCoronaTask(void *param);

#endif