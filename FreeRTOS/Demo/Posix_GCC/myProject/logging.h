#ifndef LOGGING_H
#define LOGGING_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"

#define logBUFFER_SIZE 256
#define logDEPT_NAME_MAXLEN 50

void logDepartmentUsage( char *pcDepartmentName, UBaseType_t ulDepartmentID, TickType_t xUseTime );
void logNoResourceAvailable( char *pcDepartmentName );

#endif