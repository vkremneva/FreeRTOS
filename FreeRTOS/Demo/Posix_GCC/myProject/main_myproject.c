#include <stdio.h>
#include <unistd.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
 
#include "events.h"
#include "dispatcher.h"
#include "department.h"

QueueHandle_t xQueueEvents;

void main_myproject(void)
{
    srand(time(NULL));

    xQueueEvents = xQueueCreate(eventsQUEUE_SIZE, eventsQUEUE_ITEM_SIZE);

    xPoliceQueue = xQueueCreate(deptQUEUE_SIZE, deptQUEUE_ITEM_SIZE);
    xAmbulanceQueue = xQueueCreate(deptQUEUE_SIZE, deptQUEUE_ITEM_SIZE);
    xFirefightersQueue = xQueueCreate(deptQUEUE_SIZE, deptQUEUE_ITEM_SIZE);
    xCoronaQueue = xQueueCreate(deptQUEUE_SIZE, deptQUEUE_ITEM_SIZE);

    xPoliceCountingSemaphore = xSemaphoreCreateCounting( deptPOLICE_CARS_TOTAL, deptPOLICE_CARS_TOTAL );  
    xAmbulanceCountingSemaphore = xSemaphoreCreateCounting( deptAMBULANCE_CARS_TOTAL, deptAMBULANCE_CARS_TOTAL );  
    xFirefightersCountingSemaphore = xSemaphoreCreateCounting( deptFIREFIGHTERS_CARS_TOTAL, deptFIREFIGHTERS_CARS_TOTAL );  
    xCoronaCountingSemaphore = xSemaphoreCreateCounting( deptCORONA_CARS_TOTAL, deptCORONA_CARS_TOTAL );  

    xTaskCreate(vEventGeneratorTask, "Event Generator Task", configMINIMAL_STACK_SIZE, (void *)xQueueEvents, eventsPRIORITY, NULL);
    xTaskCreate(vDispatcherTask, "Dispatcher Task", configMINIMAL_STACK_SIZE, (void *)xQueueEvents, dispatcherPRIORITY, NULL);

    xTaskCreate(vDeptPoliceTask, "Police Task", configMINIMAL_STACK_SIZE, NULL, deptPOLICE_PRIORITY, NULL);
    xTaskCreate(vDeptAmbulanceTask, "Ambulance Task", configMINIMAL_STACK_SIZE, NULL, deptAMBULANCE_PRIORITY, NULL);
    xTaskCreate(vDeptFirefightersTask, "Firefighters Task", configMINIMAL_STACK_SIZE, NULL, deptFIREFIGHTERS_PRIORITY, NULL);
    xTaskCreate(vDeptCoronaTask, "Corona Task", configMINIMAL_STACK_SIZE, NULL, deptCORONA_PRIORITY, NULL);

    vTaskStartScheduler();

    for ( ;; )
        ;
}
