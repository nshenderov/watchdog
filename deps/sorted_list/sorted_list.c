/*******************************************************************************
*
* FILENAME : sorted_list.c
*
* DESCRIPTION : Sorted list implementation.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 24.04.2023
* 
*******************************************************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */

#include "sorted_list.h"


struct sorted_list
{
    dlist_t *dlist;
    sorted_list_compare_func_t compare;
};

static int IsTail(dlist_iterator_t iterator, sorted_list_t *list);
static int FindIsEqual(const void *data, void *params);
static int FindIsLargerOrEqual(const void *data, void *params);

static dlist_iterator_t TranslateSortedIterToDlistIter(
										const sorted_list_iterator_t iterator);

static sorted_list_iterator_t TranslateDlistIterToSortedIter(
											const dlist_iterator_t dlist_iter,
											const sorted_list_t *sorted_list);

static dlist_iterator_t GetPositionToInsert(const sorted_list_t *sorted_list,
											dlist_iterator_t from,
											dlist_iterator_t to,
											void *data);

sorted_list_t *SortedListCreate(sorted_list_compare_func_t comp)
{
	sorted_list_t *new_list = NULL;
	dlist_t *dlist = NULL;

	assert(NULL != comp);

	new_list = (sorted_list_t *) malloc(sizeof(sorted_list_t));
	if(NULL == new_list)
    {
        return (NULL);
    }

    dlist = DListCreate();
    if(NULL == dlist)
    {
    	free(new_list);
    	new_list = NULL;

        return (NULL);
    }

    new_list -> dlist = dlist;
    new_list -> compare = comp;

    return (new_list);
}

void SortedListDestroy(sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);
	assert(NULL != sorted_list -> dlist);

	DListDestroy(sorted_list -> dlist);

	sorted_list->dlist = NULL;

	free(sorted_list);
	sorted_list = NULL;
}

size_t SortedListSize(const sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);
	assert(NULL != sorted_list -> dlist);

	return (DListSize(sorted_list -> dlist));
}

int SortedListIsEmpty(const sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);
	assert(NULL != sorted_list -> dlist);

	return (DListIsEmpty(sorted_list -> dlist));
}

sorted_list_iterator_t SortedListInsert(sorted_list_t *sorted_list, void *data)
{
	sorted_list_iterator_t new_iterator = {0};
	dlist_iterator_t insertion_point = NULL;
	dlist_iterator_t d_from = NULL;
	dlist_iterator_t d_to = NULL;

	assert(NULL != sorted_list);
	assert(NULL != sorted_list -> dlist);

	d_from = TranslateSortedIterToDlistIter(SortedListBegin(sorted_list));
	d_to = TranslateSortedIterToDlistIter(SortedListEnd(sorted_list));

	insertion_point = GetPositionToInsert(sorted_list, d_from, d_to, data);

	insertion_point = DListInsert(insertion_point, data);

	new_iterator = TranslateDlistIterToSortedIter(insertion_point, sorted_list);

	return (new_iterator);
}

sorted_list_iterator_t SortedListRemove(sorted_list_iterator_t iterator)
{
	dlist_iterator_t iter_to_remove = NULL;

	assert(NULL != TranslateSortedIterToDlistIter(iterator));
	assert(NULL != iterator.parent_list);

	iter_to_remove = TranslateSortedIterToDlistIter(iterator);

	#ifndef NDEBUG
    iterator = TranslateDlistIterToSortedIter(DListRemove(iter_to_remove),
														iterator.parent_list);
    #else
    iterator = TranslateDlistIterToSortedIter(DListRemove(iter_to_remove),NULL);
    #endif

	return (iterator);
}

sorted_list_iterator_t SortedListBegin(const sorted_list_t *sorted_list)
{
	dlist_iterator_t dlist_begin = NULL;

	assert(NULL != sorted_list);
	assert(NULL != sorted_list->dlist);

	dlist_begin = DListBegin(sorted_list -> dlist);

	return (TranslateDlistIterToSortedIter(dlist_begin, sorted_list));
}

sorted_list_iterator_t SortedListEnd(const sorted_list_t *sorted_list)
{
	dlist_iterator_t dlist_end = NULL;

	assert(NULL != sorted_list);
	assert(NULL != sorted_list->dlist);

	dlist_end = DListEnd(sorted_list -> dlist);

	return (TranslateDlistIterToSortedIter(dlist_end, sorted_list));
}

