/*******************************************************************************
*
* FILENAME : watchdog.c
*
* DESCRIPTION : Implementation of the watchdog.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 10.07.23
* 
*******************************************************************************/

#define _POSIX_C_SOURCE 200809L

#include <assert.h> /* assert */
#include <stdio.h> /* sprintf */
#include <stdlib.h> /* exit */
#include <signal.h> /* signal */
#include <semaphore.h> /* semaphores */
#include <fcntl.h> /* O_CREAT */
#include <pthread.h> /* threads */
#include <unistd.h> /* getpgid */
#include <sys/types.h> /* pid_t */
#include <sys/shm.h> /* key_t, ftok */

#include "scheduler.h"
#include "watchdog.h"

#define MAX_ARGS_AMOUNT (256)
#define CLOSE_ATTEMPTS_AMOUNT (5)
#define KICKTIME_FREQUENCY (5)

enum {WD_NEG_FAILURE = -1, WD_SUCCESS, WD_FAILURE};
enum {WD_COMPLETE, WD_RESCHEDULE};
enum {FALSE, TRUE};
typedef struct wdparams
{
    int wd_sig_is_received;
    int wd_sig_stop_is_received;
    int wd_argc;
    size_t kicktime;
    size_t downtime;
    pthread_t id_thread;
    pid_t observed_pid;
    scheduler_t *scheduler;
    sem_t *sem_thread;
    sem_t *sem_process;
    char sem_thread_name[MAX_ARGS_AMOUNT];
    char sem_process_name[MAX_ARGS_AMOUNT];
    char *wd_argv[MAX_ARGS_AMOUNT];
} wdparams_t;


static int WDInitSemophores(void);
static void WDGraceExit(void);
static int WDInitSigHandlers(void);
static void *WDThread(void *argv);
static int WDInitScheduler(void);
static void WDInitParameters(int argc, char *argv[], size_t downtime);
static int WDSetAppParams(void);
static void WDSetWdParams(void);
static int WDSyncThreads(sem_t *posted_sem, sem_t *waited_sem);
static int WDSyncApp(void);
static void WDWaitSeconds(size_t seconds);
static void HandleKick(int sig);
static int TaskKick(void *operation_params);
static void TaskCleanupDummy(void *cleanup_params);
static int TaskReboot(void *argv);


const int __attribute__((weak)) g_is_wd;
wdparams_t g_wd_params;


int WDStart(int argc, char *argv[], size_t downtime)
{
    assert(NULL != argv[0]);
    assert(0 < argc);
    assert(5 <= downtime);

    WDInitParameters(argc, argv, downtime);

    if (WDInitScheduler() || WDInitSigHandlers() || WDInitSemophores())
    {
        return (WD_FAILURE);
    }
    
    if (!g_is_wd)
    {
        if(WDSetAppParams())
        {
            return (WD_FAILURE);
        }

        if (WD_SUCCESS != pthread_create(&g_wd_params.id_thread, NULL, WDThread,
                                         NULL))
        {
            return (WD_FAILURE);
        }
    }
    else
    {
        atexit(WDGraceExit);

        WDSetWdParams();

        WDSyncThreads(g_wd_params.sem_process, g_wd_params.sem_thread);

        SchedulerRun(g_wd_params.scheduler);

        kill(g_wd_params.observed_pid, SIGUSR2);
    }
    
    WDSyncApp();

    return (WD_SUCCESS);
}

void WDStop(void)
{
    int i = 0;

    SchedulerStop(g_wd_params.scheduler);

    while (!g_wd_params.wd_sig_stop_is_received && CLOSE_ATTEMPTS_AMOUNT > i++)
    {
        kill(g_wd_params.observed_pid, SIGUSR2);
        WDWaitSeconds(g_wd_params.downtime);
    }
    
    pthread_join(g_wd_params.id_thread, NULL);

    WDGraceExit();

    sem_unlink(g_wd_params.sem_process_name);
    sem_unlink(g_wd_params.sem_thread_name);
}

static void *WDThread(void *argv)
{
    WDWaitSeconds(g_wd_params.kicktime * 2);

	TaskReboot(NULL);

    SchedulerRun(g_wd_params.scheduler);
    
    return (NULL);
    (void) argv;
}

static int TaskReboot(void *argv)
{
    pid_t pid = 0;

    if(g_wd_params.wd_sig_stop_is_received)
    {
        SchedulerStop(g_wd_params.scheduler);

        return (WD_COMPLETE);
    }

    if(!g_wd_params.wd_sig_is_received)
    {
        pid = fork();
        if (WD_NEG_FAILURE == pid)
        {
            exit(WD_FAILURE);
        }

        if (0 == pid)
        {
            if (WD_NEG_FAILURE == execvp(g_wd_params.wd_argv[0],
                                         g_wd_params.wd_argv))
            {
                exit(WD_FAILURE);
            }
        }
        else
        {
            g_wd_params.observed_pid = pid;

            WDSyncThreads(g_wd_params.sem_thread, g_wd_params.sem_process);
        }
    }
    else
    {
        g_wd_params.wd_sig_is_received = FALSE;
    }
    
    return (WD_RESCHEDULE);
    (void) argv;
}

