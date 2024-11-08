/*******************************************************************************
*
* FILENAME : wd_runner.c
*
* DESCRIPTION : Implementation of the watchdog process runner.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 10.07.23
* 
*******************************************************************************/
#include <assert.h> /* assert */
#include <stdlib.h> /* atoi */

#include "watchdog.h"

const int g_is_wd = 1;

int main(int argc, char *argv[])
{
    size_t downtime = 0;

    assert(0 < argc);
    assert(NULL != argv[0]);

    downtime = atol(argv[1]);

    WDStart(argc, argv, downtime);

    return (0);
}
