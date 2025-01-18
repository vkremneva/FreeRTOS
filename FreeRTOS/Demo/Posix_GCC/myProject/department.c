#include "department.h"

void vUseResource( void *pvParameters ) {
    SemaphoreHandle_t xCountingSemaphore = (SemaphoreHandle_t)pvParameters;
    BaseType_t xStatusGive = 0;
    TickType_t xTicksToWait = pdMS_TO_TICKS( rand() % deptUSE_RESOURCE_MAX_TIME + deptUSE_RESOURCE_MIN_TIME );

    vTaskDelay( xTicksToWait );

    xStatusGive = xSemaphoreGive( xCountingSemaphore );
    xCheckPdPASS( xStatusGive, logUSE_RESOURCE, logGIVE_SEMAPHORE );
}

void vDepartmentTask( void *pvParameters ) {
    department_t *xDepartment = (department_t *)pvParameters;

    BaseType_t xStatusReceive = 0, xStatusTake = 0;
    int16_t sEventCode = 0;
    TickType_t xStartTime = 0, xEndTime = 0;

        for ( ;; ) {
        xStatusReceive = xQueueReceive( xDepartment->queue, &sEventCode, portMAX_DELAY );
        if ( xCheckPdPASS(xStatusReceive, xDepartment->name, logRECEIVE_CODE )) {

            // start counting ticks after receiving the message 
            xStartTime = xTaskGetTickCount();

            xStatusTake = xSemaphoreTake( xDepartment->countingSemaphore, portMAX_DELAY );
            if (xCheckPdPASS( xStatusTake, xDepartment->name, logTAKE_SEMAPHORE) ) {

                vUseResource( (void *)xDepartment->countingSemaphore );
            }
            xEndTime = xTaskGetTickCount();

            logDepartmentUsage( xDepartment->name, xDepartment->id, ( xEndTime - xStartTime ) );
        }   
    }
}