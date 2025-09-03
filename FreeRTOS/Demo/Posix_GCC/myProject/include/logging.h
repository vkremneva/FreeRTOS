/**
 * @file logging.h
 * @brief Logging module.
 *
 * Handles recording of events, errors, and system state
 * into queues and CSV files for later analysis.
 */

#ifndef LOGGING_H
#define LOGGING_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"

#include "events.h"

#define logQUEUE_SIZE          10000
#define logQUEUE_ITEM_SIZE     ( sizeof( log_entry_t ))

#define logBUFFER_SIZE         256
#define logDEPT_NAME_MAXLEN    50
#define logPRIORITY            3

/** @name Parameters for CSV export log file. */
///@{
#define logCSV_BUFFER_SIZE    10240
#define logCSV_MSG_SIZE       64
#define logCSV_UPTIME         100000
#define logCSV_FILENAME       "./myProject/log.csv"
///@}

/** @name Types of log messages. */
///@{
#define logERR_DEPT_N0_RESOURCE_AVAILABLE    254
#define logERR_DISP_EVENT_REJECTED_MAX       253
#define logDEPT_USAGE                        252
#define logCURRENT_STATE                     251
///@}

/**
 * @brief Single log entry descriptor.
 */
typedef struct
{
    BaseType_t    uxType;                          /**< Log type. */
    TickType_t    xTimestamp;                      /**< Log timestamp. */
    uint8_t       ucEventCode;                     /**< Related event code. */
    char          ucEntityName[ logCSV_MSG_SIZE ]; /**< Entity name string. */
} log_entry_t;


/**
 * @brief Buffer of log entries.
 */
typedef struct
{
    uint16_t       usInd;                         /**< Current buffer index. */
    log_entry_t    xBody[ logCSV_BUFFER_SIZE ];   /**< Log entry storage. */
} log_t;

/** @brief Logging queue handle. */
extern QueueHandle_t xQueueLogging;

/**
 * @brief FreeRTOS task for log handling and CSV writing.
 * @param pvParameters Unused for this task.
 */
void vLoggingTask( void * pvParameters );

/**
 * @brief Log an error when no department resource is available.
 * @param pcDepartmentName Department name string.
 */
void vLogNoResourceAvailable( char * pcDepartmentName );

/**
 * @brief Log an error when an event exceeds maximum rejection attempts.
 * @param xEvent Event that was rejected.
 * @param psDepartmentName Department name string.
 */
void vLogEventRejectedMax( event_t xEvent,
                           char * psDepartmentName );

/**
 * @brief Log department resource usage.
 * @param xTimestamp Total event processing time.
 * @param ucEventCode Related event code.
 * @param psDepartmentName Department name string.
 */
void vLogDepartmentUsage( TickType_t xTimestamp,
                          uint8_t ucEventCode,
                          char * psDepartmentName );

/**
 * @brief Log current system state.
 * @param xTimestamp Timestamp.
 * @param ucEventCode Related event code.
 * @param psDepartmentName Department name string.
 */
void vLogCurrentState( TickType_t xTimestamp,
                       uint8_t ucEventCode,
                       char * psDepartmentName );

/**
 * @brief Create a log entry and send it to xQueueLogging.
 * @param uxType Log type/category.
 * @param xTimestamp Timestamp.
 * @param ucEventCode Event code.
 * @param pcEntityName Related entity name.
 */
void vMakeLogEntry( BaseType_t uxType,
                    TickType_t xTimestamp,
                    uint8_t ucEventCode,
                    char * pcEntityName );

#endif /* ifndef LOGGING_H */
