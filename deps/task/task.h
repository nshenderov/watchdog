/*******************************************************************************
*
* FILENAME : task.h
*
* DESCRIPTION : Interface of a task data structure.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 7.05.2023
* 
*******************************************************************************/

#ifndef __NSRD_TASK_H__
#define __NSRD_TASK_H__

#include <time.h> /* time_t */

#include "uid.h"

typedef struct task task_t;

typedef enum op_status {COMPLETE, RESCHEDULE, FAILED} op_status_t;

/* 
DESCRIPTION
	A type of function that executes an operation and returns its status and 
	if it needs to be rescheduled. The function and its parameters are defined 
	by the user. 
RETURN
	0 - execution succesful and no rescheduling needed;
	1 - execution succesful and needs to be rescheduled;
	2 - execution failed.
INPUT
	operation_params - a generic pointer to function parameters.
*/
typedef op_status_t (*task_action_t)(void *operation_params);

/* 
DESCRIPTION
	Type of function that cleans up after other operations. The cleanup and \
	its parameters are defined by the user 
RETURN
	There is no return for this function tyope
INPUT
	cleanup_params - a generic pointer to function parameters.
*/                                            
typedef void (*task_clean_func_t)(void *cleanup_params); 

/* 
DESCRIPTION
	Creates task and allocates memory. The task will contain all the data 
	passed via input, a UID and an execution time. 
	User is responsible for destroying tasks using TaskDestroy. 
	The parameter "execution_time" of the task will be calculated as a sum of
	the time of creation and passed interval. 
	Creation may fail.
RETURN
	A pointer to the task - if success;
	NULL - if failure.
INPUT
	action - a pointer to the operation constituting in the task;
	clean_up - a pointer to the function that cleanups after the operation.
	params - parameters for the action function.
	cleanup_params - parameters for the clean_up function.
	interval_seconds - interval of rescheduling the task.
*/  
task_t *TaskCreate(task_action_t action, task_clean_func_t clean_up, 
				void *params, void *cleanup_params,  size_t interval_seconds);

/* 
DESCRIPTION
	Destroys a task by deallocating memory and running the cleanup function
	with provided parameters. 
RETURN
	There is no return for this function.
INPUT
	task - a pointer to the task.
*/
void TaskDestroy(task_t *task);

/* 
DESCRIPTION
	Runs the execute function using its parameters. Returns status if 
	the function needs to be rescheduled nd if it was succesful.
RETURN
	0 - execution succesful and no rescheduling needed;
	1 - execution succesful and needs to be rescheduled;
	2 - execution failed.
INPUT
	task - a pointer to the task.
*/
/* return action status */ 
op_status_t TaskExecute(task_t *task); 

/* 
DESCRIPTION
	Compares execution time of two tasks.
RETURN
	-1 - if task1 is scheduled earlier than task2;
	0 - if tasks are scheduled on the same time;
	1 - if task1 is scheduled later than task2. 
INPUT
	task1 - a pointer to the task;
	task2 - a pointer to the task.
*/
int TaskCompare(const void *task1, const void *task2); 

/* 
DESCRIPTION
	Copares UIDs of two tasks. 
RETURN
	1 - UIDs are the same;
	0 - UIDs are not the same. 
INPUT
	task1 - a pointer to the task;
	task2 - a pointer to the task.
*/
int TaskIsSame(const void *task, void *uid);

/* 
DESCRIPTION
	Returns the UID of a task.
RETURN
	UID of a task by value.
INPUT
	task1 - a pointer to the task;
	task2 - a pointer to the task.
*/
nsrd_uid_t TaskGetUID(const task_t *task);

/* 
DESCRIPTION
	Returns the scheduled execution time of a task.
RETURN
	Execution time in seconds since UNIX Epoch.
INPUT
	task - a pointer to the task.
*/
time_t TaskGetExecutionTime(const task_t *task);

/* 
DESCRIPTION
	Changes the execution time of a task by the interval in seconds,
	stored in the task.
RETURN
	Execution time in seconds since UNIX Epoch.
INPUT
	task - a pointer to the task.
*/
int TaskUpdateExecTime(task_t *task);

#endif /* __NSRD_TASK_H__ */

