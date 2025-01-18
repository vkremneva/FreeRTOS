#include "logging.h"

void logDepartmentUsage( char *pcDepartmentName, UBaseType_t ulDepartmentID, TickType_t xUseTime ) {
    char logBuffer[logBUFFER_SIZE];

    snprintf(logBuffer, logBUFFER_SIZE, "%s: %s %lu. %s. %s %lu. %s.\n",
             pcDepartmentName, logRECEIVE_CODE, ulDepartmentID, 
             logRESOURCE_ALLOCATED,
             logTASK_TIME, xUseTime,
             logTASK_SUCCES);

    printf("%s\n", logBuffer);
}