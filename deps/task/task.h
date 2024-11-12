/*******************************************************************************
*
* FILENAME : task.h
*
* DESCRIPTION : Task refers to a specific unit of work that needs to be executed
* by the computer system. The task is a tool for scheduler to manage and
* prioritize the execution of various operations or processes running on a
* computer system. The scheduler decides which tasks to execute, when to execute
* them, and for how long they should run.
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
    Pointer to the user's function that executes a task using
    the operation_params. The actual action and types of the input are
    defined by the user.
RETURN
    COMPLETE: success and no need to reschedule the task.
    RESCHEDULE: success and need to reschedule the task.
    FAILED: fail.
INPUT
    operation_params: pointer to the parameter.
*/
typedef op_status_t (*task_action_t)(void *operation_params);

/*
DESCRIPTION
    Pointer to the user's function that clean up after a task execution
    using the cleanup_params. The actual clean up pattern and types of the
    input are defined by the user.
RETURN
    Doesn't return anything.
INPUT
    cleanup_params: pointer to the parameter.
*/             
typedef void (*task_clean_func_t)(void *cleanup_params); 

/* 
DESCRIPTION
	Creates new task. Creation may fail, due to memory allocation fail.
	User is responsible for deallocating tasks using TaskDestroy. 
	The parameter "execution_time" of the task will be calculated as a sum of
	the time of creation and passed interval.
RETURN
	pointer to the task - if success;
	NULL - if failure.
INPUT
	action: pointer to the operation constituting in the task;
	clean_up: pointer to the function that cleanups after the operation.
	params: pointer to users params for action.
	cleanup_params: pointer to users params for clean_up.
	interval_seconds: interval of rescheduling the task.
*/  
task_t *TaskCreate(task_action_t action, task_clean_func_t clean_up, 
				void *params, void *cleanup_params,  size_t interval_seconds);

/* 
DESCRIPTION
	Destroys the task by deallocating memory and running the cleanup function
	with provided parameters. 
RETURN
	There is no return for this function.
INPUT
	task: pointer to the task.
*/
void TaskDestroy(task_t *task);

/* 
DESCRIPTION
	Runs the execute function using its parameters. Returns status if 
	the function needs to be rescheduled nd if it was succesful.
RETURN
	COMPLETE: execution succesful and no rescheduling needed;
	RESCHEDULE: execution succesful and needs to be rescheduled;
	FAILED: execution failed.
INPUT
	task: pointer to the task.
*/
op_status_t TaskExecute(task_t *task); 

/* 
DESCRIPTION
	Compares execution time of two tasks.
RETURN
	-1: task1 is scheduled earlier than task2;
	0: tasks are scheduled on the same time;
	1: task1 is scheduled later than task2. 
INPUT
	task1: pointer to the task;
	task2: pointer to the task.
*/
int TaskCompare(const void *task1, const void *task2); 

/* 
DESCRIPTION
	Copares UIDs of two tasks. 
RETURN
	1: UIDs are the same;
	0: UIDs are not the same. 
INPUT
	task1: pointer to the task;
	task2: pointer to the task.
*/
int TaskIsSame(const void *task, void *uid);

/* 
DESCRIPTION
	Returns the UID of a task.
RETURN
	UID of a task by value.
INPUT
	task1: pointer to the task;
	task2: pointer to the task.
*/
nsrd_uid_t TaskGetUID(const task_t *task);

/* 
DESCRIPTION
	Returns the scheduled execution time of a task.
RETURN
	Execution time in seconds since UNIX Epoch.
INPUT
	task: pointer to the task.
*/
time_t TaskGetExecutionTime(const task_t *task);

/* 
DESCRIPTION
	Changes the execution time of a task by the interval in seconds,
	stored in the task.
RETURN
	0: success.
	1: fail.
INPUT
	task: pointer to the task.
*/
int TaskUpdateExecTime(task_t *task);

#endif /* __NSRD_TASK_H__ */

