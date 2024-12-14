#include "dispatcher.h"

void vDispatcherTask(void *param) {
    QueueHandle_t xQueueEvents = (QueueHandle_t)param;
    // TODO periodic lauch
    BaseType_t xStatusReceive = 0, xStatusSend = 0;
    int16_t sEventCode = 0;
    for ( ;; ) {
        xStatusReceive = xQueueReceive(xQueueEvents, &sEventCode, portMAX_DELAY);

        if (xStatusReceive == pdPASS) {
            printf("Dispatcher Task: Receiving the code SUCCESS: code is %d\n", sEventCode);

            xStatusSend = xQueueSend(xPoliceQueue, &sEventCode, portMAX_DELAY);
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
    }
}