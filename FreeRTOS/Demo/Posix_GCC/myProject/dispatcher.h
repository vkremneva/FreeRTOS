#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#define dispatcherPRIORITY 3
#define dispatcherDELAY ( pdMS_TO_TICKS( 1000 ))

#define dispPOLICE_ID 1
#define dispAMBULANCE_ID 2
#define dispFIREFIGHTERS_ID 3
#define dispCORONA_ID 4

extern QueueHandle_t xPoliceQueue;
extern QueueHandle_t xAmbulanceQueue;
extern QueueHandle_t xFirefightersQueue;

void vDispatcherTask(void *param);

#endif