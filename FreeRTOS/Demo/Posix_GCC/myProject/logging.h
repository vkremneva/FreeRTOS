#ifndef LOGGING_H
#define LOGGING_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"

#include <stdatomic.h>

#define logCSV_BUFFER_SIZE 10240
#define logCSV_MSG_SIZE 64
#define logCSV_UPTIME 10000
#define logCSV_FILENAME "./myProject/log.csv"

typedef struct {
  TickType_t timestamp;
  int16_t eventcode;
  char msg[logCSV_MSG_SIZE];
} log_entry_t;

struct logCSV {
  atomic_int ind;
  log_entry_t log[logCSV_BUFFER_SIZE];
};

void addToCSVLog(struct logCSV *log, TickType_t timestamp, int16_t eventcode, char* msg);

#define logBUFFER_SIZE 256
#define logDEPT_NAME_MAXLEN 50

void logDepartmentUsage( char *pcDepartmentName, UBaseType_t ulDepartmentID, TickType_t xUseTime );
void logNoResourceAvailable( char *pcDepartmentName );

void logQueueSendError(char *pcQueueName);

#endif