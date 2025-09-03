#ifndef LOGGING_H
#define LOGGING_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"

#include "events.h"

#define logQUEUE_SIZE                        10000
#define logQUEUE_ITEM_SIZE                   ( sizeof( log_entry_t ))

#define logBUFFER_SIZE                       256
#define logDEPT_NAME_MAXLEN                  50
#define logPRIORITY                          3

#define logCSV_BUFFER_SIZE                   10240
#define logCSV_MSG_SIZE                      64
#define logCSV_UPTIME                        100000
#define logCSV_FILENAME                      "./myProject/log.csv"

#define logERR_QUEUE_SEND                    255
#define logERR_DEPT_N0_RESOURCE_AVAILABLE    254
#define logERR_DISP_EVENT_REJECTED_MAX       253
#define logDEPT_USAGE                        252
#define logCURRENT_STATE                     251

typedef struct
{
    BaseType_t    uxType;
    TickType_t    xTimestamp;
    uint8_t       ucEventCode;
    char          ucEntityName[ logCSV_MSG_SIZE ];
} log_entry_t;

typedef struct
{
    uint16_t       usInd;
    log_entry_t    xBody[ logCSV_BUFFER_SIZE ];
} log_t;

extern QueueHandle_t xQueueLogging;

void vLoggingTask( void * pvParameters );

void vLogNoResourceAvailable( char * pcDepartmentName );
void vLogQueueSendError( char * pcQueueName );
void vLogEventRejectedMax( event_t xEvent,
                           char * psDepartmentName );

void vLogDepartmentUsage( TickType_t xTimestamp,
                          uint8_t ucEventCode,
                          char * psDepartmentName );
void vLogCurrentState( TickType_t xTimestamp,
                       uint8_t ucEventCode,
                       char * psDepartmentName );

void vMakeLogEntry( BaseType_t uxType,
                    TickType_t xTimestamp,
                    uint8_t ucEventCode,
                    char * pcEntityName );

#endif /* ifndef LOGGING_H */
