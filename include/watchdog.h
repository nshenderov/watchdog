/*******************************************************************************
*
* FILENAME : watchdog.h
*
* DESCRIPTION : Interface of the watchdog.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 10.07.23
* 
*******************************************************************************/

#ifndef __NSRD_WATCHDOG_H__
#define __NSRD_WATCHDOG_H__

#include <stddef.h>
#include <semaphore.h>
#include <pthread.h>

#ifndef NDEBUG
#define PATH_TO_WATCHDOG ("../watchdog_dbg.out")
#else
#define PATH_TO_WATCHDOG ("../watchdog.out")
#endif

/* 
DESCRIPTION
	Starts a background process named "watchdog" that will watch over user's 
	program to make sure it doesn't crash unexpectedly. It the program crashes 
	the watchdog will restart it. The program is watched from the moment the 
	function returns success status.
	The provided executable file watchdog.out should be placed according to the
    PATH_TO_WATCHDOG, and the libwatchdog.so should be placed next to the
    watchdog.out either in the ./bin directory or in the same directory.
RETURN	
	0 - on success
	1 - on failure to start the watchdog.
INPUT
	argc - number of the command line arguments. This is the main function 
	parameter that should be passed to WDStart.
	argv - array of strings with command line arguments. This is the main 
	function parameter that should be passed to WDStart.
	downtime - the time interval in seconds for which the system is allowed
	to not respond. The value should be no less than 5 seconds.
*/
int WDStart(int argc, char *argv[], size_t downtime);

/*
DESCRIPTION
	Stops the watchdog and frees resources. It should not be run if WDStart
	returned 1 (failed to start ther watchdog) because all the resourses
	will be cleaned up on failure.
RETURN	
	There is no return for this function.
INPUT
	There is no input for this function.
*/
void WDStop(void);

#endif /* __NSRD_WATCHDOG_H__ */
