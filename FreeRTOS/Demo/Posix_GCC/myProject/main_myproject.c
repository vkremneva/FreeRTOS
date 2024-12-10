#include <stdio.h>
#include <unistd.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "department.h"

void main_myproject(void)
{
    xTaskCreate(vDeptPoliceTask, "Police Task", configMINIMAL_STACK_SIZE, NULL, deptPOLICE_PRIORITY, NULL);
    xTaskCreate(vDeptAmbulanceTask, "Ambulance Task", configMINIMAL_STACK_SIZE, NULL, deptAMBULANCE_PRIORITY, NULL);
    xTaskCreate(vDeptFirefightersTask, "Firefighters Task", configMINIMAL_STACK_SIZE, NULL, deptFIREFIGHTERS_PRIORITY, NULL);
    xTaskCreate(vDeptCoronaTask, "Corona Task", configMINIMAL_STACK_SIZE, NULL, deptCORONA_PRIORITY, NULL);

    vTaskStartScheduler();

    for ( ;; )
        ;
}
