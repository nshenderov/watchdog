/*******************************************************************************
*
* FILENAME : sorted_list_test.c
*
* AUTHOR : Nick Shenderov
*
* DATE : 24.04.2023
* 
*******************************************************************************/

#include <stdio.h>

#include "sorted_list.h" /* sorted linked list */
#include "testing.h" /* TH_ASSERT, TH_TEST_T, TH_TESTS_ARRAY_END, TH_RUN_TESTS*/


static int CompareInts1(const void* data1, const void *data2)
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

static int CompareInts2(const void* data1, void *param)
{
	if (*(int *) data1 > *(int *) param)
	{
		return (-1);
	}
	else if (*(int *) data1 == *(int *) param)
	{
		return (0);
	}

	return (1);
}

static int CompareInts3(const void* data1, void *param)
{
	return (0);
	(void) data1;
	(void) param;
}

static int ActionAddInt(void *data, void *param)
{
	*(int *) data += *(int *) param;
	return (0);
}

static void TestSize(void);
static void TestSizeEmpty(void);
static void TestIsEmpty(void);
static void TestInsert(void);
static void TestInsertReturn(void);
/*static void TestRemove(void);
static void TestRemoveReturn(void);*/
static void TestBegin(void);
static void TestEnd(void);
static void TestNext(void);
static void TestPrev(void);
static void TestIsSameIterator(void);
static void TestGetData(void);
static void TestPopBack(void);
static void TestPopFront(void);
static void TestForEach(void);
static void TestMerge1(void);
static void TestMerge2(void);
static void TestMerge3(void);
static void TestFind(void);
static void TestFindIf(void);

int main()
{
	TH_TEST_T TESTS[] = {
		{"Test SortedListBegin", TestBegin},
		{"Test SortedListEnd", TestEnd},
		{"Test SortedListNext", TestNext},
		{"Test SortedListPrev", TestPrev},
		{"Test SortedListGetData", TestGetData},
		{"Test SortedListIsSameIterator", TestIsSameIterator},
		{"Test size", TestSize},
		{"Test size empty", TestSizeEmpty},
		{"Test is_empty", TestIsEmpty},
		{"Test insert general", TestInsert},
		{"Test insert return value", TestInsertReturn},
		/*{"Test remove general", TestRemove},
		{"Test remove return value", TestRemoveReturn},*/
		{"Test pop back", TestPopBack},
		{"Test pop front", TestPopFront},
		{"Test for each", TestForEach},
		{"Test merge 1", TestMerge1},
		{"Test merge 2", TestMerge2},
		{"Test merge 3", TestMerge3},
		{"Test find", TestFind},
		{"Test find_if", TestFindIf},
		TH_TESTS_ARRAY_END
	};

	TH_RUN_TESTS(TESTS);

	return (0);
}

static void TestSize(void)
{
	int t = 3;

	sorted_list_t *new_list = SortedListCreate(CompareInts1);

	SortedListInsert(new_list, &t);
	TH_ASSERT(1 == SortedListSize(new_list));

	SortedListInsert(new_list, &t);
	TH_ASSERT(2 == SortedListSize(new_list));

	SortedListDestroy(new_list);
}

static void TestSizeEmpty(void)
{
	sorted_list_t *new_list = SortedListCreate(CompareInts1);

	TH_ASSERT(0 == SortedListSize(new_list));

	SortedListDestroy(new_list);
}

static void TestIsEmpty(void)
{
	int t = 3;

	sorted_list_t *new_list = SortedListCreate(CompareInts1);

	TH_ASSERT(1 == SortedListIsEmpty(new_list));

	SortedListInsert(new_list, &t);
	TH_ASSERT(0 == SortedListIsEmpty(new_list));

	SortedListDestroy(new_list);
}

