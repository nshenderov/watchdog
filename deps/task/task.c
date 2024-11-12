/*******************************************************************************
*
* FILENAME : task.c
*
* DESCRIPTION : Task implementation.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 7.05.2023
* 
*******************************************************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free, size_t, NULL */

#include "task.h"

struct task
{
    nsrd_uid_t task_id;
    task_action_t action_func;
    task_clean_func_t clean_func;
    void *operation_params;
    void *cleanup_params;   
    time_t execution_time; 
    size_t interval_seconds;   
};

task_t *TaskCreate(task_action_t action, task_clean_func_t clean_up, 
                void *params, void *cleanup_params, size_t interval_seconds)
{
    task_t *new_task = NULL;
    nsrd_uid_t uid;

    assert(NULL != action);
    assert(NULL != clean_up);    

    uid = UIDCreate();
    if (UIDIsSame(uid, BadUID))
    {
        return (NULL);
    }
        
    new_task = (task_t *)malloc(sizeof(struct task));
    if (NULL == new_task)
    {
        return (NULL);
    }
    
    new_task->task_id = uid;

    new_task->action_func = action;
    new_task->clean_func = clean_up;
    new_task->operation_params = params;
    new_task->cleanup_params = cleanup_params;

    new_task->execution_time = uid.timestamp + interval_seconds;
    new_task->interval_seconds = interval_seconds;
    
    return (new_task);   
}                

void TaskDestroy(task_t *task)
{
    assert(NULL != task);
    assert(NULL != task->clean_func);

    task->clean_func(task->cleanup_params);
    
    free(task);
    task = NULL;
}

op_status_t TaskExecute(task_t *task)
{
    op_status_t status = 0;
    
    assert(NULL != task);
    assert(NULL != task->action_func);
    
    status = task->action_func(task->operation_params);

    return (status);
}

int TaskCompare(const void *task1, const void *task2)
{
    double time_difference = 0.0;
    int result = 0;
    task_t *task1_conv = (task_t *)task1;
    task_t *task2_conv = (task_t *)task2;
    
    assert(NULL != task1);
    assert(NULL != task2);
    
    time_difference = difftime(task1_conv->execution_time, 
                                            task2_conv->execution_time);
    
    if (0 > time_difference)
    {
        result = 1;
    }
    else if (0 < time_difference)
    {
        result = -1;
    }
    else
    {
        result = 0;
    }
    
    return (result);
}

int TaskIsSame(const void *task, void *uid)
{
    assert(NULL != task);
    
    return (UIDIsSame(TaskGetUID((task_t *)task), *(nsrd_uid_t *)uid));
}

nsrd_uid_t TaskGetUID(const task_t *task)
{
    assert(NULL != task);
    
    return (task->task_id);
}

time_t TaskGetExecutionTime(const task_t *task)
{
    assert(NULL != task);
    
    return (task->execution_time);
}

int TaskUpdateExecTime(task_t *task)
{
	time_t current_time = 0;
	
    assert(NULL != task);
    
    current_time = time(NULL);
    if (-1 == current_time)
    {
    	return (1);
    }
    
    task->execution_time = current_time + task->interval_seconds;
    
    return (0);
}
