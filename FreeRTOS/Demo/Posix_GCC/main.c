#include "main_config.h"

/*
 * Signal handler for Ctrl_C to cause the program to exit, and generate the
 * profiling info.
 */
static void handle_sigint(int signal);

#define BUILD BUILD_DIR

extern void main_myproject(void);

int main(void)
{
    /* SIGINT is not blocked by the posix port */
    signal(SIGINT, handle_sigint);

    /* Initialise the trace recorder.  Use of the trace recorder is optional.
     * See http://www.FreeRTOS.org/trace for more information. */
    vTraceEnable(TRC_START);

    console_init();

    main_myproject();

    return 0;
}
/*-----------------------------------------------------------*/

void handle_sigint(int signal)
{
    int xReturn;

    xReturn = chdir(BUILD); /* changing dir to place gmon.out inside build */

    if (xReturn == -1)
    {
        printf("chdir into %s error is %d\n", BUILD, errno);
    }

    exit(2);
}