static void TestInsert(void)
{
	int t0 = 0, t1 = 1, t2 = 2, t3 = 3, t4 = 4, t7 = 7, t9 = 9;

	sorted_list_iterator_t r = {0};

	sorted_list_t *new_list = SortedListCreate(CompareInts1);

	SortedListInsert(new_list, &t1);
	SortedListInsert(new_list, &t2);
	SortedListInsert(new_list, &t3);
	SortedListInsert(new_list, &t4);

	r = SortedListBegin(new_list);
	TH_ASSERT(1 == *(int *) SortedListGetData(r));

	r = SortedListNext(r);
	TH_ASSERT(2 == *(int *) SortedListGetData(r));

	r = SortedListNext(r);
	TH_ASSERT(3 == *(int *) SortedListGetData(r));

	r = SortedListNext(r);
	TH_ASSERT(4 == *(int *) SortedListGetData(r));

	SortedListInsert(new_list, &t1);
	SortedListInsert(new_list, &t1);
	r = SortedListBegin(new_list);
	TH_ASSERT(1 == *(int *) SortedListGetData(r));
	r = SortedListNext(r);
	TH_ASSERT(1 == *(int *) SortedListGetData(r));
	r = SortedListNext(r);
	TH_ASSERT(1 == *(int *) SortedListGetData(r));

	SortedListInsert(new_list, &t9);
	r = SortedListEnd(new_list);
	r = SortedListPrev(r);
	TH_ASSERT(9 == *(int *) SortedListGetData(r));
	r = SortedListPrev(r);
	TH_ASSERT(4 == *(int *) SortedListGetData(r));

	SortedListInsert(new_list, &t7);
	r = SortedListEnd(new_list);
	r = SortedListPrev(r);
	TH_ASSERT(9 == *(int *) SortedListGetData(r));
	r = SortedListPrev(r);
	TH_ASSERT(7 == *(int *) SortedListGetData(r));

	SortedListInsert(new_list, &t0);
	r = SortedListBegin(new_list);
	TH_ASSERT(0 == *(int *) SortedListGetData(r));

	SortedListDestroy(new_list);
}

static void TestInsertReturn(void)
{
	int t = 3;

	sorted_list_t *new_list = SortedListCreate(CompareInts1);

	sorted_list_iterator_t r = SortedListInsert(new_list, &t);
	TH_ASSERT(3 == *(int *) SortedListGetData(r));
	TH_ASSERT(NULL == SortedListNext(SortedListNext(r)).internal_iter);

	SortedListDestroy(new_list);
}

/*static void TestRemove(void)
{
	int t1 = 1, t2 = 2, t3 = 3, t4 = 4, t5 = 5;

	sorted_list_iterator_t r = {0};

	sorted_list_t *new_list = SortedListCreate(CompareInts1);

	SortedListInsert(new_list, &t1);
	SortedListInsert(new_list, &t2);
	SortedListInsert(new_list, &t3);
	SortedListInsert(new_list, &t4);
	SortedListInsert(new_list, &t5);

	r = SortedListBegin(new_list);

	r = SortedListRemove(r);
	TH_ASSERT(2 == *(int *) SortedListGetData(r));

	r = SortedListEnd(new_list);
	r = SortedListPrev(r);
	r = SortedListRemove(r);
	TH_ASSERT(4 == *(int *) SortedListGetData(r));

	r = SortedListEnd(new_list);
	r = SortedListPrev(r);
	r = SortedListRemove(r);
	TH_ASSERT(3 == *(int *) SortedListGetData(r));

	SortedListDestroy(new_list);
}

static void TestRemoveReturn(void)
{
	int t1 = 1;
	int t2 = 2;

	sorted_list_t *new_list = SortedListCreate(CompareInts1);

	sorted_list_iterator_t r1 = SortedListInsert(new_list, &t1);
	sorted_list_iterator_t r2 = SortedListInsert(new_list, &t2);

	r1 = SortedListRemove(r1);
	TH_ASSERT(2 == *(int *) SortedListGetData(r1));

	r2 = SortedListRemove(r2);
	TH_ASSERT(NULL == SortedListNext(r2).internal_iter);
	TH_ASSERT(NULL == SortedListPrev(r2).internal_iter);

	SortedListDestroy(new_list);
}*/

