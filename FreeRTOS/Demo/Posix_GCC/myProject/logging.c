#include "logging.h"

bool xCheckPdPASS( BaseType_t xStatus, char *pTaskName, char *pMsg ) {
    if (xStatus == pdPASS) {
        printf("%s: %s SUCCESS\n", pTaskName, pMsg);
        return true;
    } else {
        printf("%s: %s FAILURE\n", pTaskName, pMsg);
        return false;
    }
}