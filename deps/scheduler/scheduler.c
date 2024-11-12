/*******************************************************************************
*
* FILENAME : scheduler.c
*
* DESCRIPTION : Scheduler implementation.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 09.05.2023
* 
*******************************************************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include <time.h> /* time_t */
#include <unistd.h> /* sleep */

#include "scheduler.h"
#include "task.h"
#include "pqueue.h"

struct scheduler
{
    pq_t *pq;
    task_t *curr_running_task;
    int is_running;
    int remove_current_task;
};

enum {FALSE, TRUE};

enum {SUCCESS_OUT, FAIL_OUT};      

static scheduler_run_status_t TaskExecutionHandler(scheduler_t *scheduler);
static void FailureHandler(scheduler_t *scheduler);
static void CompleteHandler(scheduler_t *scheduler);
static int RescheduleHandler(scheduler_t *scheduler);

static int WaitUntill(time_t task_time);

scheduler_t *SchedulerCreate(void)
{
    pq_t *new_pq = NULL;

    scheduler_t *new_scheduler = (scheduler_t *) malloc(sizeof(scheduler_t));
    if (NULL == new_scheduler)
    {
        return (NULL);
    }

    new_pq = PQCreate(TaskCompare);
    if (NULL == new_pq)
    {
        free(new_scheduler);
        new_scheduler = NULL;

        return (NULL);
    }

    new_scheduler->is_running = FALSE;
    new_scheduler->remove_current_task = FALSE;
    new_scheduler->curr_running_task = NULL;
    new_scheduler->pq = new_pq;

    return (new_scheduler);
}

void SchedulerDestroy(scheduler_t *scheduler)
{
    assert(NULL != scheduler);

    SchedulerClear(scheduler);

    PQDestroy(scheduler->pq);
    scheduler->pq = NULL;

    free(scheduler);
    scheduler = NULL;
}

nsrd_uid_t SchedulerAddTask(scheduler_t *scheduler, 
                            int (*action)(void *params), 
                            void (*cleanup)(void *params), 
                            void *action_params, void *cleanup_params,
                            size_t interval_seconds)
{
    task_t *new_task = NULL;

    task_action_t t_action = NULL;
    task_clean_func_t t_cleanup = NULL;

    assert(NULL != scheduler);
    assert(NULL != action);
    assert(NULL != cleanup);

    t_action = (task_action_t)action;
    t_cleanup = (task_clean_func_t)cleanup; 


    new_task = TaskCreate(t_action, t_cleanup, action_params, cleanup_params,
                                                        interval_seconds);
    if(NULL == new_task)
    {
        return (BadUID);
    }

    if(FAIL_OUT == PQEnqueue(scheduler->pq, new_task))
    {
        TaskDestroy(new_task);
        new_task = NULL;

        return (BadUID);
    }

    return (TaskGetUID(new_task));
}

int SchedulerRemoveTask(scheduler_t *scheduler, nsrd_uid_t uid)
{
    task_t *task = NULL;

    assert(NULL != scheduler);

    if (NULL != scheduler->curr_running_task && TaskIsSame(scheduler->curr_running_task, &uid))
    {
        scheduler->remove_current_task = TRUE;
        return (FAIL_OUT);
    }

    task = PQErase(scheduler->pq,(pqueue_is_match_func_t) TaskIsSame, &uid);
    if(NULL == task)
    {
        return (FAIL_OUT);
    }
    TaskDestroy(task);
    task = NULL;

    return (SUCCESS_OUT);
}


int SchedulerRun(scheduler_t *scheduler)
{
    scheduler_run_status_t exit_status = SUCCESS;

    assert(NULL != scheduler);

    scheduler->is_running = TRUE;

    while(!SchedulerIsEmpty(scheduler) && TRUE == scheduler->is_running)
    {
        task_t *task = PQDequeue(scheduler->pq);
        scheduler->curr_running_task = task;

        if(SUCCESS_OUT != WaitUntill(TaskGetExecutionTime(task)))
        {
            FailureHandler(scheduler);
            exit_status = FAILURE;
            break;
        }


        exit_status = TaskExecutionHandler(scheduler);
        if(FALSE == scheduler->is_running && exit_status == SUCCESS)
        {
            exit_status = STOPPED;
        }
    }

    return (exit_status);
}

void SchedulerStop(scheduler_t *scheduler)
{
    assert(NULL != scheduler);

    scheduler->is_running = FALSE;
}

size_t SchedulerSize(const scheduler_t *scheduler)
{
    assert(NULL != scheduler);

    return (PQSize(scheduler->pq));
}

int SchedulerIsEmpty(const scheduler_t *scheduler)
{
    assert(NULL != scheduler);

    return (PQIsEmpty(scheduler->pq));
}

void SchedulerClear(scheduler_t *scheduler)
{
    pq_t *pq = NULL;

    assert(NULL != scheduler);

    pq = scheduler->pq;

    if(NULL != scheduler->curr_running_task)
    {
        scheduler->remove_current_task = TRUE;
    }

    while(!PQIsEmpty(pq))
    {
        task_t *current_task = (task_t *)PQDequeue(pq);
        TaskDestroy(current_task);
    }
}

static int WaitUntill(time_t task_time)
{
    time_t curr_time = 0;
    double sleep_time = 0.0;

    if (-1 == time(&curr_time))
    {
        return (FAIL_OUT);
    }   

    sleep_time = difftime(task_time, curr_time);

    if (0.0 < sleep_time)
    {
        sleep(sleep_time);
    }

    return (SUCCESS_OUT);
}

static scheduler_run_status_t TaskExecutionHandler(scheduler_t *scheduler)
{
    op_status_t op_status = COMPLETE;
    scheduler_run_status_t scheduler_run_status = SUCCESS;

    assert(NULL != scheduler);

    op_status = TaskExecute(scheduler->curr_running_task);

    if(FAILED == op_status)
    {
        FailureHandler(scheduler);
        scheduler_run_status = FAILURE;
    }
    else if(TRUE == scheduler->remove_current_task || COMPLETE == op_status)
    {
        CompleteHandler(scheduler);
    }
    else if(RESCHEDULE == op_status)
    {
        if(FAIL_OUT == RescheduleHandler(scheduler))
        {
            FailureHandler(scheduler);
            scheduler_run_status = FAILURE;
        }
    }

    return (scheduler_run_status);
}

static void FailureHandler(scheduler_t *scheduler)
{
    assert(NULL != scheduler);

    TaskDestroy(scheduler->curr_running_task);
    scheduler->curr_running_task = NULL;
    SchedulerStop(scheduler);
}

static void CompleteHandler(scheduler_t *scheduler)
{
    assert(NULL != scheduler);

    scheduler->remove_current_task = FALSE;
    TaskDestroy(scheduler->curr_running_task);
    scheduler->curr_running_task = NULL;
}

static int RescheduleHandler(scheduler_t *scheduler)
{
    task_t *task = NULL;

    assert(NULL != scheduler);

    task = scheduler->curr_running_task;

    if(SUCCESS_OUT != TaskUpdateExecTime(task))
    {
        return(FAIL_OUT);
    }

    if(SUCCESS_OUT != PQEnqueue(scheduler->pq, task))
    {
        return (FAIL_OUT);
    }

    scheduler->curr_running_task = NULL;
    return (SUCCESS_OUT);
}
