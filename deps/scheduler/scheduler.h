/*******************************************************************************
*
* FILENAME : scheduler.h
*
* DESCRIPTION : Interface of a scheduler data structure.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 09.05.2023
* 
*******************************************************************************/

#ifndef __NSRD_SCHEDULER_H__
#define __NSRD_SCHEDULER_H__

#include <stddef.h> /* size_t */

#include "uid.h" /* nsrd_uid_t */

typedef struct scheduler scheduler_t;

typedef enum scheduler_run_status
{
SUCCESS,
FAILURE,
STOPPED
} scheduler_run_status_t;


/*
DESCRIPTION
    Creates new scheduler. It will sort and execute tasks, based on time.
    Creation may fail, due to memory allocation fail.
    User is responsible for destroying schedulers.
RETURN
    Pointer to the created scheduler on success.
    NULL if allocation failed.
INPUT
    Doesn't receive anything.
TIME COMPLEXITY:
	O(1)
*/
scheduler_t *SchedulerCreate(void);

/*
DESCRIPTION
    Frees the memory allocated for each task of scheduler and
    the scheduler by itself.
    All tasks and data inside the scheduler will be lost.
    User is responsible for handling dangling pointers after destroy.
RETURN
    Doesn't return anything.
INPUT
    scheduler: a pointer to a scheduler.
TIME COMPLEXITY:
	O(1)
*/
void SchedulerDestroy(scheduler_t *scheduler);

/*
DESCRIPTION
    Function responsible for adding a new task to the scheduler.
    Each task has a generated unique identifier nsrd_uid_t.
    If the addition of the task fails for any reason, the function returns a
    special BadUID value, indicating that the task was not added to the queue.
RETURN
    Task's unique identifier nsrd_uid_t if success.
    BadUID if fail.
INPUT
    scheduler: a pointer to a scheduler.
    action: pointer to user's action function.
    cleanup: pointer to user's cleanup function.
    action_params: pointer to users params for action.
    cleanup_params: pointer to users params for cleanup.
    interval_seconds: how many seconds before execute(s) after creating task.
        Also reschedule action() task if needed.
TIME COMPLEXITY:
	O(n)
*/
nsrd_uid_t SchedulerAddTask(scheduler_t *scheduler, 
                            int (*action)(void *params), 
                            void (*cleanup)(void *params), 
                            void *action_params, void *cleanup_params,
                            size_t interval_seconds);

/*
DESCRIPTION
    Removes the task, represented by uid from a scheduler, from a scheduler.
    Remove function can fail if the element wasn't found in scheduler.
    User is able to create task that will remove itself. The removal
    will be applied after the end of the task.
RETURN
    0 - if remove was successful.
    1 - if function failed.
INPUT
    scheduler: a pointer to a scheduler.
    uid - unique identifier representing task to remove.
TIME COMPLEXITY:
	O(n)
*/
int SchedulerRemoveTask(scheduler_t *scheduler, nsrd_uid_t uid);

/*
DESCRIPTION
    SchedulerRun is the heart of the scheduler. 
    It runs the scheduler and executes all the tasks that are currently
    in the scheduler.
    It  repeatedly extracting tasks and executing them, one by one,
    until the queue is empty or the scheduler is stopped.
    Each task is executed in its own time slot, based on its predefined
    execution time. If a task executes successfully or fails, it is destroyed
    and removed from the queue. If a task needs to be rescheduled, it is
    updated accordingly to the set time interval_seconds.
RETURN
    Returns int according to scheduler_run_status_t.
    0 - SUCCESS. Scheduler completed all tasks.
    1 - FAILURE. One of the tasks failed or failure inside scheduler.
    2 - STOPPED. Scheduler stopped by user.
INPUT
    scheduler: a pointer to a scheduler.
TIME COMPLEXITY:
	O(n)
*/
int SchedulerRun(scheduler_t *scheduler);

/*
DESCRIPTION
    Stops current running scheduler.
RETURN
    Doesn't return anything.
INPUT
    scheduler: a pointer to a scheduler.
TIME COMPLEXITY:
	O(1)
*/
void SchedulerStop(scheduler_t *scheduler);

/*
DESCRIPTION
    Traverses the scheduler and returns the amount of tasks.
RETURN
    Number of tasks in scheduler.
INPUT
    scheduler: a pointer to a scheduler.
TIME COMPLEXITY:
	O(n)
*/
size_t SchedulerSize(const scheduler_t *scheduler);
/*
DESCRIPTION
    Checks if scheduler is empty.
RETURN
    1 (true) - if scheduler is empty.
    0 (false) - if it's not.
INPUT
    scheduler: a pointer to a scheduler.
TIME COMPLEXITY:
	O(1)
*/
int SchedulerIsEmpty(const scheduler_t *scheduler);

/*
DESCRIPTION
    Removes all tasks from a scheduler.
RETURN
    Doesn't return anything.
INPUT
    scheduler: a pointer to a scheduler.
TIME COMPLEXITY:
	O(n)
*/
void SchedulerClear(scheduler_t *scheduler);


#endif /* __NSRD_SCHEDULER_H__ */