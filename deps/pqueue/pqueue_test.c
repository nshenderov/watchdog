/*******************************************************************************
*
* FILENAME : pqueue_test.c
*
* DESCRIPTION : Pqueue unit tests.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 07.05.2023
* 
*******************************************************************************/

#include "pqueue.h"
#include "testing.h"


static int CompareInts(const void* data1, const void *data2);
static int RemoveInt(const void *data, void *param);


static void TestPqueue(void);

int main()
{
	TH_TEST_T TESTS[] = {
		{"Test pqueue", TestPqueue},
		TH_TESTS_ARRAY_END
	};

	TH_RUN_TESTS(TESTS);

	return (0);
}

static void TestPqueue(void)
{
	int arr[10] = {1,2,3,4,5,6,7,8,9,10};

	pq_t *pqueue = PQCreate(CompareInts);

	PQEnqueue(pqueue, arr);
	PQEnqueue(pqueue, arr+1);
	PQEnqueue(pqueue, arr+9);
	TH_ASSERT(1 != PQSize(pqueue));
	TH_ASSERT(3 == PQSize(pqueue));
	TH_ASSERT(10 == *(int *) PQPeek(pqueue));

	PQDequeue(pqueue);
	TH_ASSERT(2 == *(int *) PQPeek(pqueue));

	PQDequeue(pqueue);
	TH_ASSERT(1 == *(int *) PQPeek(pqueue));

	PQEnqueue(pqueue, arr+3);
	PQEnqueue(pqueue, arr+5);
	TH_ASSERT(3 == PQSize(pqueue));

	PQErase(pqueue, RemoveInt, arr+5);
	TH_ASSERT(2 == PQSize(pqueue));
	TH_ASSERT(4 == *(int *) PQPeek(pqueue));

	PQErase(pqueue, RemoveInt, arr);
	TH_ASSERT(1 == PQSize(pqueue));
	TH_ASSERT(4 == *(int *) PQPeek(pqueue));

	TH_ASSERT(0 == PQIsEmpty(pqueue));
	PQClear(pqueue);
	TH_ASSERT(1 == PQIsEmpty(pqueue));

	PQEnqueue(pqueue, arr+3);
	PQEnqueue(pqueue, arr+5);
	TH_ASSERT(2 == PQSize(pqueue));
	PQClear(pqueue);
	TH_ASSERT(1 == PQIsEmpty(pqueue));

	PQEnqueue(pqueue, arr+1);
	PQEnqueue(pqueue, arr+2);
	TH_ASSERT(2 == PQSize(pqueue));
	PQDequeue(pqueue);
	PQDequeue(pqueue);
	TH_ASSERT(0 == PQSize(pqueue));
	TH_ASSERT(1 == PQIsEmpty(pqueue));
	
	PQDestroy(pqueue);
}

static int CompareInts(const void* data1, const void *data2)
{
	if (*(int *) data1 < *(int *) data2)
	{
		return (-1);
	}
	else if (*(int *) data1 == *(int *) data2)
	{
		return (0);
	}

	return (1);
}

static int RemoveInt(const void *data, void *param)
{
	if (*(int *) data == *(int *) param)
	{
		return (1);
	}

	return (0);
}