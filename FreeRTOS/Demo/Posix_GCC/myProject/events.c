#include "events.h"

void vEventGeneratorTask(void *param) {
    QueueHandle_t xQueueEvents = (QueueHandle_t)param;
    int16_t sEventCode = 0;
    TickType_t xTicksToWait = 0;
    BaseType_t xStatusSend = 0;

    for ( ;; ) {
        sEventCode = rand() % 3 + 1;
        xStatusSend = xQueueSend(xQueueEvents, &sEventCode, portMAX_DELAY);
        if (xStatusSend != pdPASS) {
            printf("Event Generation Task: Sending event code FAILURE: code is %d\n", sEventCode);
        }

        xTicksToWait = pdMS_TO_TICKS( rand() % 3000 + 500 );
        vTaskDelay(xTicksToWait);
    }
}