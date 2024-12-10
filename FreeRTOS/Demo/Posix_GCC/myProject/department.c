#include "department.h"

void vDeptPoliceTask(void *param) {
    QueueHandle_t xQueueEvents = (QueueHandle_t)param;
    int xStatusReceive = 0;
    int xMsgReceive = 0;

    department_t xPolice;
    xPolice.id = deptPOLICE_ID;
    xPolice.calls_total = deptPOLICE_CARS_TOTAL;
    xPolice.cars_available = xPolice.cars_total;
    xPolice.cars_occupied = 0;

    for ( ;; ) {
        xStatusReceive = xQueueReceive(xQueueEvents, &xMsgReceive, portMAX_DELAY);

        if (xStatusReceive == pdPASS) {
            
        } else if (xStatusReceive== errQUEUE_EMPTY) {
            
        } else {
            printf("Control Unit Task: queue error\n");
        }
    }
}


void vDeptAmbulanceTask(void *param) {

    department_t xAmbulance;
    xAmbulance.id = deptAMBULANCE_ID;
    xAmbulance.calls_total = deptAMBULANCE_CARS_TOTAL;
    xAmbulance.cars_available = xAmbulance.cars_total;
    xAmbulance.cars_occupied = 0;

    for ( ;; ) {
    }
}

void vDeptFirefightersTask(void *param){

    department_t xFirefighters;
    xFirefighters.id = deptFIREFIGHTERS_ID;
    xFirefighters.calls_total = deptFIREFIGHTERS_CARS_TOTAL;
    xFirefighters.cars_available = xFirefighters.cars_total;
    xFirefighters.cars_occupied = 0;

    for ( ;; ) {
    }
}

void vDeptCoronaTask(void *param) {

    department_t xCorona;
    xCorona.id = deptCORONA_ID;
    xCorona.calls_total = deptCORONA_CARS_TOTAL;
    xCorona.cars_available = xCorona.cars_total;
    xCorona.cars_occupied = 0;

    for ( ;; ) {
    }
}
