/*******************************************************************************
* FILENAME : dlist_test.c
*
* AUTHOR : Nick Shenderov
* 
* DATE : 22.04.2023
*
*******************************************************************************/

#include <stdio.h> /* printf */

#include "dlist.h"

typedef struct dlist_node dlist_node_t;

struct dlist_node
{
	void *data;
	dlist_node_t *next;
	dlist_node_t *prev;
};

struct dlist
{
	dlist_node_t head;
	dlist_node_t tail;
};

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define NC "\033[0m"

static const char *ok = GREEN"ok"NC;
static const char *not_ok = RED"not ok"NC;

static void PrintTestFailed(char *name, int line, long exp, long got)
{
	fprintf( stderr, RED"\n## %s failed on line %d ##\n"NC, name, line);
	fprintf( stderr, "EXPECTED: %ld\n", exp);
	fprintf( stderr, "GOT: %ld\n", got);
}

static int TestList(dlist_t *list);

static int EqualsInt(const void *data, void *param)
{
	if (*(int *) data == *(int *) param)
	{
		return (1);
	}

	return (0);
}

static int EqualsTest2(const void *data, void *param)
{
	if (*(double *) data == 3.22 || *(int *) data == 123)
	{
		return (1);
	}

	return (0);
	(void) param;
}

static int Add(void *data, void *param)
{
	*(int *) data += *(int *) param;
	return (0);
}


int main()
{
	dlist_t *list1 = DListCreate();
	dlist_t *list2 = DListCreate();

	printf("List1 is %s\n", TestList(list1) ? not_ok : ok);
	printf("List2 is %s\n", TestList(list2) ? not_ok : ok);

	return 0;
}


