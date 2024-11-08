/*******************************************************************************
*
* FILENAME : watchdog_test.c
*
* DESCRIPTION : Test of the watchdog.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 10.07.23
* 
*******************************************************************************/

#include <time.h> /* time */
#include <stdio.h> /* printf */

#include "watchdog.h" /* watchdog */

#define DOWNTIME (5)
#define TIMEOUT (20)
#define PRINT_INTEVAL (1)

static void WaitFor(unsigned int secs)
{
    unsigned int retTime = time(0) + secs;
    while (time(0) < retTime);
}

int main(int argc, char *argv[])
{
    time_t end_of_test_time = time(NULL) + TIMEOUT;
    int i = 0;

    if (0 != WDStart(argc, argv, DOWNTIME))
    {
        printf("Couldn't start the watchdog\n");
        return 0;
    }

    while(time(NULL) < end_of_test_time)
    {
        printf("[WATCHDOG_TEST] Running.. %d\n", i++);
        WaitFor(PRINT_INTEVAL);
    };

    WDStop();

	return (0);
}