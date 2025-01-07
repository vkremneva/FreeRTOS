#ifndef LOGGING_H
#define LOGGING_H

#include <stdbool.h>
#include <stdio.h>
#include "FreeRTOS.h"

// TODO use trace hook macros?
#define logRECEIVE_CODE "Receiving the event code"
#define logSEND_CODE "Sending the event code"
#define logTAKE_SEMAPHORE "Taking the counting semaphore"
#define logGIVE_SEMAPHORE "Giving the counting semaphore"

bool xCheckPdPASS(BaseType_t xStatus, char *pTaskName, char *pMsg);

#endif