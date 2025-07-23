#include "logging.h"

void vLogDepartmentUsage( char *pcDepartmentName, UBaseType_t ulDepartmentID, TickType_t xUseTime ) {
    printf("%s: Receiving the event code %lu. ", pcDepartmentName, ulDepartmentID);
    printf("A free resource was allocated. ");
    printf("Task lasted %lu. The task was completed.\n", xUseTime);
}
/*-----------------------------------------------------------*/

void vLogNoResourceAvailable( char *pcDepartmentName ) {
    printf("%s: No resources available. ", pcDepartmentName);
    printf("The event is remaining in the queue for further handling. ");
    printf("The task failed.\n");
}
/*-----------------------------------------------------------*/

void vLogQueueSendError(char *pcQueueName) {
    printf("ERROR: Failed to send to %s, it is full.\n", pcQueueName);
}
/*-----------------------------------------------------------*/

/* For debug puposes only. */
void addToCSVLog(struct logCSV *log, TickType_t timestamp, int16_t eventcode, char* msg) {
    int index = atomic_fetch_add(&log->ind, 1);
    log->log[index].timestamp = timestamp;
    log->log[index].eventcode = eventcode;
    memcpy(log->log[index].msg, msg, logCSV_MSG_SIZE * sizeof(char));

    if (timestamp >= logCSV_UPTIME) {
        FILE *file;
        file = fopen(logCSV_FILENAME, "w");

        fprintf(file, "Time,Event Code,Entity\n");
        if(file != NULL) {
            for (int i = 0; i < log->ind; ++i) {
                fprintf(file, "%lu,%d,%s\n", log->log[i].timestamp, log->log[i].eventcode, log->log[i].msg);
            }
            fclose(file);
            printf("**********logCSV: DATA GATHERED.**********\n");
        } else {
            printf("logCSV: failed to open log file " logCSV_FILENAME);
        }
    }
}
/* End of debug purposes. */