static void TestPopBack(void)
{
	int t1 = 1, t2 = 2, t3 = 3, t4 = 4, t5 = 5;

	sorted_list_iterator_t r = {0};

	int *data = NULL;

	sorted_list_t *new_list = SortedListCreate(CompareInts1);

	SortedListInsert(new_list, &t1);
	SortedListInsert(new_list, &t2);
	SortedListInsert(new_list, &t3);
	SortedListInsert(new_list, &t4);
	SortedListInsert(new_list, &t5);

	r = SortedListBegin(new_list);

	data = (int *) SortedListPopBack(new_list);
	TH_ASSERT(5 == *data);

	data = (int *) SortedListPopBack(new_list);
	TH_ASSERT(4 == *data);

	data = (int *) SortedListPopBack(new_list);
	TH_ASSERT(3 == *data);

	r = SortedListEnd(new_list);
	r = SortedListPrev(r);
	TH_ASSERT(2 == *(int *) SortedListGetData(r));

	SortedListDestroy(new_list);
}

static void TestPopFront(void)
{
	int t1 = 1, t2 = 2, t3 = 3, t4 = 4, t5 = 5;

	sorted_list_iterator_t r = {0};

	int *data = NULL;

	sorted_list_t *new_list = SortedListCreate(CompareInts1);

	SortedListInsert(new_list, &t1);
	SortedListInsert(new_list, &t2);
	SortedListInsert(new_list, &t3);
	SortedListInsert(new_list, &t4);
	SortedListInsert(new_list, &t5);

	r = SortedListBegin(new_list);

	data = (int *) SortedListPopFront(new_list);
	TH_ASSERT(1 == *data);

	data = (int *) SortedListPopFront(new_list);
	TH_ASSERT(2 == *data);

	data = (int *) SortedListPopFront(new_list);
	TH_ASSERT(3 == *data);

	r = SortedListBegin(new_list);
	TH_ASSERT(4 == *(int *) SortedListGetData(r));

	SortedListDestroy(new_list);
}


static void TestBegin(void)
{
	int t1 = 1;

	sorted_list_t *new_list = SortedListCreate(CompareInts1);

	sorted_list_iterator_t r = SortedListBegin(new_list);

	TH_ASSERT(NULL == SortedListNext(r).internal_iter);
	TH_ASSERT(NULL == SortedListPrev(SortedListPrev(r)).internal_iter);

	r = SortedListInsert(new_list, &t1);

	r = SortedListBegin(new_list);
	TH_ASSERT(1 == *(int *) SortedListGetData(r));

	SortedListDestroy(new_list);
}
static void TestEnd(void)
{
	int t1 = 1;
	int t2 = 2;

	sorted_list_t *new_list = SortedListCreate(CompareInts1);

	sorted_list_iterator_t r = SortedListEnd(new_list);

	TH_ASSERT(NULL == SortedListNext(r).internal_iter);
	TH_ASSERT(NULL == SortedListPrev(SortedListPrev(r)).internal_iter);

	r = SortedListInsert(new_list, &t1);
	r = SortedListInsert(new_list, &t2);

	r = SortedListEnd(new_list);
	TH_ASSERT(2 == *(int *) SortedListGetData(SortedListPrev(r)));

	SortedListDestroy(new_list);
}
static void TestNext(void)
{
	int t1 = 1;
	int t2= 2;

	sorted_list_t *new_list = SortedListCreate(CompareInts1);

	sorted_list_iterator_t r = SortedListEnd(new_list);

	r = SortedListNext(r);

	TH_ASSERT(NULL == r.internal_iter);

	r = SortedListInsert(new_list, &t1);
	r = SortedListInsert(new_list, &t2);

	r = SortedListBegin(new_list);
	r = SortedListNext(r);
	TH_ASSERT(2 == *(int *) SortedListGetData(r));

	SortedListDestroy(new_list);
}
static void TestPrev(void)
{
	int t1 = 1;
	int t2= 2;

	sorted_list_t *new_list = SortedListCreate(CompareInts1);

	sorted_list_iterator_t r = SortedListBegin(new_list);

	r = SortedListPrev(r);
	r = SortedListPrev(r);

	TH_ASSERT(NULL == r.internal_iter);

	r = SortedListInsert(new_list, &t1);
	r = SortedListInsert(new_list, &t2);

	r = SortedListEnd(new_list);
	r = SortedListPrev(r);
	r = SortedListPrev(r);
	TH_ASSERT(1 == *(int *) SortedListGetData(r));

	SortedListDestroy(new_list);
}