sorted_list_iterator_t SortedListNext(sorted_list_iterator_t iterator)
{
	dlist_iterator_t dlist_next = NULL;

	assert(NULL != TranslateSortedIterToDlistIter(iterator));

	dlist_next = TranslateSortedIterToDlistIter(iterator);
	dlist_next = DListNext(dlist_next);

	#ifndef NDEBUG
    iterator = TranslateDlistIterToSortedIter(dlist_next, iterator.parent_list);
    #else
    iterator = TranslateDlistIterToSortedIter(dlist_next, NULL);
    #endif

	return (iterator);
}

sorted_list_iterator_t SortedListPrev(sorted_list_iterator_t iterator)
{
	dlist_iterator_t dlist_prev = NULL;

	assert(NULL != TranslateSortedIterToDlistIter(iterator));

	dlist_prev = TranslateSortedIterToDlistIter(iterator);
	dlist_prev = DListPrev(dlist_prev);

	#ifndef NDEBUG
    iterator = TranslateDlistIterToSortedIter(dlist_prev, iterator.parent_list);
    #else
    iterator = TranslateDlistIterToSortedIter(dlist_prev, NULL);
    #endif

	return (iterator);
}

int SortedListIsSameIterator(sorted_list_iterator_t iterator1,
								sorted_list_iterator_t iterator2)
{
	dlist_iterator_t dlist_iter1 = NULL;
	dlist_iterator_t dlist_iter2 = NULL;

	assert(NULL != TranslateSortedIterToDlistIter(iterator1));
	assert(NULL != TranslateSortedIterToDlistIter(iterator2));

	dlist_iter1 = TranslateSortedIterToDlistIter(iterator1);
	dlist_iter2 = TranslateSortedIterToDlistIter(iterator2);

    return (DListIsSameIterator(dlist_iter1, dlist_iter2));
}

void *SortedListGetData(sorted_list_iterator_t iterator)
{
	dlist_iterator_t dlist_iter = NULL;

	assert(NULL != TranslateSortedIterToDlistIter(iterator));

	dlist_iter = TranslateSortedIterToDlistIter(iterator);

	return (DListGetData(dlist_iter));
}

void *SortedListPopBack(sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);
	assert(NULL != sorted_list->dlist);

	return (DListPopBack(sorted_list -> dlist));
}    

void *SortedListPopFront(sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);
	assert(NULL != sorted_list->dlist);

	return (DListPopFront(sorted_list -> dlist));
}

int SortedListForEach(sorted_list_iterator_t from, sorted_list_iterator_t to,
                    			sorted_list_action_func_t action, void *param)
{
	dlist_iterator_t d_from = NULL;
	dlist_iterator_t d_to = NULL;

	assert(NULL != TranslateSortedIterToDlistIter(from));
	assert(NULL != TranslateSortedIterToDlistIter(to));
	assert(from.parent_list == to.parent_list);
	assert(NULL != action);
	d_from = TranslateSortedIterToDlistIter(from);
	d_to = TranslateSortedIterToDlistIter(to);

	return (DListForEach(d_from,d_to, action, param));
}

sorted_list_t *SortedListMerge(sorted_list_t *sorted_list1,
								sorted_list_t *sorted_list2)
{
	dlist_iterator_t runner_dest_from = NULL;
	dlist_iterator_t runner_dest_to = NULL;
	dlist_iterator_t runner_src_from = NULL;
	dlist_iterator_t runner_src_to = NULL;
	dlist_iterator_t dlist2_end = NULL;

	assert(NULL != sorted_list1);
	assert(NULL != sorted_list2);
    assert(sorted_list1 -> compare == sorted_list2 -> compare);

    runner_dest_from = DListBegin(sorted_list1 -> dlist);
    runner_dest_to = DListEnd(sorted_list1 -> dlist);
    runner_src_from =  DListBegin(sorted_list2 -> dlist);

    dlist2_end = DListEnd(sorted_list2 -> dlist);

    while(!IsTail(runner_dest_from, sorted_list1)
    	&& !DListIsEmpty(sorted_list2 -> dlist))
    {
		runner_dest_from = GetPositionToInsert(sorted_list1, runner_dest_from,
								runner_dest_to, DListGetData(runner_src_from));


	    runner_dest_from = DListNext(runner_dest_from);

	    if (NULL == runner_dest_from || IsTail(runner_dest_from, sorted_list1))
		{
			break;
		}

		runner_src_to = GetPositionToInsert(sorted_list2, runner_src_from,
								dlist2_end, DListGetData(runner_dest_from));

		DListSplice(runner_dest_from, runner_src_from, runner_src_to);

		runner_src_from = runner_src_to;
    }

    if (!DListIsEmpty(sorted_list2 -> dlist))
    {
    	DListSplice(runner_dest_to, runner_src_from, dlist2_end);
    }

    return (sorted_list1);
}

