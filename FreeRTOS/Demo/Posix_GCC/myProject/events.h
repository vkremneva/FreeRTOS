#ifndef EVENTS_H
#define EVENTS_H

#include <time.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

// TODO not so sure about 10
#define eventsQUEUE_SIZE 10
#define eventsQUEUE_ITEM_SIZE (sizeof(int16_t)) 
#define eventsPRIORITY 3

void vEventGeneratorTask(void *param);

#endif
