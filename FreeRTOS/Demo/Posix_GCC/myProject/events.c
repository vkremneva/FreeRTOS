#include "events.h"

void vEventGeneratorTask(void *param) {
    QueueHandle_t xQueueEvents = (QueueHandle_t)param;
    int16_t sEventCode = 0;
    int32_t lWaitTime = 0;
    BaseType_t xStatusSend = 0;
    srand(time(NULL)); 

    for ( ;; ) {
        // TODO: randomize event codes
//        sEventCode = rand() % 3 + 1;
        sEventCode = 1;
        xStatusSend = xQueueSend(xQueueEvents, &sEventCode, portMAX_DELAY);
        if (xStatusSend == pdPASS) {
            printf("Event Generation Task: Sending event code SUCCESS: code is %d\n", sEventCode);
        } else {
            printf("Event Generation Task: Sending event code FAILURE: code is %d\n", sEventCode);
        }

        // TODO: randomize wait time
//        lWaitTime = rand() % 3000 + 500;
        lWaitTime = 600;
        vTaskDelay(pdMS_TO_TICKS(lWaitTime));
    }
}