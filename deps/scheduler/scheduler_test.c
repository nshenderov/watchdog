/*******************************************************************************
*
* FILENAME : scheduler_test.c
*
* DESCRIPTION : Scheduler unit tests.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 09.05.2023
* 
*******************************************************************************/

#include <stdlib.h> /* free */
#include <stdio.h> /* printf */
#include <string.h> /* strstr, fclose, fopen, fread, feof */

#include "scheduler.h"
#include "testing.h"


enum {COMPLETE, RESCHEDULE, FAILED};

#define DUMMY_TASK scheduler, Execute, Cleanup, NULL, NULL, 2

typedef int(*action_func_t)(void *params);
typedef int(*exit_func_t)(void *params);

typedef struct op_params
{
	scheduler_t *scheduler;
	action_func_t action_func;
	exit_func_t exit_func;
	void *data;
	void *exit_params;
	int repetable;
} op_params_container_t;

static int Execute(void *operation_params);
static int ExitByFile(void *operation_params);
static int ExitByValue(void *operation_params);
static int IncrementValue(void *val);
static void Cleanup(void *cleanup_params);


static void TestSchedulerCreate(void);
static void TestSchedulerAddTask(void);
static void TestSchedulerRemoveTask(void);
static void TestSchedulerSize(void);
static void TestSchedulerIsEmpty(void);
static void TestSchedulerClear(void);
static void TestSchedulerRun(void);
static void TestSchedulerStop(void);
static void TestSchedulerExitByFile(void);

int main()
{
	TH_TEST_T TESTS[] = {
		{"SchedulerCreate", TestSchedulerCreate},
		{"SchedulerAddTask", TestSchedulerAddTask},
		{"SchedulerRemoveTask", TestSchedulerRemoveTask},
		{"SchedulerSize", TestSchedulerSize},
		{"SchedulerIsEmpty", TestSchedulerIsEmpty},
		{"SchedulerClear", TestSchedulerClear},
		{"SchedulerRun", TestSchedulerRun},
		{"SchedulerStop", TestSchedulerStop},
		{"ExitByFile", TestSchedulerExitByFile},
		TH_TESTS_ARRAY_END
	};

	TH_RUN_TESTS(TESTS);

	return (0);
}

static void TestSchedulerCreate(void)
{
	scheduler_t *scheduler = SchedulerCreate();

	TH_ASSERT(NULL != scheduler);

	SchedulerDestroy(scheduler);
}

static void TestSchedulerAddTask(void)
{
	nsrd_uid_t uid1 = {0}, uid2 = {0};

	scheduler_t *scheduler = SchedulerCreate();

	uid1 = SchedulerAddTask(DUMMY_TASK);

	TH_ASSERT(1 == SchedulerSize(scheduler));

	uid2 = SchedulerAddTask(DUMMY_TASK);

	TH_ASSERT(2 == SchedulerSize(scheduler));

	SchedulerDestroy(scheduler);

	(void) uid1;
	(void) uid2;
}

static void TestSchedulerRemoveTask(void)
{
	nsrd_uid_t uid1 = {0}, uid2 = {0};

	scheduler_t *scheduler = SchedulerCreate();

	uid1 = SchedulerAddTask(DUMMY_TASK);

	SchedulerRemoveTask(scheduler, uid1);

	TH_ASSERT(1 == SchedulerIsEmpty(scheduler));

	uid2 = SchedulerAddTask(DUMMY_TASK);

	SchedulerRemoveTask(scheduler, uid2);

	TH_ASSERT(1 == SchedulerIsEmpty(scheduler));

	SchedulerDestroy(scheduler);

	(void) uid1;
	(void) uid2;
}

static void TestSchedulerSize(void)
{
	nsrd_uid_t uid1 = {0}, uid2 = {0};

	scheduler_t *scheduler = SchedulerCreate();

	uid1 = SchedulerAddTask(DUMMY_TASK);

	TH_ASSERT(1 == SchedulerSize(scheduler));

	uid2 = SchedulerAddTask(DUMMY_TASK);
	uid2 = SchedulerAddTask(DUMMY_TASK);
	uid2 = SchedulerAddTask(DUMMY_TASK);
	uid2 = SchedulerAddTask(DUMMY_TASK);
	uid2 = SchedulerAddTask(DUMMY_TASK);

	TH_ASSERT(6 == SchedulerSize(scheduler));

	SchedulerDestroy(scheduler);

	(void) uid1;
	(void) uid2;
}

