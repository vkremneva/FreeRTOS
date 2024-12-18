#include "events.h"

void vEventGeneratorTask(void *pvParameters) {
    QueueHandle_t xQueueEvents = (QueueHandle_t)pvParameters;
    int16_t sEventCode = 0;
    TickType_t xTicksToWait = 0;
    BaseType_t xStatusSend = 0;

    for ( ;; ) {
        sEventCode = rand() % eventsMAX_EVENT_CODE + eventsMIN_EVENT_CODE;
        xStatusSend = xQueueSend(xQueueEvents, &sEventCode, portMAX_DELAY);

        xCheckPdPASS(xStatusSend, "Event Generation Task", logSEND_CODE);

        xTicksToWait = pdMS_TO_TICKS( rand() % eventMAX_WAIT_TIME + eventsMIN_WAIT_TIME );
        vTaskDelay(xTicksToWait);
    }
}