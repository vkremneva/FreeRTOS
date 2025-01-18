#ifndef UTILS_H
#define UTILS_H

#include "FreeRTOS.h"
#include "logging.h"

/* 
    I use this function simply for readability in other tasks
    I don't really like having separate module for just one function,
    which is also does logging, so might refactor later
*/
bool xCheckPdPASS(BaseType_t xStatus, char *pTaskName, char *pMsg);

#endif