static void TestIsSameIterator(void)
{
	int t1 = 1;
	int t2 = 2;

	sorted_list_t *new_list = SortedListCreate(CompareInts1);

	sorted_list_iterator_t r1 = SortedListInsert(new_list, &t1);
	sorted_list_iterator_t r2 = SortedListInsert(new_list, &t2);

	TH_ASSERT(1 == SortedListIsSameIterator(r2, r2));
	TH_ASSERT(0 == SortedListIsSameIterator(r1, r2));

	SortedListDestroy(new_list);
}

static void TestGetData(void)
{
	int t1 = 1;
	int t2= 2;

	sorted_list_t *new_list = SortedListCreate(CompareInts1);

	sorted_list_iterator_t r1 = SortedListInsert(new_list, &t1);
	sorted_list_iterator_t r2 = SortedListInsert(new_list, &t2);

	TH_ASSERT(t2 == *(int *) SortedListGetData(r2));
	TH_ASSERT(t1 == *(int *) SortedListGetData(r1));

	SortedListDestroy(new_list);
}

static void TestForEach(void)
{
	int t1 = 1, t2 = 2, t3 = 3, t4 = 4, t5 = 5;

	sorted_list_iterator_t r = {0};

	sorted_list_t *new_list = SortedListCreate(CompareInts1);

	SortedListInsert(new_list, &t1);
	SortedListInsert(new_list, &t2);
	SortedListInsert(new_list, &t3);
	SortedListInsert(new_list, &t4);
	SortedListInsert(new_list, &t5);

	r = SortedListBegin(new_list);

	SortedListForEach(SortedListBegin(new_list), SortedListEnd(new_list),
																		ActionAddInt, &t5);

	TH_ASSERT(6 == *(int *) SortedListGetData(r));

	r = SortedListNext(r);
	TH_ASSERT(7 == *(int *) SortedListGetData(r));
	
	r = SortedListEnd(new_list);
	r = SortedListPrev(r);
	TH_ASSERT(10 == *(int *) SortedListGetData(r));

	SortedListDestroy(new_list);
}

static void TestMerge1(void)
{
	size_t i = 0;

	size_t expected_size = 8;
	size_t first_list_size = 5;
	size_t second_list_size = 3;

	int expected_result[8] = {1, 1, 2, 2, 3, 3, 4, 5};
	int first_list_vals[5] = {1, 2, 3, 4, 5};
	int second_list_vals[3] = {1, 2, 3};

	sorted_list_iterator_t r = {0};

	sorted_list_t *returned_list = NULL;
	sorted_list_t *list1 = SortedListCreate(CompareInts1);
	sorted_list_t *list2 = SortedListCreate(CompareInts1);

	for (; i < first_list_size; ++i)
	{
		SortedListInsert(list1, first_list_vals + i);
	}

	for (i = 0; i < second_list_size; ++i)
	{
		SortedListInsert(list2, second_list_vals + i);
	}


	returned_list = SortedListMerge(list1, list2);

	r = SortedListBegin(returned_list);

	TH_ASSERT(expected_size == SortedListSize(returned_list));

	for (i = 0; i < expected_size; ++i, r = SortedListNext(r))
	{
		TH_ASSERT(expected_result[i] == *(int *) SortedListGetData(r));
	}

	SortedListDestroy(list1);
	SortedListDestroy(list2);
}

static void TestMerge2(void)
{
	size_t i = 0;

	size_t expected_size = 12;
	size_t first_list_size = 9;
	size_t second_list_size = 3;

	int expected_result[12] = {1, 1, 1, 2, 2, 3, 3, 4, 5, 11, 12, 15};
	int first_list_vals[9] = {1, 1, 2, 3, 4, 5, 11, 12, 15};
	int second_list_vals[3] = {1, 2, 3};

	sorted_list_iterator_t r = {0};

	sorted_list_t *returned_list = NULL;
	sorted_list_t *list1 = SortedListCreate(CompareInts1);
	sorted_list_t *list2 = SortedListCreate(CompareInts1);

	for (; i < first_list_size; ++i)
	{
		SortedListInsert(list1, first_list_vals + i);
	}

	for (i = 0; i < second_list_size; ++i)
	{
		SortedListInsert(list2, second_list_vals + i);
	}

	returned_list = SortedListMerge(list2, list1);

	r = SortedListBegin(returned_list);

	TH_ASSERT(expected_size == SortedListSize(returned_list));

	for (i = 0; i < expected_size; ++i, r = SortedListNext(r))
	{
		TH_ASSERT(expected_result[i] == *(int *) SortedListGetData(r));
	}

	SortedListDestroy(list1);
	SortedListDestroy(list2);
}

