#include "logging.h"

void logDepartmentUsage( char *pcDepartmentName, UBaseType_t ulDepartmentID, TickType_t xUseTime ) {
    printf("%s: Receiving the event code %lu. ", pcDepartmentName, ulDepartmentID);
    printf("A free resource was allocated. ");
    printf("Task lasted %lu. The task was completed.\n", xUseTime);
}

void logNoResourceAvailable( char *pcDepartmentName ) {
    printf("%s: No resources available. ", pcDepartmentName);
    printf("The event is remaining in the queue for further handling.");
    printf("The task failed.\n");
}

void logQueueSendError(char *pcQueueName) {
    printf("ERROR: Failed to send to %s\n", pcQueueName);
}