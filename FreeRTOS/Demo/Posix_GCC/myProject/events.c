#include "events.h"

QueueHandle_t xQueueEvents;

void vEventGeneratorTask(void *pvParameters) {
    QueueHandle_t xQueueEvents = (QueueHandle_t)pvParameters;
    TickType_t xTicksToWait = 0;
    BaseType_t xStatusSend = 0;

    event_t xEvent = { 0, false, eventCODE, 0 };

    for ( ;; ) {
        xEvent.ucCode = rand() % eventsMAX_EVENT_CODE + eventsMIN_EVENT_CODE;
        xStatusSend = xQueueSend(xQueueEvents, &xEvent, portMAX_DELAY);

        /* For debug puposes only. */
        addToCSVLog(&log_to_csv, xTaskGetTickCount(), xEvent.ucCode, "Event Generator Task");

        xTicksToWait = pdMS_TO_TICKS( rand() % eventMAX_WAIT_TIME + eventsMIN_WAIT_TIME );
        vTaskDelay(xTicksToWait);
    }
}
