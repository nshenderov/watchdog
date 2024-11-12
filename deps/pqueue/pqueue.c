/*******************************************************************************
*
* FILENAME : pqueue.c
*
* DESCRIPTION : Priority queue implementation.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 07.05.2023
* 
*******************************************************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* free, malloc */

#include "pqueue.h"

enum {SUCCESS, FAILURE};

struct pq
{
    sorted_list_t *sorted_list;
};

pq_t *PQCreate(pqueue_compare_func_t compare)
{
    pq_t *new_pqueue = NULL;
    sorted_list_t *new_list = NULL;

    assert(NULL != compare);

    new_pqueue = (pq_t *) malloc(sizeof(pq_t));
    if (NULL == new_pqueue)
    {
        return (NULL);
    }

    new_list = SortedListCreate(compare);
    if (NULL == new_list)
    {
        free(new_pqueue);
        new_pqueue = NULL;
        return (NULL);
    }

    new_pqueue->sorted_list = new_list;

    return (new_pqueue);
}

void PQDestroy(pq_t *pqueue)
{
    assert(NULL != pqueue);
    assert(NULL != pqueue->sorted_list);

    SortedListDestroy(pqueue->sorted_list);
    pqueue -> sorted_list = NULL;

    free(pqueue);
    pqueue = NULL;
}

int PQEnqueue(pq_t *pqueue, void *data)
{
    sorted_list_t *list = NULL;
    sorted_list_iterator_t result = {0};
    sorted_list_iterator_t tail = {0};

    assert(NULL != pqueue);
    assert(NULL != pqueue->sorted_list);
    assert(NULL != data);

    list = pqueue->sorted_list;
    tail = SortedListEnd(list);
    result = SortedListInsert(list, data);

    if (SortedListIsSameIterator(result, tail))
    {
        return (FAILURE);
    }

    return (SUCCESS);
}

void *PQDequeue(pq_t *pqueue)
{
    assert(NULL != pqueue);
    assert(NULL != pqueue->sorted_list);

    return (SortedListPopBack(pqueue->sorted_list));
}

void *PQPeek(const pq_t *pqueue)
{
    sorted_list_iterator_t highest_node = {0};

    assert(NULL != pqueue);
    assert(NULL != pqueue->sorted_list);

    highest_node = SortedListPrev(SortedListEnd(pqueue->sorted_list));

    return (SortedListGetData(highest_node));
}

int PQIsEmpty(const pq_t *pqueue)
{
    assert(NULL != pqueue);
    assert(NULL != pqueue->sorted_list);

    return (SortedListIsEmpty(pqueue->sorted_list));
}

size_t PQSize(const pq_t *pqueue)
{
    assert(NULL != pqueue);
    assert(NULL != pqueue->sorted_list);
    
    return (SortedListSize(pqueue->sorted_list)); 
}

void PQClear(pq_t *pqueue)
{
    sorted_list_t *list = NULL;

    assert(NULL != pqueue);
    assert(NULL != pqueue->sorted_list);

    list = pqueue->sorted_list;

    while(!SortedListIsEmpty(list))
    {
        SortedListPopBack(list);
    }
}

void *PQErase(pq_t *pqueue, pqueue_is_match_func_t is_match, void *param)
{
    sorted_list_t *list = NULL;
    sorted_list_iterator_t begin = {0};
    sorted_list_iterator_t tail = {0};
    sorted_list_iterator_t element_to_remove = {0};
    void *removed_element_data = NULL;

    assert(NULL != pqueue);
    assert(NULL != pqueue->sorted_list);
    assert(NULL != is_match);
    assert(NULL != param);

    list = pqueue->sorted_list;
    begin = SortedListBegin(list);
    tail = SortedListEnd(list);

    element_to_remove = SortedListFindIf(begin, tail, is_match, param);
    if (SortedListIsSameIterator(tail, element_to_remove))
    {
        return (NULL);
    }

    removed_element_data = SortedListGetData(element_to_remove);
    SortedListRemove(element_to_remove);

    return (removed_element_data);
}