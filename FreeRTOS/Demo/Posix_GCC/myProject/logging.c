#include "logging.h"

void logDepartmentUsage( char *pcDepartmentName, UBaseType_t ulDepartmentID, TickType_t xUseTime ) {
    char logBuffer[logBUFFER_SIZE];

    snprintf(logBuffer, logBUFFER_SIZE, "%s: Receiving the event code %lu. "
                                        "A free resource was allocated. "
                                        "Task lasted %lu. The task was completed.",
             pcDepartmentName, 
             ulDepartmentID, 
             xUseTime
    );

    printf("%s\n", logBuffer);
}

void logNoResourceAvailable( char *pcDepartmentName ) {
    char logBuffer[logBUFFER_SIZE];

    sprintf(logBuffer, logBUFFER_SIZE, "%s: No resources available. "
                                        "The event is remaining in the queue for further handling. "
                                        "The task failed.",
            pcDepartmentName
    );
    
    printf("%s\n", logBuffer);
}