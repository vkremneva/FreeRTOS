#ifndef LOGGING_H
#define LOGGING_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"

#define logBUFFER_SIZE 256
#define logDEPT_NAME_MAXLEN 50

#define logRECEIVE_CODE "Receiving the event code"
#define logSEND_CODE "Sending the event code"
#define logTAKE_SEMAPHORE "Taking the counting semaphore"
#define logGIVE_SEMAPHORE "Giving the counting semaphore"
#define logUSE_RESOURCE "Using the resource"

#define logEVENT_TASK_NAME "Event Generation Task"
#define logRESOURCE_ALLOCATED "A free resource was allocated"
#define logTASK_TIME "Task lasted"
#define logTASK_SUCCES "The task was completed"

void logDepartmentUsage( char *pcDepartmentName, UBaseType_t ulDepartmentID, TickType_t xUseTime );

#endif