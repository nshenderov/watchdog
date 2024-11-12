/*******************************************************************************
* FILENAME : dlist_test.c
*
* DESCRIPTION : Doubly linked list unit tests.
*
* AUTHOR : Nick Shenderov
* 
* DATE : 22.04.2023
*
*******************************************************************************/

#include <stdio.h> /* printf */

#include "dlist.h"
#include "testing.h"


static int EqualsInt(const void *data, void *param);
static int AddInt(void *data, void *param);


static void TestGeneral(void);
static void TestSplice(void);
static void TestFind(void);
static void TestMultiFind(void);
static void TestForEach(void);

int main()
{
	TH_TEST_T tests[] = {
		{"General", TestGeneral},
		{"Splice", TestSplice},
		{"Find", TestFind},
		{"MultiFind", TestMultiFind},
		{"ForEach", TestForEach},
		TH_TESTS_ARRAY_END
	};

	TH_RUN_TESTS(tests);

	return (0);
}


static void TestGeneral(void)
{
	int n1 = 123;
	float n2 = 99.123;
	double n3 = 3.22;

	dlist_iterator_t list_begin = NULL;
	dlist_iterator_t list_head_next = NULL;
	dlist_iterator_t list_end = NULL;

	dlist_t *list = DListCreate();

	list_begin = DListBegin(list);
	TH_ASSERT(NULL != list_begin);

	list_begin = DListPushFront(list, &n1);
    TH_ASSERT(n1 == *(int *) DListGetData(list_begin));
    TH_ASSERT(n1 == *(int *) DListPopFront(list));

	list_begin = DListBegin(list);

	list_end = DListPushBack(list, &n1);
	TH_ASSERT(n1 == *(int *) DListGetData(list_end));

    TH_ASSERT(n1 == *(int *) DListPopBack(list));

	list_end = DListEnd(list);

	list_begin = DListInsert(list_begin, &n1);
    TH_ASSERT(n1 == *(int *) DListGetData(list_begin));

	list_begin = DListInsert(list_begin, &n2);
    TH_ASSERT(n2 == *(float *) DListGetData(list_begin));

	list_begin = DListInsert(list_begin, &n3);
    TH_ASSERT(n3 == *(double *) DListGetData(list_begin));

    TH_ASSERT(3 == DListSize(list));

	list_end = DListEnd(list);
    TH_ASSERT(NULL == DListNext(list_end));
	
    list_end = DListInsert(list_end, &n1);
    TH_ASSERT(n1 == *(int *) DListGetData(list_end));
    TH_ASSERT(NULL == DListNext(DListNext(list_end)));

    TH_ASSERT(4 == DListSize(list));

	list_head_next = DListNext(list_begin);
    TH_ASSERT(n2 == *(float *) DListGetData(list_head_next));

	list_head_next = DListNext(list_end);
    TH_ASSERT(NULL == DListNext(list_head_next));

	list_begin = DListRemove(list_begin);
	TH_ASSERT(3 == DListSize(list));
    TH_ASSERT(n2 == *(float *) DListGetData(list_begin));

	list_begin = DListRemove(list_begin);
	TH_ASSERT(2 == DListSize(list));
    TH_ASSERT(n1 == *(int *) DListGetData(list_begin));

	list_begin = DListRemove(list_begin);
	list_begin = DListRemove(list_begin);

    TH_ASSERT(1 == DListIsEmpty(list));

	DListDestroy(list);
}