static int TestList(dlist_t *list)
{
	int t1 = 123;
	int t1_res = 0;
	float t2 = 99.123;
	float t2_res = 0.0;
	double t3 = 3.22;
	double t3_res = 0.0;
	int t4 = 0;
	int t5 = 100;
	int t6 = 20200;

	dlist_iterator_t list_begin = NULL;
	dlist_iterator_t list_head_next = NULL;
	dlist_iterator_t list_end = NULL;
	dlist_iterator_t search_res = NULL;

	dlist_t *list_of_res = DListCreate();
	dlist_t *list_for_slice = DListCreate();

	if (NULL == list || NULL == &list -> tail || NULL == &list -> head)
	{
		PrintTestFailed("Test ListCreate", __LINE__, 1, 0);
		return (1);
	}

	printf("Test ListCreate %s\n", ok);

	list_begin = DListBegin(list);

	if (list -> head.next != list_begin)
	{
		PrintTestFailed("Test ListBegin", __LINE__, 1, 0);
		return (1);
	}

	printf("Test ListBegin %s\n", ok);

	list_begin = DListPushFront(list, &t1);
	t1_res = *(int *) DListGetData(list_begin);
	if (t1 != t1_res)
	{
		PrintTestFailed("Test ListPushFront", __LINE__, t1, t1_res);
		return (1);
	}

	t1_res = *(int *) DListPopFront(list);
	if (t1 != t1_res)
	{
		PrintTestFailed("Test ListPushFront", __LINE__, t1, t1_res);
		return (1);
	}

	list_begin = DListBegin(list);

	printf("Test ListPushFront ListPopFront %s\n", ok);

	list_end = DListPushBack(list, &t1);
	t1_res = *(int *) DListGetData(list_end);
	if (t1 != t1_res)
	{
		PrintTestFailed("Test ListPushBack", __LINE__, t1, t1_res);
		return (1);
	}

	t1_res = *(int *) DListPopBack(list);
	if (t1 != t1_res)
	{
		PrintTestFailed("Test ListPushBack", __LINE__, t1, t1_res);
		return (1);
	}

	list_end = DListEnd(list);

	printf("Test ListPushBack ListPopBack %s\n", ok);

	list_begin = DListInsert(list_begin, &t1);
	t1_res = *(int *) DListGetData(list_begin);
	if (t1 != t1_res)
	{
		PrintTestFailed("Test ListInsert ListGetData", __LINE__, t1, t1_res);
		return (1);
	}

	printf("Test ListInsert ListGetData %s\n", ok);

	list_begin = DListInsert(list_begin, &t2);
	t2_res = *(float *) DListGetData(list_begin);
	if (t2 != t2_res)
	{
		PrintTestFailed("Test ListInsert ListGetData", __LINE__, t2, t2_res);
		return (1);
	}

	printf("Test ListInsert ListGetData %s\n", ok);

	list_begin = DListInsert(list_begin, &t3);
	t3_res = *(double *) DListGetData(list_begin);
	if (t3 != t3_res)
	{
		PrintTestFailed("Test ListInsert ListGetData", __LINE__, t3, t3_res);
		return (1);
	}

	printf("Test ListInsert ListGetData %s\n", ok);

	if (3 != DListSize(list))
	{
		PrintTestFailed("Test ListSize", __LINE__, 3, DListSize(list));
		return (1);
	}

	printf("Test ListSize %s\n", ok);

	list_end = DListEnd(list);
	if (&list -> tail != list_end)
	{
		PrintTestFailed("Test ListEnd", __LINE__, 1, 0);
		return (1);
	}

	printf("Test ListEnd %s\n", ok);

	list_end = DListInsert(list_end, &t1);
	t1_res = *(int *) DListGetData(list_end);
	if (t1 != t1_res)
	{
		PrintTestFailed("Test insertion to the end", __LINE__, t1, t1_res);
		return (1);
	}
	if (NULL != list_end -> next -> next)
	{
		PrintTestFailed("Test insertion to the end", __LINE__, 0, 1);
		return (1);
	}

	if (4 != DListSize(list))
	{
		PrintTestFailed("Test ListSize", __LINE__, 4, DListSize(list));
		return (1);
	}

	printf("Test insertion to the end %s\n", ok);

	list_head_next = DListNext(list_begin);
	if (t2 != *(float *) DListGetData(list_head_next))
	{
		PrintTestFailed("Test ListNext", __LINE__, t2, *(float *) DListGetData(list_head_next));
		return (1);
	}

	list_head_next = DListNext(list_end);
	if (NULL != list_head_next -> next)
	{
		PrintTestFailed("Test ListNext", __LINE__, 0, 1);
		return (1);
	}

	printf("Test ListNext %s\n", ok);

	list_begin = DListRemove(list_begin);
	if (3 != DListSize(list))
	{
		PrintTestFailed("Test ListRemove", __LINE__, 3, DListSize(list));
		return (1);
	}
	t2_res = *(float *) DListGetData(list_begin);
	if (t2 != t2_res)
	{
		PrintTestFailed("Test ListRemove", __LINE__, t2, t2_res);
		return (1);
	}

	list_begin = DListRemove(list_begin);

	if (2 != DListSize(list))
	{
		PrintTestFailed("Test ListRemove", __LINE__, 2, DListSize(list));
		return (1);
	}
	t1_res = *(int *) DListGetData(list_begin);
	if (t1 != t1_res)
	{
		PrintTestFailed("Test ListRemove", __LINE__, t1, t1_res);
		return (1);
	}

	list_begin = DListRemove(list_begin);
	list_begin = DListRemove(list_begin);

	if (!DListIsEmpty(list))
	{
		PrintTestFailed("Test ListRemove", __LINE__, 1, DListIsEmpty(list));
		return (1);
	}

	printf("Test ListRemove ListIsEmpty %s\n", ok);

	for (; t4 < 200; ++t4)
	{
		list_begin = DListInsert(list_begin, &t4);
	}
	if (200 != DListSize(list))
	{
		PrintTestFailed("Test ListForEach ListSetData  ListFind",
				 __LINE__, 200, DListSize(list));
		return (1);
	}
	list_end = DListEnd(list);
	DListForEach(list_begin, list_end, Add, &t5);
	search_res = DListFind(list_begin, list_end, EqualsInt, &t6);
	if (t6 != *(int *) search_res -> data)
	{
		PrintTestFailed("Test ListForEach ListSetData  ListFind",
				 __LINE__, t6, *(int *) search_res -> data);
		return (1);
	}
	if (NULL == search_res -> next)
	{
		PrintTestFailed("Test ListForEach ListSetData  ListFind",
				 __LINE__, 1, 0);
		return (1);
	}
	printf("Test ListForEach ListFind %s\n", ok);

	DListSetData(list_begin, &t1);
	if (t1 != *(int *) DListGetData(list_begin))
	{
		PrintTestFailed("Test ListForEach ListSetData  ListFind",
				 __LINE__, t1, *(int *) DListGetData(list_begin));
		return (1);
	}
	DListSetData(list_begin, &t2);
	if (t2 != *(float *) DListGetData(list_begin))
	{
		PrintTestFailed("Test ListForEach ListSetData  ListFind",
				 __LINE__, t2, *(float *) DListGetData(list_begin));
		return (1);
	}
	search_res = DListFind(list_begin, list_end, EqualsInt, &t2);
	if (t2 != *(float *) DListGetData(search_res))
	{
		PrintTestFailed("Test ListForEach ListSetData  ListFind",
				 __LINE__, t2, *(float *) DListGetData(search_res));
		return (1);
	}
	search_res = DListFind(list_begin, list_end, EqualsInt, &t3);
	if (NULL != search_res -> next)
	{
		PrintTestFailed("Test ListForEach ListSetData  ListFind",
				 __LINE__, 1, 0);
		return (1);
	}

	printf("Test ListSetData ListFind %s\n", ok);

	list_begin = DListEnd(list);
	list_end = DListEnd(list);

	if (1 != DListIsSameIterator(list_begin, list_end))
	{
		PrintTestFailed("Test ListIsSameIterator", __LINE__, 1, 0 );
		return (1);
	}

	printf("Test ListIsSameIterator %s\n", ok);

	list_begin = DListBegin(list);

	DListSetData(list_begin, &t1);
	DListSetData(list_begin -> next, &t1);
	DListSetData(list_begin -> next -> next, &t3);

	if (3 != DListMultiFind(list_begin, list_end, list_of_res, EqualsTest2, &t1))
	{
		printf("%f\n", t2);
		PrintTestFailed("Test ListMultiFind", __LINE__, 3,
			DListMultiFind(list_begin, list_end, list_of_res, EqualsTest2, &t1));
		return (1);
	}

	if (3 != DListSize(list_of_res))
	{
		PrintTestFailed("Test ListMultiFind", __LINE__, 3, DListSize(list_of_res));
		return (1);
	}

	list_begin = DListBegin(list_of_res);
	if (t1 != *(int *) DListGetData(list_begin))
	{
		PrintTestFailed("Test ListMultiFind",
				 __LINE__, t1, *(int *) DListGetData(list_begin));
		return (1);
	}
	if (t1 != *(int *) DListGetData(list_begin -> next))
	{
		PrintTestFailed("Test ListMultiFind",
				 __LINE__, t1, *(int *) DListGetData(list_begin));
		return (1);
	}
	if (t3 != *(double *) DListGetData(list_begin -> next -> next))
	{
		PrintTestFailed("Test ListMultiFind",
				 __LINE__, t1, *(double *) DListGetData(list_begin -> next -> next));
		return (1);
	}
	printf("Test ListMultiFind %s\n", ok);


	list_begin = DListBegin(list_for_slice);
	list_begin = DListInsert(list_begin, &t1);
	list_begin = DListInsert(list_begin, &t2);
	list_begin = DListInsert(list_begin, &t3);

	list_begin = DListSplice(DListBegin(list), DListBegin(list_for_slice), DListEnd(list_for_slice));
	if (t3 != *(double *) DListGetData(list_begin))
	{
		PrintTestFailed("Test ListSplice",
				 __LINE__, t3, *(double *) DListGetData(list_begin));
		return (1);
	}

	list_begin = list_begin -> next;
	if (t2 != *(float *) DListGetData(list_begin))
	{
		PrintTestFailed("Test ListSplice",
				 __LINE__, t2, *(float *) DListGetData(list_begin));
		return (1);
	}

	list_begin = DListBegin(list);
	if (t3 != *(double *) DListGetData(list_begin))
	{
		PrintTestFailed("Test ListSplice",
				 __LINE__, t3, *(double *) DListGetData(list_begin));
		return (1);
	}


	list_begin = list_begin -> next -> next;
	if (t1 != *(int *) DListGetData(list_begin))
	{
		PrintTestFailed("Test ListSplice",
				 __LINE__, t1, *(int *) DListGetData(list_begin));
		return (1);
	}

	if (!DListIsEmpty(list_for_slice))
	{
		PrintTestFailed("Test ListSplice", __LINE__, 1, DListIsEmpty(list_for_slice));
		return (1);
	}

	list_begin = DListBegin(list_for_slice);
	list_begin = DListInsert(list_begin, &t1);
	list_begin = DListInsert(list_begin, &t2);
	list_begin = DListInsert(list_begin, &t3);

	list_begin = DListSplice(DListEnd(list), DListBegin(list_for_slice), DListEnd(list_for_slice));
	if (t3 != *(double *) DListGetData(list_begin))
	{
		PrintTestFailed("Test ListSplice",
				 __LINE__, t3, *(double *) DListGetData(list_begin));
		return (1);
	}
	
	if (!DListIsEmpty(list_for_slice))
	{
		PrintTestFailed("Test ListSplice", __LINE__, 1, DListIsEmpty(list_for_slice));
		return (1);
	}

	list_begin = list_begin -> next;
	if (t2 != *(float *) DListGetData(list_begin))
	{
		PrintTestFailed("Test ListSplice",
				 __LINE__, t2, *(float *) DListGetData(list_begin));
		return (1);
	}

	list_begin = list_begin -> next;
	if (t1 != *(int *) DListGetData(list_begin))
	{
		PrintTestFailed("Test ListSplice",
				 __LINE__, t1, *(int *) DListGetData(list_begin));
		return (1);
	}

	list_begin = list_begin -> next;
	if (NULL != list_begin -> next)
	{
		PrintTestFailed("Test ListSplice", __LINE__, 0, 1);
		return (1);
	}

	printf("Test ListSplice %s\n", ok);

	DListDestroy(list);
	DListDestroy(list_of_res);
	DListDestroy(list_for_slice);

	printf("%s\n", GREEN"Passed all"NC);

	return 0;
}