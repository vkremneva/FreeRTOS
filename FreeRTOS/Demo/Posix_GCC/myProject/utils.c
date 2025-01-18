#include "utils.h"

bool xCheckPdPASS( BaseType_t xStatus, char *pTaskName, char *pMsg ) {
    if (xStatus == pdPASS) {
        //printf( "%s: %s SUCCESS\n", pTaskName, pMsg );
        return true;
    } else {
        printf( "\n\n%s: %s FAILURE\n\n", pTaskName, pMsg );
        return false;
    }
}