static void TestMerge3(void)
{
	size_t i = 0;

	size_t expected_size = 15;
	size_t first_list_size = 10;
	size_t second_list_size = 5;

	int expected_result[15] = {1, 1, 2, 3, 4, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15};
	int first_list_vals[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int second_list_vals[5] = {1, 4, 11, 12, 15};

	sorted_list_iterator_t r = {0};

	sorted_list_t *returned_list = NULL;
	sorted_list_t *list1 = SortedListCreate(CompareInts1);
	sorted_list_t *list2 = SortedListCreate(CompareInts1);

	for (; i < first_list_size; ++i)
	{
		SortedListInsert(list1, first_list_vals + i);
	}

	for (i = 0; i < second_list_size; ++i)
	{
		SortedListInsert(list2, second_list_vals + i);
	}

	returned_list = SortedListMerge(list1, list2);

	r = SortedListBegin(returned_list);

	TH_ASSERT(expected_size == SortedListSize(returned_list));

	for (i = 0; i < expected_size; ++i, r = SortedListNext(r))
	{
		TH_ASSERT(expected_result[i] == *(int *) SortedListGetData(r));
	}

	SortedListDestroy(list1);
	SortedListDestroy(list2);
}

static void TestFind(void)
{
	int t0 = 0, t1 = 1, t2 = 2, t3 = 3, t4 = 4, t5 = 5, t6 = 6;

	sorted_list_iterator_t r = {0};

	sorted_list_t *list = SortedListCreate(CompareInts1);

	SortedListInsert(list, &t1);
	SortedListInsert(list, &t2);
	SortedListInsert(list, &t3);
	SortedListInsert(list, &t4);
	SortedListInsert(list, &t5);

	r = SortedListFind(list, SortedListBegin(list), SortedListEnd(list), &t5);
	TH_ASSERT(5 == *(int *) SortedListGetData(r));

	r = SortedListFind(list, SortedListBegin(list), SortedListEnd(list), &t0);
	TH_ASSERT(NULL == SortedListNext(r).internal_iter);

	r = SortedListFind(list, SortedListBegin(list), SortedListEnd(list), &t2);
	TH_ASSERT(2 == *(int *) SortedListGetData(r));

	r = SortedListFind(list, SortedListBegin(list), SortedListEnd(list), &t6);
	r = SortedListNext(r);
	TH_ASSERT(NULL == r.internal_iter);

	SortedListDestroy(list);
}

static void TestFindIf(void)
{
	int t0 = 0, t1 = 1, t2 = 2, t3 = 3, t4 = 4, t5 = 5, t6 = 6;

	sorted_list_iterator_t r = {0};

	sorted_list_t *list = SortedListCreate(CompareInts1);

	SortedListInsert(list, &t1);
	SortedListInsert(list, &t2);
	SortedListInsert(list, &t3);
	SortedListInsert(list, &t4);
	SortedListInsert(list, &t5);

	r = SortedListFindIf(SortedListBegin(list), SortedListEnd(list),
																CompareInts2, &t5);
	TH_ASSERT(1 == *(int *) SortedListGetData(r));

	r = SortedListFindIf(SortedListBegin(list), SortedListEnd(list),
																CompareInts2, &t0);
	TH_ASSERT(1 == *(int *) SortedListGetData(r));

	r = SortedListFindIf(SortedListBegin(list), SortedListEnd(list),
																CompareInts2, &t6);
	TH_ASSERT(1 == *(int *) SortedListGetData(r));

	r = SortedListFindIf(SortedListBegin(list), SortedListEnd(list),
																CompareInts2, &t6);
	r = SortedListPrev(r);
	r = SortedListPrev(r);
	TH_ASSERT(NULL == r.internal_iter);

	r = SortedListFindIf(SortedListBegin(list), SortedListEnd(list),
																CompareInts3, &t6);
	TH_ASSERT(1 == SortedListIsSameIterator(r, SortedListEnd(list)));

	SortedListDestroy(list);
}
