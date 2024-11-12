/*******************************************************************************
* FILENAME : dlist.c
*
* DESCRIPTION : Doubly linked list implementation.
* 
* AUTHOR : Nick Shenderov
* 
* DATE : 22.04.2023
*
*******************************************************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */

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

#define DUMMY_DATA ((void *) 0xDEADBEEF)

#define FREE_MEMORY(ptr) \
{free(ptr); (ptr) = NULL;}


static dlist_node_t *CreateNode(void *data, dlist_iterator_t next, dlist_iterator_t prev);
static dlist_iterator_t GetPointerToTail(dlist_iterator_t iterator);
static int NodeCounter(void *data, void *param);
static int IsTail(dlist_iterator_t iterator);

dlist_t *DListCreate(void)
{
    dlist_t *new_list = (dlist_t *) malloc(sizeof(dlist_t));
    if (NULL == new_list)
    {
        return (NULL);
    }

    new_list -> head.data = DUMMY_DATA;
    new_list -> head.next = &new_list -> tail;
    new_list -> head.prev = NULL;

    new_list -> tail.data = DUMMY_DATA;
    new_list -> tail.next = NULL;
    new_list -> tail.prev = &new_list -> head;

    return (new_list);
}

void DListDestroy(dlist_t *dlist)
{
	dlist_node_t *runner = NULL;
    dlist_node_t *tmp = NULL;

    assert(NULL != dlist);

    runner = dlist -> head.next;

    while(NULL != runner -> next)
    {
        tmp = runner;
        runner = runner -> next;
        FREE_MEMORY(tmp);
    }

    FREE_MEMORY(dlist);
}

dlist_iterator_t DListInsert(dlist_iterator_t iterator, void *data)
{
	dlist_node_t *new_node = NULL;

	assert(NULL != iterator);

	new_node = CreateNode(data, iterator, iterator -> prev);
    if (NULL == new_node)
    {
        return (GetPointerToTail(iterator));
    }

    iterator -> prev -> next = new_node;
    iterator -> prev = new_node;

    return (new_node);
}

dlist_iterator_t DListRemove(dlist_iterator_t iterator)
{
	dlist_node_t *node_to_return = NULL;

	assert(NULL != iterator);

	node_to_return = iterator -> next;

	iterator -> next -> prev = iterator -> prev;
	iterator -> prev -> next = iterator -> next;

    FREE_MEMORY(iterator);

    return (node_to_return);
}

dlist_iterator_t DListFind(dlist_iterator_t from, dlist_iterator_t to,
    								dlist_is_match_func_t is_match, void *param)
{
	dlist_node_t *from_runner = NULL;
    dlist_node_t *to_runner = NULL;
   
   assert(NULL != from);
   assert(NULL != to);
   assert(NULL != is_match);

    from_runner = from;
    to_runner = to;

    while(!DListIsSameIterator(from_runner, to_runner))
    {
        if (is_match(from_runner -> data, param))
        {
            return (from_runner);
        }

        from_runner = from_runner -> next;
    }

    return (to_runner);
} 

size_t DListMultiFind(dlist_iterator_t from, dlist_iterator_t to,
    		dlist_t *output_list, dlist_is_match_func_t is_match, void *param)
{
	dlist_node_t *occurence_runner = NULL;
    dlist_node_t *to_runner = NULL;
    size_t counter = 0;

	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != output_list);
	assert(NULL != is_match);

	occurence_runner = DListFind(from, to, is_match, param);
    to_runner = to;

    while(occurence_runner != to_runner)
    {
    	if (IsTail(DListInsert(&output_list -> tail, occurence_runner -> data)))
    	{
    		return (counter);
    	}

    	++counter;

    	occurence_runner = 
    	DListFind(occurence_runner -> next, to_runner, is_match, param);
    }

   return (counter);
}

size_t DListSize(const dlist_t *dlist)
{
    size_t counter = 0;

    assert(NULL != dlist);

    DListForEach(DListBegin(dlist), DListEnd(dlist),
												NodeCounter, &counter);

    return (counter);
}

int DListForEach(dlist_iterator_t from, dlist_iterator_t to, 
                       			dlist_action_func_t action, void *param)
{
    dlist_node_t *from_runner = NULL;
    dlist_node_t *to_runner = NULL;

   assert(NULL != from);
   assert(NULL != to);
   assert(NULL != action);

    from_runner = from;
    to_runner = to;
    while(!DListIsSameIterator(from_runner, to_runner))
    {
        if (0 != action(from_runner -> data, param))
        {
            return (1);
        }
        from_runner = from_runner -> next;
    }

    return (0);
}

