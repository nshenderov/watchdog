/*******************************************************************************
*
* FILENAME : pqueue.h
*
* DESCRIPTION : Priority queue is an abstract data type, where each stored
* element has an associated priority value. The priority of an element
* determines the order in which it will be processed and removed from the queue.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 07.05.2023
* 
*******************************************************************************/

#ifndef __NSRD_PQUEUE_H__
#define __NSRD_PQUEUE_H__

#include "sorted_list.h"

typedef struct pq pq_t;

/*
DESCRIPTION
    Pointer to the user's function that compares data1 and data2.
    The actual matching and types of the input are defined by the user.
    Elements will be in priority list based on this function.
RETURN
    Positive: data1 > data2.
    Negative: data1 < data2.
    0: data1 == data2.
INPUT
    data1: pointer to the user's data.
    data2: pointer to the user's data.
*/
typedef int (*pqueue_compare_func_t)(const void *data1, const void *data2);

/*
DESCRIPTION
    Pointer to the user's function that compares if the data matches a certain
    criteria, using the param. The actual matching and types of the input are
    defined by the user.
RETURN
    1: matches.
    0: not matches.
INPUT
    data: pointer to the user's data.
    param: pointer to the parameter.
*/
typedef int (*pqueue_is_match_func_t)(const void *data, void *param);

/*
DESCRIPTION
    Creates new priority queue. Queue will use pattern of sorting, based
    on user's compare function - "compare".
    Creation may fail, due to memory allocation fail.
    User is responsible for memory deallocation.
RETURN
    Pointer to the created priority queue on success.
    NULL if allocation failed.
INPUT
    compare: pointer to the compare function.
TIME COMPLEXITY:
    O(1)
*/
pq_t *PQCreate(pqueue_compare_func_t compare);

/*
DESCRIPTION
    Frees the memory allocated for each element of a priority queue and the
    queue by itself. All data inside the queue will be lost.
RETURN
    Doesn't return anything.
INPUT
    pqueue: pointer to the priority queue.
TIME COMPLEXITY:
    O(n)
*/
void PQDestroy(pq_t *pqueue);

/*
DESCRIPTION
    Inserts a new element to the priority queue on the position, according
    to saved in queue pointer on compare function. If some elements has same
    priority, new element will go after the existing one (less priority).
    Enqueue may fail, due to memory allocation fail.
RETURN
    0: success.
    1: fail.
INPUT
    pqueue: pointer to the priority queue.
    data: pointer to the user's data.
TIME COMPLEXITY:
    O(n)
*/
int PQEnqueue(pq_t *pqueue, void *data);

/*
DESCRIPTION
    Removes the element with highest priority of passed priority queue.
    Trying to dequeue empty priority queue may cause undefined behavior.
RETURN
    Pointer to user's data of removed element.
INPUT
    pqueue: pointer to the priority queue.
TIME COMPLEXITY:
    O(1)
*/
void *PQDequeue(pq_t *pqueue);

/*
DESCRIPTION
    Gets data from highest priority element.
    Data is stored in priority queue by reference.
    Trying to peek empty priority queue is undefined behavior. 
RETURN
    Pointer to user's data of highest priority element.
INPUT
    pqueue: pointer to the priority queue.
TIME COMPLEXITY:
    O(1)
*/
void *PQPeek(const pq_t *pqueue);

/*
DESCRIPTION
    Checks if priority queue is empty.
RETURN
    1: empty.
    0: is not empty.
INPUT
    pqueue: pointer to the priority queue.
TIME COMPLEXITY:
    O(1)
*/
int PQIsEmpty(const pq_t *pqueue);

/*
DESCRIPTION
    Traverses the sorted linked list and returns the amount of elements.
RETURN
    Number of elements in priority queue.
INPUT
    pqueue: pointer to the priority queue.
TIME COMPLEXITY:
    O(n)
*/
size_t PQSize(const pq_t *pqueue);

/*
DESCRIPTION
	Removes all elements from priority queue.
RETURN
    Doesn't return anything.
INPUT
    pqueue: pointer to the priority queue.
TIME COMPLEXITY:
    O(n)
*/
void PQClear(pq_t *pqueue);

/*
DESCRIPTION
	Traverses the priority queue for element that satisfies is_match 
	function's criteria. Removes the first occurance.
RETURN
	Pointer to user's data of removed element. on success.
	NULL if nothing found (user's data also might be NULL, so need to
	check PQSize to see element was found and removed).
INPUT
    pqueue: pointer to the priority queue.
    is_match: user's function that compares if the data matches a certain
    criteria.
    param: a parameter for the is_match function.
TIME COMPLEXITY:
    O(n)
*/
void *PQErase(pq_t *pqueue, pqueue_is_match_func_t is_match, void *param);

#endif  /* __NSRD_PQUEUE_H__ */ 