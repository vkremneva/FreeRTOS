#include "include/logging.h"

QueueHandle_t xQueueLogging;

void vMakeLogEntry( BaseType_t uxType,
                    TickType_t xTimestamp,
                    uint8_t ucEventCode,
                    char * pcEntityName )
{
    BaseType_t xStatusSend = 0;
    log_entry_t xNewEntry = { 0, 0, 0, "" };

    xNewEntry.uxType = uxType;
    xNewEntry.xTimestamp = xTimestamp;
    xNewEntry.ucEventCode = ucEventCode;

    if( pcEntityName != NULL )
    {
        strncpy( xNewEntry.ucEntityName, pcEntityName, logCSV_MSG_SIZE - 1 );
        xNewEntry.ucEntityName[ logCSV_MSG_SIZE - 1 ] = '\0';
    }

    xStatusSend = xQueueSend( xQueueLogging, &xNewEntry, portMAX_DELAY );

    if( xStatusSend == errQUEUE_FULL )
    {
    }
}
/*-----------------------------------------------------------*/

void vLoggingTask( void * pvParameters )
{
    QueueHandle_t xQueueLogging = ( QueueHandle_t ) pvParameters;

    BaseType_t xStatusReceive = 0;

    static log_t xLog;

    xLog.usInd = 0;
    log_entry_t xLogEntry = { 0, 0, 0, "" };

    for( ;; )
    {
        xStatusReceive = xQueueReceive( xQueueLogging, &xLogEntry, portMAX_DELAY );

        if( xStatusReceive == pdPASS )
        {
            switch( xLogEntry.uxType )
            {
                case logERR_QUEUE_SEND:
                    printf( "ERROR: Failed to send to %s Queue, it is full.\n", xLogEntry.ucEntityName );
                    break;

                case logERR_DISP_EVENT_REJECTED_MAX:
                    printf( "FAILURE: Event from %s ", xLogEntry.ucEntityName );
                    printf( "was rejected %lu times ", eventsCOUNTER_REJECTED_MAX );
                    printf( "and now is removed from the queue. \n" );
                    break;

                case logERR_DEPT_N0_RESOURCE_AVAILABLE:
                    printf( "ERROR: No resources available for %s. ", xLogEntry.ucEntityName );
                    printf( "The event is remaining in the queue for further handling.\n" );
                    break;

                case logDEPT_USAGE:
                    printf( "%s: Received the ", xLogEntry.ucEntityName );
                    printf( "event code %d. ", xLogEntry.ucEventCode );
                    printf( "A free resource was allocated. " );
                    printf( "Task lasted %lu. The task was completed.\n", xLogEntry.xTimestamp );
                    break;

                case logCURRENT_STATE:
                    xLog.xBody[ xLog.usInd ].ucEventCode = xLogEntry.ucEventCode;
                    xLog.xBody[ xLog.usInd ].uxType = xLogEntry.uxType;
                    xLog.xBody[ xLog.usInd ].xTimestamp = xLogEntry.xTimestamp;

                    strncpy( xLog.xBody[ xLog.usInd ].ucEntityName, xLogEntry.ucEntityName, logCSV_MSG_SIZE - 1 );
                    xLog.xBody[ xLog.usInd ].ucEntityName[ logCSV_MSG_SIZE - 1 ] = '\0';

                    xLog.usInd++;

                    break;

                default:
            }

            if( xLogEntry.xTimestamp >= logCSV_UPTIME )
            {
                FILE * file;
                file = fopen( logCSV_FILENAME, "w" );

                if( file != NULL )
                {
                    fprintf( file, "Time,Event Code,Entity\n" );

                    for(int i = 0; i < xLog.usInd; ++i)
                    {
                        fprintf( file, "%lu,%d,%s\n", xLog.xBody[ i ].xTimestamp, xLog.xBody[ i ].ucEventCode, xLog.xBody[ i ].ucEntityName );
                    }

                    fclose( file );
                    printf( "**********logCSV: DATA GATHERED.**********\n" );
                }
                else
                {
                    printf( "logCSV: failed to open log file " logCSV_FILENAME );
                }
            }
        }
    }
}
/*-----------------------------------------------------------*/


void vLogNoResourceAvailable( char * pcDepartmentName )
{
    vMakeLogEntry( logERR_DEPT_N0_RESOURCE_AVAILABLE, 0, 0, pcDepartmentName );
}
/*-----------------------------------------------------------*/

void vLogQueueSendError( char * pcQueueName )
{
    vMakeLogEntry( logERR_QUEUE_SEND, 0, 0, pcQueueName );
}
/*-----------------------------------------------------------*/

void vLogEventRejectedMax( event_t xEvent,
                           char * psDepartmentName )
{
    vMakeLogEntry( logERR_DISP_EVENT_REJECTED_MAX, 0, 0, psDepartmentName );
}
/*-----------------------------------------------------------*/

void vLogDepartmentUsage( TickType_t xTimestamp,
                          uint8_t ucEventCode,
                          char * psDepartmentName )
{
    vMakeLogEntry( logDEPT_USAGE, xTimestamp, ucEventCode, psDepartmentName );
}
/*-----------------------------------------------------------*/

void vLogCurrentState( TickType_t xTimestamp,
                       uint8_t ucEventCode,
                       char * pcEntityName )
{
    vMakeLogEntry( logCURRENT_STATE, xTimestamp, ucEventCode, pcEntityName );
}
/*-----------------------------------------------------------*/
