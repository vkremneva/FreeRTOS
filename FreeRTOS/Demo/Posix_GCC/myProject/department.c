#include "department.h"

// omg how am I not sure about all this
department_t xPolice = { deptPOLICE_ID, deptPOLICE_CARS_TOTAL, deptPOLICE_CARS_TOTAL, 0, 0};
QueueHandle_t xPoliceQueue = NULL;
SemaphoreHandle_t xPoliceCountingSemaphore = NULL;

department_t xAmbulance = { deptAMBULANCE_ID, deptAMBULANCE_CARS_TOTAL, deptAMBULANCE_CARS_TOTAL, 0, 0};
QueueHandle_t xAmbulanceQueue = NULL;
SemaphoreHandle_t xAmbulanceCountingSemaphore = NULL;

department_t xFirefighters = { deptFIREFIGHTERS_ID, deptFIREFIGHTERS_CARS_TOTAL, deptFIREFIGHTERS_CARS_TOTAL, 0, 0};
QueueHandle_t xFirefightersQueue = NULL;
SemaphoreHandle_t xFirefightersCountingSemaphore = NULL;

department_t xCorona = { deptCORONA_ID, deptCORONA_CARS_TOTAL, deptCORONA_CARS_TOTAL, 0, 0};
QueueHandle_t xCoronaQueue = NULL;
SemaphoreHandle_t xCoronaCountingSemaphore = NULL;

void vUseResource(void *param) {
    SemaphoreHandle_t xCountingSemaphore = (SemaphoreHandle_t)param;
    BaseType_t xStatusGive = 0;
    TickType_t xTicksToWait = pdMS_TO_TICKS( rand() % 3000 + 500 );

    printf("Using Resource: STARTED\n");

    vTaskDelay( xTicksToWait );

    xStatusGive = xSemaphoreGive(xCountingSemaphore);
    if (xStatusGive != pdPASS) {
        printf("Using Resource: Semaphore giving FAILURE\n");
    }
    printf("Using Resource: FINISHED in %d ticks\n", xTicksToWait);
}

void vDeptPoliceTask(void *param) {
    BaseType_t xStatusReceive = 0, xStatusTake = 0;
    int16_t sEventCode = 0;

    for ( ;; ) {
        xStatusReceive = xQueueReceive(xPoliceQueue, &sEventCode, portMAX_DELAY);

        if (xStatusReceive == pdPASS) {
            printf("Police Task: Receiving the code SUCCESS: code is: %d\n", sEventCode);
            xStatusTake = xSemaphoreTake(xPoliceCountingSemaphore, portMAX_DELAY);
            if (xStatusTake == pdPASS) {
                printf("Police Task: Semaphore taking SUCCESS\n");
                vUseResource((void *)xPoliceCountingSemaphore);
            }
        } else if (xStatusReceive == errQUEUE_EMPTY) {
            printf("Police Task: Receiving the code FAILURE: queue is empty\n");
        } else {
            printf("Police Task: Receiving the code FAILURE: queue error\n");
        }
    }
}

void vDeptAmbulanceTask(void *param) {
    BaseType_t xStatusReceive = 0, xStatusTake = 0;
    int16_t sEventCode = 0;

    for ( ;; ) {
        xStatusReceive = xQueueReceive(xAmbulanceQueue, &sEventCode, portMAX_DELAY);

        if (xStatusReceive == pdPASS) {
            printf("Ambulance Task: Receiving the code SUCCESS: code is: %d\n", sEventCode);
            xStatusTake = xSemaphoreTake(xAmbulanceCountingSemaphore, portMAX_DELAY);
            if (xStatusTake == pdPASS) {
                printf("Ambulance Task: Semaphore taking SUCCESS\n");
                vUseResource((void *)xAmbulanceCountingSemaphore);
            }
        } else if (xStatusReceive == errQUEUE_EMPTY) {
            printf("Ambulance Task: Receiving the code FAILURE: queue is empty\n");
        } else {
            printf("Ambulance Task: Receiving the code FAILURE: queue error\n");
        }
    }
}

void vDeptFirefightersTask(void *param) {
    BaseType_t xStatusReceive = 0, xStatusTake = 0;
    int16_t sEventCode = 0;

    for ( ;; ) {
        xStatusReceive = xQueueReceive(xFirefightersQueue, &sEventCode, portMAX_DELAY);

        if (xStatusReceive == pdPASS) {
            printf("Firefighters Task: Receiving the code SUCCESS: code is: %d\n", sEventCode);
            xStatusTake = xSemaphoreTake(xFirefightersCountingSemaphore, portMAX_DELAY);
            if (xStatusTake == pdPASS) {
                printf("Firefighters Task: Semaphore taking SUCCESS\n");
                vUseResource((void *)xFirefightersCountingSemaphore);
            }
        } else if (xStatusReceive == errQUEUE_EMPTY) {
            printf("Firefighters Task: Receiving the code FAILURE: queue is empty\n");
        } else {
            printf("Firefighters Task: Receiving the code FAILURE: queue error\n");
        }
    }
}

void vDeptCoronaTask(void *param) {
    BaseType_t xStatusReceive = 0, xStatusTake = 0;
    int16_t sEventCode = 0;

    for ( ;; ) {
        xStatusReceive = xQueueReceive(xCoronaQueue, &sEventCode, portMAX_DELAY);

        if (xStatusReceive == pdPASS) {
            printf("Corona Task: Receiving the code SUCCESS: code is: %d\n", sEventCode);
            xStatusTake = xSemaphoreTake(xCoronaCountingSemaphore, portMAX_DELAY);
            if (xStatusTake == pdPASS) {
                printf("Corona Task: Semaphore taking SUCCESS\n");
                vUseResource((void *)xCoronaCountingSemaphore);
            }
        } else if (xStatusReceive == errQUEUE_EMPTY) {
            printf("Corona Task: Receiving the code FAILURE: queue is empty\n");
        } else {
            printf("Corona Task: Receiving the code FAILURE: queue error\n");
        }
    }
}