int DListIsSameIterator(dlist_iterator_t iterator1, 
                                            dlist_iterator_t iterator2)
{
    assert(NULL != iterator1);
    assert(NULL != iterator2);

    if (iterator1 -> data == iterator2 -> data 
        && iterator1 -> next == iterator2 -> next
        && iterator1 -> prev == iterator2 -> prev)
    {
        return (1);
    }

    return (0);
}

int DListIsEmpty(const dlist_t *dlist)
{
	assert(NULL != dlist);
	return (dlist -> head.next == &dlist -> tail);
}

void *DListGetData(dlist_iterator_t iterator)
{
	assert(NULL != iterator);
	return (iterator -> data);
}

void DListSetData(dlist_iterator_t iterator, void *data)
{
	assert(NULL != iterator);
	iterator -> data = data;
}

dlist_iterator_t DListBegin(const dlist_t *dlist)
{
	assert(NULL != dlist);
	return (dlist -> head.next);
}

dlist_iterator_t DListEnd(const dlist_t *dlist)
{
	assert(NULL != dlist);
	return ((dlist_iterator_t) &dlist -> tail);
}

dlist_iterator_t DListNext(dlist_iterator_t iterator)
{
	assert(NULL != iterator);
	return (iterator -> next);
}

dlist_iterator_t DListPrev(dlist_iterator_t iterator)
{
	assert(NULL != iterator);
	return (iterator -> prev);
}

dlist_iterator_t DListPushFront(dlist_t * dlist, void *data)
{
	assert(NULL != dlist);
	return (DListInsert(DListBegin(dlist), data));
}
void *DListPopFront(dlist_t *dlist)
{
	void *data_of_removed_element = NULL;

	assert(NULL != dlist);

	data_of_removed_element =
	dlist -> head.next -> data;

	DListRemove(DListBegin(dlist));

	return (data_of_removed_element);
}
dlist_iterator_t DListPushBack(dlist_t *dlist, void *data)
{
	assert(NULL != dlist);
	return (DListInsert(DListEnd(dlist), data));
}
void *DListPopBack(dlist_t *dlist)
{
	void *data_of_removed_element = NULL;

	assert(NULL != dlist);

	data_of_removed_element = 
	dlist -> tail.prev -> data;

	DListRemove(DListEnd(dlist) -> prev);

	return (data_of_removed_element);
}

dlist_iterator_t DListSplice(dlist_iterator_t where,
													dlist_iterator_t from,
													dlist_iterator_t to)
{
	dlist_node_t *dest_start = NULL;
	dlist_node_t *dest_end = where;
	dlist_node_t *src_start = NULL;
	dlist_node_t *src_end = to;
	dlist_node_t *splice_start = from;
	dlist_node_t *splice_end = NULL;

	assert(NULL != where);
	assert(NULL != from);
	assert(NULL != to);

	dest_start = where -> prev;
	src_start = from -> prev; 
	splice_end = src_end -> prev;

	src_start -> next = src_end;
	src_end -> prev = src_start;

	splice_start -> prev = dest_start;
	dest_start -> next = splice_start;
	splice_end -> next = dest_end;
	dest_end -> prev = splice_end;

	return (from);
}

static dlist_node_t *CreateNode(void *data,
														dlist_iterator_t next,
														dlist_iterator_t prev)
{
	dlist_node_t *new_node = NULL;

	assert(NULL != next);
	assert(NULL != prev);

    new_node = (dlist_node_t *) malloc(sizeof(dlist_node_t));
    if (NULL == new_node)
    {
        return (NULL);
    }

    new_node -> data = data;
    new_node -> next = next;
    new_node -> prev = prev;

    return (new_node);
}

static int NodeCounter(void *data, void *param)
{
	assert(NULL != param);

    *(size_t *) param += 1;

    return (0);
    (void) data;
}

static dlist_iterator_t GetPointerToTail(dlist_iterator_t iterator)
{
    assert(NULL != iterator);   

    while (NULL != iterator -> next)
    {
        iterator = iterator -> next;
    }
    
    return (iterator);
}

static int IsTail(dlist_iterator_t iterator)
{
    return (NULL == iterator -> next);
}