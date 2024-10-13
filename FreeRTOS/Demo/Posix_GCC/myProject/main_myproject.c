#include <stdio.h>
#include <unistd.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

void main_myproject(void)
{

    while (1)
    {
        printf("Hello, world from main_myproject \n");
        sleep(1);
    }
}
