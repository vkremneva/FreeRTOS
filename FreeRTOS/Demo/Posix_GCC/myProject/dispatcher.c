#include "dispatcher.h"

void vDispatcherTask(void *param) {
    QueueHandle_t xQueueEvents = (QueueHandle_t)param;
    TickType_t xLastWakeTime;
    BaseType_t xStatusReceive = 0, xStatusSend = 0;
    int16_t sEventCode = 0;

    // get the current tick count
    xLastWakeTime = xTaskGetTickCount();

    for ( ;; ) {
        xStatusReceive = xQueueReceive(xQueueEvents, &sEventCode, portMAX_DELAY);

        if (xStatusReceive == pdPASS) {
            printf("Dispatcher Task: Receiving the code SUCCESS: code is %d\n", sEventCode);

            if (sEventCode == dispPOLICE_ID) {
                xStatusSend = xQueueSend(xPoliceQueue, &sEventCode, portMAX_DELAY);
            } else if (sEventCode == dispAMBULANCE_ID) {
                xStatusSend = xQueueSend(xAmbulanceQueue, &sEventCode, portMAX_DELAY);
            } else if (sEventCode == dispFIREFIGHTERS_ID) {
                xStatusSend = xQueueSend(xFirefightersQueue, &sEventCode, portMAX_DELAY);
            } else {
                printf("Dispatcher Task: ERROR no department with code: %d\n", sEventCode);
            }

            if (xStatusSend == pdPASS) {
                printf ("Dispatcher Task: Sending the code SUCCESS\n");
            } else if (xStatusSend == errQUEUE_FULL) {
                printf("Dispatcher Task: Sending the code FAILURE: queue is already full\n");
            } else {
                printf("Dispatcher Task: Sending the code FAILURE: queue error\n");
            }

        } else if (xStatusReceive == errQUEUE_EMPTY) {
            printf("Dispatcher Task: Receiving the code FAILURE: queue is empty\n");
        } else {
            printf("Dispatcher Task: Receiving the code FAILURE: queue error\n");
        }

        vTaskDelayUntil( &xLastWakeTime, dispatcherDELAY );
    }
}