static void TestSchedulerIsEmpty(void)
{
	nsrd_uid_t uid1 = {0}, uid2 = {0};

	scheduler_t *scheduler = SchedulerCreate();

	uid1 = SchedulerAddTask(DUMMY_TASK);

	TH_ASSERT(1 == SchedulerSize(scheduler));

	uid2 = SchedulerAddTask(DUMMY_TASK);
	SchedulerRemoveTask(scheduler, uid2);

	uid2 = SchedulerAddTask(DUMMY_TASK);
	SchedulerRemoveTask(scheduler, uid2);

	uid2 = SchedulerAddTask(DUMMY_TASK);
	SchedulerRemoveTask(scheduler, uid2);

	uid2 = SchedulerAddTask(DUMMY_TASK);
	SchedulerRemoveTask(scheduler, uid2);

	uid2 = SchedulerAddTask(DUMMY_TASK);
	SchedulerRemoveTask(scheduler, uid2);

	TH_ASSERT(0 == SchedulerIsEmpty(scheduler));

	SchedulerRemoveTask(scheduler, uid1);

	TH_ASSERT(1 == SchedulerIsEmpty(scheduler));

	SchedulerDestroy(scheduler);

	(void) uid1;
	(void) uid2;
}
static void TestSchedulerClear(void)
{
	nsrd_uid_t uid1 = {0}, uid2 = {0};

	scheduler_t *scheduler = SchedulerCreate();

	uid1 = SchedulerAddTask(DUMMY_TASK);

	TH_ASSERT(1 == SchedulerSize(scheduler));

	uid2 = SchedulerAddTask(DUMMY_TASK);
	uid2 = SchedulerAddTask(DUMMY_TASK);
	uid2 = SchedulerAddTask(DUMMY_TASK);
	uid2 = SchedulerAddTask(DUMMY_TASK);
	uid2 = SchedulerAddTask(DUMMY_TASK);

	SchedulerClear(scheduler);

	TH_ASSERT(0 == SchedulerSize(scheduler));

	SchedulerDestroy(scheduler);

	(void) uid1;
	(void) uid2;
}

static void TestSchedulerRun(void)
{
	int t1 = 0, e1 = 3, e2 = 8;

	scheduler_t *scheduler = SchedulerCreate();

	op_params_container_t params1 = {NULL, IncrementValue, ExitByValue, NULL, NULL, 0};
	op_params_container_t params2 = {NULL, IncrementValue, ExitByValue, NULL, NULL, 0};

	nsrd_uid_t uid1 = {0}, uid2 = {0};

	params1.scheduler = scheduler;
	params2.scheduler = scheduler;
	params1.data = &t1;
	params2.data = &t1;
	params1.exit_params = &e1;
	params2.exit_params = &e2;

	uid1 = SchedulerAddTask(scheduler, Execute, Cleanup, &params1, NULL, 0);
	uid2 = SchedulerAddTask(scheduler, Execute, Cleanup, &params2, NULL, 0);

	printf("SCHEDULER IS RUNNING\n");
	TH_ASSERT(SUCCESS == SchedulerRun(scheduler));
	printf("SCHEDULER IS STOPPED\n");

	SchedulerClear(scheduler);

	TH_ASSERT(1 == SchedulerIsEmpty(scheduler));
	TH_ASSERT(0 == SchedulerSize(scheduler));

	TH_ASSERT(2 == t1);

	SchedulerDestroy(scheduler);

	(void) t1;
	(void) e1;
	(void) e2;
	(void) uid1;
	(void) uid2;
}
static void TestSchedulerStop(void)
{
	int t1 = 0, e1 = 3, e2 = 8;

	scheduler_t *scheduler = SchedulerCreate();

	op_params_container_t params1 = {NULL, IncrementValue, ExitByValue, NULL, NULL, 1};
	op_params_container_t params2 = {NULL, IncrementValue, ExitByValue, NULL, NULL, 1};

	nsrd_uid_t uid1 = {0}, uid2 = {0};

	params1.scheduler = scheduler;
	params2.scheduler = scheduler;
	params1.data = &t1;
	params2.data = &t1;
	params1.exit_params = &e1;
	params2.exit_params = &e2;

	uid1 = SchedulerAddTask(scheduler, Execute, Cleanup, &params1, NULL, 2);
	uid2 = SchedulerAddTask(scheduler, Execute, Cleanup, &params1, NULL, 5);

	printf("SCHEDULER IS RUNNING\n");
	TH_ASSERT(STOPPED == SchedulerRun(scheduler));
	printf("SCHEDULER IS STOPPED\n");

	SchedulerClear(scheduler);

	TH_ASSERT(3 == t1);

	uid1 = SchedulerAddTask(scheduler, Execute, Cleanup, &params2, NULL, 2);
	uid2 = SchedulerAddTask(scheduler, Execute, Cleanup, &params2, NULL, 5);

	SchedulerRemoveTask(scheduler, uid2);

	printf("SCHEDULER IS RUNNING\n");
	TH_ASSERT(STOPPED == SchedulerRun(scheduler));
	printf("SCHEDULER IS STOPPED\n");

	TH_ASSERT(8 == t1);

	SchedulerDestroy(scheduler);

	(void) t1;
	(void) e1;
	(void) e2;
	(void) uid1;
	(void) uid2;
}