sorted_list_iterator_t SortedListFind(sorted_list_t *sorted_list,
										sorted_list_iterator_t from,
										sorted_list_iterator_t to, void *data)
{
	void *parameters[2] = {NULL};
	dlist_iterator_t d_from = NULL;
	dlist_iterator_t d_to = NULL;
	dlist_iterator_t founded_node = NULL;
	
	assert(NULL != TranslateSortedIterToDlistIter(from));
	assert(NULL != TranslateSortedIterToDlistIter(to));
	assert(from.parent_list == to.parent_list);

	parameters[0] = (void *) sorted_list;
	parameters[1] = data;

	d_from = TranslateSortedIterToDlistIter(from);
	d_to = TranslateSortedIterToDlistIter(to);

	founded_node = DListFind(d_from, d_to, FindIsEqual, parameters);

	return (TranslateDlistIterToSortedIter(founded_node, sorted_list));
}


sorted_list_iterator_t SortedListFindIf(sorted_list_iterator_t from,
										sorted_list_iterator_t to,
										sorted_list_is_match_func_t is_match,
										void *param)
{
	dlist_iterator_t d_from = NULL;
	dlist_iterator_t d_to = NULL;
	dlist_iterator_t founded_node = NULL;

	assert(NULL != TranslateSortedIterToDlistIter(from));
	assert(NULL != TranslateSortedIterToDlistIter(to));
	assert(from.parent_list == to.parent_list);

	d_from = TranslateSortedIterToDlistIter(from);
	d_to = TranslateSortedIterToDlistIter(to);

	founded_node = DListFind(d_from,d_to,(dlist_is_match_func_t)is_match,param);

	#ifndef NDEBUG
	return (TranslateDlistIterToSortedIter(founded_node, from.parent_list));
    #else
    return (TranslateDlistIterToSortedIter(founded_node, NULL));
    #endif
}

static int FindIsLargerOrEqual(const void *data, void *params)
{
	void **parameters = params;
	sorted_list_t *list = *parameters;
	const void *data_new = *(parameters + 1);
	
	return (-1 != list -> compare(data, data_new));
}

static int FindIsEqual(const void *data, void *params)
{
	void **parameters = params;
	sorted_list_t *list = *parameters;
	const void *data_new = *(parameters + 1);
	
	return (0 == list -> compare(data, data_new));
}

static dlist_iterator_t GetPositionToInsert(const sorted_list_t *sorted_list,
											dlist_iterator_t from,
											dlist_iterator_t to,
											void *data)
{
	void *parameters[2] = {NULL};

	assert(NULL != sorted_list -> dlist);

	parameters[0] = (void *) sorted_list;
	parameters[1] = data;

	return (DListFind(from, to, FindIsLargerOrEqual, parameters));
}

static dlist_iterator_t TranslateSortedIterToDlistIter(
										const sorted_list_iterator_t iterator)
{
	return (iterator.internal_iter);
}

static sorted_list_iterator_t TranslateDlistIterToSortedIter(
											const dlist_iterator_t dlist_iter,
											const sorted_list_t *sorted_list)
{
	sorted_list_iterator_t new_iterator = {0};

	assert(NULL != sorted_list);

	new_iterator.internal_iter = dlist_iter;

	#ifndef NDEBUG
    new_iterator.parent_list = (sorted_list_t *) sorted_list;
    #endif

    return (new_iterator);

    #ifdef NDEBUG
    (void) sorted_list;
    #endif
}

static int IsTail(dlist_iterator_t iterator, sorted_list_t *list)
{
	assert(NULL != list);
	assert(NULL != iterator);

    return (DListIsSameIterator(DListEnd(list -> dlist), iterator));
}
