/*******************************************************************************
*
* FILENAME : task_test.c
*
* DESCRIPTION : Task unit tests.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 7.05.2023
* 
*******************************************************************************/

#include <stdlib.h> /* free */
#include <unistd.h> /* sleep */

#include "task.h"
#include "testing.h"


#define FREE_MEMORY(ptr) \
{free(ptr); (ptr) = NULL;}

typedef op_status_t(*actual_action_t)(void *params);

typedef struct op_params
{
	actual_action_t func;
	int test_val;
	int *allocated_mem;
} op_params_container_t;

static void TestTaskExecute(void);
static op_status_t ExecIncr(void *operation_params);
static op_status_t IncrInt(void *val){*((int *)val) += 1; return(COMPLETE);}
static void Cleanup(void *cleanup_params){FREE_MEMORY(cleanup_params);}


static void TestTaskIsSame(void);
static void TestTaskCompare(void);
static void TestTaskGetUID(void);
static void TestTaskGetExecutionTime(void);
static void TestTaskUpdateExecTime(void);

int main()
{
    TH_TEST_T TESTS[] = {
        {"Test IsSame", TestTaskIsSame},
        {"Compare", TestTaskCompare},
        {"Execute", TestTaskExecute},
        {"UpdateExecTime", TestTaskUpdateExecTime},
        {"GetExecutionTime", TestTaskGetExecutionTime},
        {"GetUID", TestTaskGetUID},
        TH_TESTS_ARRAY_END
    };

    TH_RUN_TESTS(TESTS);

    return (0);
}

static void TestTaskIsSame(void)
{
	op_params_container_t box1 = {IncrInt, 0, NULL};
	op_params_container_t box2 = {IncrInt, 0, NULL};

	task_t *task = TaskCreate(ExecIncr, Cleanup, &box1, NULL, 0);
	task_t *task2 = TaskCreate(ExecIncr, Cleanup, &box2, NULL, 0);

	TH_ASSERT(1 == TaskIsSame(task, task));
	TH_ASSERT(0 == TaskIsSame(task, task2));

	TaskDestroy(task);
	TaskDestroy(task2);
}

static void TestTaskCompare(void)
{
	op_params_container_t box1 = {IncrInt, 0, NULL};
	op_params_container_t box2 = {IncrInt, 0, NULL};

	task_t *task = TaskCreate(ExecIncr, Cleanup, &box1, NULL, 0);
	task_t *task2 = TaskCreate(ExecIncr, Cleanup, &box2, NULL, 200);
	task_t *task3 = TaskCreate(ExecIncr, Cleanup, &box2, NULL, 200);

	TH_ASSERT(0 == TaskCompare(task, task));
	TH_ASSERT(0 < TaskCompare(task, task2));
	TH_ASSERT(0 > TaskCompare(task2, task));
	TH_ASSERT(0 == TaskCompare(task2, task3));

	TaskDestroy(task);
	TaskDestroy(task2);
	TaskDestroy(task3);
}


static void TestTaskExecute(void)
{
	op_params_container_t box = {IncrInt, 15, NULL};

	task_t *task2 = TaskCreate(ExecIncr, Cleanup, &box, NULL, 0);

	TH_ASSERT(COMPLETE == TaskExecute(task2));
	TH_ASSERT(COMPLETE == TaskExecute(task2));
	TH_ASSERT(COMPLETE == TaskExecute(task2));
	TH_ASSERT(COMPLETE == TaskExecute(task2));

	TH_ASSERT(19 == box.test_val);

	TaskDestroy(task2);
}

static op_status_t ExecIncr(void *operation_params)
{
	op_params_container_t *box = operation_params;

	actual_action_t Action = box->func;

	Action(&(box->test_val));

	return (COMPLETE);
}

static void TestTaskGetExecutionTime(void)
{
	op_params_container_t box = {IncrInt, 0, NULL};

	task_t *task = TaskCreate(ExecIncr, Cleanup, &box, NULL, 0);

	TH_ASSERT(time(NULL) == TaskGetExecutionTime(task));

	TaskDestroy(task);
}

static void TestTaskUpdateExecTime(void)
{
	op_params_container_t box = {IncrInt, 0, NULL};
	task_t *task = TaskCreate(ExecIncr, Cleanup, &box, NULL, 0);
	time_t time_task = TaskGetExecutionTime(task);
	time_t time_now = time(NULL);

	TH_ASSERT(0.0 == difftime(time_now, time_task));

	sleep(2);
	time_now = time(NULL);
	TH_ASSERT(0.0 == difftime(time_now, (TaskGetExecutionTime(task) + 2)));

	TaskDestroy(task);
}

static void TestTaskGetUID(void)
{
	op_params_container_t box = {IncrInt, 0, NULL};
	task_t *task = TaskCreate(ExecIncr, Cleanup, &box, NULL, 0);
	nsrd_uid_t uid = TaskGetUID(task);

	printf("\n  PID: %d \n", uid.pid);
	printf("  Counter: %ld \n", uid.counter);
	printf("  Timestamp: %s", ctime(&uid.timestamp));

	TaskDestroy(task);
}