static void TestSchedulerExitByFile(void)
{
	int t1 = 0;
	char *stop_keyword = "stop";

	scheduler_t *scheduler = SchedulerCreate();
	
	op_params_container_t params = {NULL, IncrementValue, ExitByFile, NULL, NULL, 1};

	nsrd_uid_t uid1 = {0}, uid2 = {0};

	params.scheduler = scheduler;
	params.data = &t1;
	params.exit_params = stop_keyword;

	TH_ASSERT(1 == SchedulerIsEmpty(scheduler));
	TH_ASSERT(0 == SchedulerSize(scheduler));

	uid1 = SchedulerAddTask(scheduler, Execute, Cleanup, &params, NULL, 2);
	uid2 = SchedulerAddTask(scheduler, Execute, Cleanup, &params, NULL, 5);

	TH_ASSERT(0 == SchedulerIsEmpty(scheduler));
	TH_ASSERT(2 == SchedulerSize(scheduler));

	printf("SCHEDULER IS RUNNING\n");
	printf(".scheduler file is ready, write \"%s\"\nkeyword there to stop the scheduler\n", stop_keyword);
	TH_ASSERT(STOPPED == SchedulerRun(scheduler));
	printf("SCHEDULER IS STOPPED\n");

	SchedulerDestroy(scheduler);

	(void) t1;
	(void) uid1;
	(void) uid2;
}

static int Execute(void *operation_params)
{
	op_params_container_t *box = operation_params;

	action_func_t Action = box->action_func;
	exit_func_t Exit = box->exit_func;

	if (FAILED == Action(box->data) || FAILED == Exit(box))
	{
		return (FAILED);
	}

	return (box->repetable ? RESCHEDULE : COMPLETE);
}

static int ExitByFile(void *operation_params)
{
	op_params_container_t *box = operation_params;

	char *stop_keyword = (char *) (box->exit_params);

	const char *file_name = ".scheduler";
	char buffer_from_file[1024] = {0};
	FILE *stream_from = NULL;
	size_t read_res;


	stream_from = fopen(file_name, "a+");
	if (NULL == stream_from)
	{
		printf("Cannot open %s\n", file_name);
		return (FAILED);
	}

	printf(">>.scheduler file is checked\n");

	while (0 == feof(stream_from))
	{
		read_res = fread(buffer_from_file, sizeof(char), 1024, stream_from);
	}

	if (NULL != strstr(buffer_from_file, stop_keyword))
	{
		fclose(stream_from);
		remove(file_name);

		SchedulerStop(box->scheduler);
	}

	return (COMPLETE);

	(void) read_res;
}

static int ExitByValue(void *operation_params)
{
	op_params_container_t *box = operation_params;

	if (*(int *)(box->data) >= *(int *)(box->exit_params))
	{
		SchedulerStop(box->scheduler);
	}

	return (COMPLETE);
}

static int IncrementValue(void *val)
{
	int *v = val;
	*v += 1;

	printf("TASK_INCR_EXECUTED, val = %d\n", *v);

	return(COMPLETE);
}

static void Cleanup(void *cleanup_params)
{
	free(cleanup_params);
	cleanup_params = NULL;
}