static void TestSplice(void)
{
    int n1 = 123;
	float n2 = 99.123;
	double n3 = 3.22;

    dlist_iterator_t list_begin = NULL;

	dlist_t *list = DListCreate();
	dlist_t *list2 = DListCreate();

    list_begin = DListBegin(list);
	list_begin = DListInsert(list_begin, &n1);
	list_begin = DListInsert(list_begin, &n2);
	list_begin = DListInsert(list_begin, &n3);

    list_begin = DListBegin(list2);
	list_begin = DListInsert(list_begin, &n1);
	list_begin = DListInsert(list_begin, &n2);
	list_begin = DListInsert(list_begin, &n3);

    list_begin = DListSplice(DListBegin(list), DListBegin(list2), DListEnd(list2));
    TH_ASSERT(n3 == *(double *) DListGetData(list_begin));

    list_begin = DListNext(list_begin);
    TH_ASSERT(n2 == *(float *) DListGetData(list_begin));

	list_begin = DListBegin(list);
    TH_ASSERT(n3 == *(double *) DListGetData(list_begin));

	list_begin = DListNext(DListNext(list_begin));
    TH_ASSERT(n1 == *(int *) DListGetData(list_begin));

    TH_ASSERT(1 == DListIsEmpty(list2));

    DListDestroy(list);
	DListDestroy(list2);
}

static void TestFind(void)
{
    int n1 = 123;
	float n2 = 99.123;
	double n3 = 3.22;

    dlist_iterator_t list_begin = NULL;
    dlist_iterator_t search_res = NULL;

	dlist_t *list = DListCreate();

    list_begin = DListBegin(list);
	list_begin = DListInsert(list_begin, &n1);
	list_begin = DListInsert(list_begin, &n2);

    search_res = DListFind(DListBegin(list), DListEnd(list), EqualsInt, &n2);
    TH_ASSERT(n2 == *(float *) DListGetData(search_res));
    TH_ASSERT(NULL != DListNext(search_res));

    search_res = DListFind(DListBegin(list), DListEnd(list), EqualsInt, &n3);
    TH_ASSERT(1 == DListIsSameIterator(DListEnd(list), search_res));

    DListDestroy(list);
}

static void TestMultiFind(void)
{
    int n1 = 123, n2 = 3;

    dlist_iterator_t list_begin = NULL;

	dlist_t *list = DListCreate();
    dlist_t *list_of_res = DListCreate();

    list_begin = DListBegin(list);
	list_begin = DListInsert(list_begin, &n1);
	list_begin = DListInsert(list_begin, &n1);
	list_begin = DListInsert(list_begin, &n2);
	list_begin = DListInsert(list_begin, &n1);

    TH_ASSERT(3 == DListMultiFind(DListBegin(list), DListEnd(list), list_of_res, EqualsInt, &n1));

    TH_ASSERT(3 == DListSize(list_of_res));

    list_begin = DListBegin(list_of_res);
    TH_ASSERT(n1 == *(int *) DListGetData(list_begin));
    TH_ASSERT(NULL != DListNext(list_begin));
    TH_ASSERT(n1 == *(int *) DListGetData(DListNext(DListNext(list_begin))));

	DListDestroy(list);
	DListDestroy(list_of_res);
}

static void TestForEach(void)
{
    int n1 = 1, n2 = 99, n_to_add = 2, n1_exp = 3, n2_exp = 101;

    dlist_iterator_t list_begin = NULL;
    dlist_iterator_t search_res = NULL;

	dlist_t *list = DListCreate();

    list_begin = DListBegin(list);
	list_begin = DListInsert(list_begin, &n1);
	list_begin = DListInsert(list_begin, &n2);

    
    TH_ASSERT(0 == DListForEach(DListBegin(list), DListEnd(list), AddInt, &n_to_add));

	search_res = DListFind(DListBegin(list), DListEnd(list), EqualsInt, &n1_exp);
    TH_ASSERT(0 == DListIsSameIterator(DListEnd(list), search_res)
              && n1_exp == *(int *) DListGetData(search_res)
              && NULL != DListNext(search_res));

	search_res = DListFind(DListBegin(list), DListEnd(list), EqualsInt, &n2_exp);
    TH_ASSERT(0 == DListIsSameIterator(DListEnd(list), search_res)
              && n2_exp == *(int *) DListGetData(search_res)
              && NULL != DListNext(search_res));

    DListDestroy(list);
}

static int EqualsInt(const void *data, void *param)
{
	if (*(int *) data == *(int *) param)
	{
		return (1);
	}

	return (0);
}

static int AddInt(void *data, void *param)
{
	*(int *) data += *(int *) param;

	return (0);
}