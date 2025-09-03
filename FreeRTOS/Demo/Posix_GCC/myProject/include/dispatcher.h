/**
 * @file dispatcher.h
 * @brief Dispatcher module.
 *
 * Responsible for dispatching events from the global event queue
 * to appropriate departments.
 */

#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "events.h"
#include "department.h"
#include "logging.h"

#define dispPRIORITY    3
#define dispDELAY       ( pdMS_TO_TICKS( 1000 ))

/**
 * @brief FreeRTOS dispatcher task.
 *
 * Fetches events from the event queue and routes them
 * to the correct department based on priority and availability.
 *
 * @param pvParameters Pointer to department_d descriptor of all departments.
 */
void vDispatcherTask( void * pvParameters );

#endif /* ifndef DISPATCHER_H */