static int WDInitScheduler(void)
{
    nsrd_uid_t uid_kick = BadUID;
	nsrd_uid_t uid_reboot = BadUID;

    scheduler_t * scheduler = SchedulerCreate();
    if (NULL == scheduler)
	{
		return (WD_FAILURE);
	}

    g_wd_params.scheduler = scheduler;

    uid_kick = SchedulerAddTask(scheduler, TaskKick, TaskCleanupDummy, NULL,
                                NULL, g_wd_params.kicktime);
    if (UIDIsSame(uid_kick, BadUID))
    {
        return (WD_FAILURE);
    }

    uid_reboot = SchedulerAddTask(scheduler, TaskReboot, TaskCleanupDummy, NULL,
                                  NULL, g_wd_params.downtime);
    if (UIDIsSame(uid_reboot, BadUID))
    {
        return (WD_FAILURE);
    }

    return (WD_SUCCESS);
}

static int TaskKick(void *argv)
{
    assert(NULL != &g_wd_params);
 
    kill(g_wd_params.observed_pid, SIGUSR1);

	return (WD_RESCHEDULE);
    (void) argv;
}

static void TaskCleanupDummy(void *cleanup_params)
{
    (void) cleanup_params;
}

static void HandleKick(int sig)
{
    g_wd_params.wd_sig_is_received = TRUE;

    (void) sig;
}

static void HandleStop(int sig)
{
    g_wd_params.wd_sig_stop_is_received = TRUE;

    (void) sig;
}

static int WDInitSigHandlers(void)
{
    struct sigaction act = {0};

    act.sa_handler = HandleKick;

    if (WD_NEG_FAILURE == sigaction(SIGUSR1, &act, NULL))
    {
        return (WD_FAILURE);
    }

    act.sa_handler = HandleStop;

    if (WD_NEG_FAILURE == sigaction(SIGUSR2, &act, NULL))
    {
        return (WD_FAILURE);
    }

    return (WD_SUCCESS);
}

static int WDInitSemophores(void)
{
    char *sem_thread_name = g_wd_params.sem_thread_name;
    char *sem_process_name = g_wd_params.sem_process_name;
    sem_t *sem_thread = NULL;
    sem_t *sem_process = NULL;
    char *str = NULL;
    key_t key = 0;

    if (!g_is_wd)
    {
        str = g_wd_params.wd_argv[0];
    }
    else
    {
        str = g_wd_params.wd_argv[2];
    }

    key = ftok(str, getpgid(getpid()));

    if (WD_NEG_FAILURE == key 
     || WD_NEG_FAILURE == sprintf(sem_thread_name, "%d", key + 1) 
     || WD_NEG_FAILURE == sprintf(sem_process_name, "%d", key + 2))
    {
        return (WD_FAILURE);
    }
    
    sem_thread = sem_open(sem_thread_name, O_CREAT, 0666, 0);
    sem_process = sem_open(sem_process_name, O_CREAT, 0666, 0);
    
    if (SEM_FAILED == sem_thread || SEM_FAILED == sem_process)
    {
        return (WD_FAILURE);
    }

    g_wd_params.sem_thread = sem_thread;
    g_wd_params.sem_process = sem_process;

    return (WD_SUCCESS);
}

static void WDGraceExit(void)
{
    SchedulerStop(g_wd_params.scheduler);
    WDWaitSeconds(g_wd_params.downtime);

    SchedulerClear(g_wd_params.scheduler);
    SchedulerDestroy(g_wd_params.scheduler);

    sem_close(g_wd_params.sem_process);
    sem_close(g_wd_params.sem_thread);
}

static int WDSyncThreads(sem_t *posted_sem, sem_t *waited_sem)
{
    assert(NULL != posted_sem);
    assert(NULL != waited_sem);

	return (sem_post(posted_sem)||sem_wait(waited_sem)||sem_post(waited_sem));
}

static int WDSyncApp(void)
{
    sem_t *sem_thread = g_wd_params.sem_thread;
    sem_t *sem_process = g_wd_params.sem_process;

	return (sem_wait(sem_thread)||sem_post(sem_thread)||
           sem_wait(sem_process)||sem_post(sem_process));
}

static void WDInitParameters(int argc, char *argv[], size_t downtime)
{
	int i = 0;
    char **wd_argv = NULL;

    assert(NULL != argv);
    assert(5 <= downtime);

    g_wd_params.wd_argc = argc;

    wd_argv = g_wd_params.wd_argv;

	for (i = 0; i < argc; ++i)
	{
		wd_argv[i] = argv[i];
	}

    g_wd_params.downtime = downtime;
    g_wd_params.kicktime = downtime / KICKTIME_FREQUENCY;

    g_wd_params.wd_sig_is_received = 0;
    g_wd_params.wd_sig_stop_is_received = 0;

    g_wd_params.observed_pid = getppid();
}

static int WDSetAppParams(void)
{
	int i = 0;
    int wd_argc = g_wd_params.wd_argc;
    char **wd_argv = g_wd_params.wd_argv;
    size_t downtime = g_wd_params.downtime;
	static char downtime_str[20] = {0};

    if (WD_NEG_FAILURE == sprintf(downtime_str, "%ld", downtime))
    {
        return (WD_FAILURE);
    }

	for (i = wd_argc + 1; 1 < i; --i)
	{
		wd_argv[i] = wd_argv[i - 2];
	}

	wd_argv[0] = PATH_TO_WATCHDOG;
	wd_argc += 2;
	wd_argv[1] = downtime_str;
	wd_argv[wd_argc] = NULL;

    return (WD_SUCCESS);
}

static void WDSetWdParams(void)
{
	int i = 0;
    int wd_argc = g_wd_params.wd_argc;
    char **wd_argv = g_wd_params.wd_argv;

	for (i = 0; i < wd_argc; ++i)
	{
		wd_argv[i] = wd_argv[i + 2];
	}

	wd_argc -= 2;
}

static void WDWaitSeconds(size_t seconds)
{
	while (0 < seconds)
	{		
		seconds = sleep(seconds);
	}	
}