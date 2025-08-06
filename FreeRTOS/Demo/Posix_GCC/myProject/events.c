#include "events.h"

QueueHandle_t xQueueEvents;

void vEventGeneratorTask(void *pvParameters) {
    QueueHandle_t xQueueEvents = (QueueHandle_t)pvParameters;
    TickType_t xTicksToWait = 0;
    BaseType_t xStatusSend = 0;

    event_t xEvent = { 0, false, eventsCODE, 0, 0 };

    for ( ;; ) {
        xEvent.ucCode = eventsMIN_EVENT_CODE + rand() % (eventsMAX_EVENT_CODE - eventsMIN_EVENT_CODE + 1);

        xStatusSend = xQueueSend(xQueueEvents, &xEvent, portMAX_DELAY);
        if (xStatusSend == errQUEUE_FULL) {
            vLogQueueSendError("Event");
        }

        /* For debug puposes only. */
        addToCSVLog(&log_to_csv, xTaskGetTickCount(), xEvent.ucCode, "Event Generator Task");

        xTicksToWait = pdMS_TO_TICKS( eventsMIN_WAIT_TIME + rand() % (eventsMAX_WAIT_TIME - eventsMIN_WAIT_TIME + 1) );
        vTaskDelay(xTicksToWait);
    }
}
