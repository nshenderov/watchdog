/*******************************************************************************
*
* FILENAME : pqueue.h
*
* DESCRIPTION : Interface of a priority queue data structure.
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

/*******************************************************************************
*   DESCRIPTION:
*       A pointer to a function that compares two elements. This function
*       is used to properly sort elements by their priority.
*       The actual matching and types of the input is defined by the user.
*   RETURN:
*       1 - priority of data1 is bigger than data2.
*       0 - priorities are equal.
*       -1 - priority of data1 is less than data2.
*   INPUT:
*       data: a pointer to a value;
*       param: a pointer to a parameter.    
*******************************************************************************/
typedef int (*pqueue_compare_func_t)(const void *data1, const void *data2);

/*******************************************************************************
*   DESCRIPTION:
*       A pointer to a function that validates if the data matches a certain 
*       criteria using the param.
*       The actual matching and types of the input is defined by the user.
*   RETURN:
*       1 true - if the criteria is matched.
*       0 false - if it's not
*   INPUT:
*       data: a pointer to some value;
*       param: a pointer to a parameter used during the comparison.    
*******************************************************************************/
typedef int (*pqueue_is_match_func_t)(const void *data, void *param);

/*******************************************************************************
*   DESCRIPTION:
*       Creates a priority queue.
*       Creation may fail, due to memory allocation fail.
*       User is responsible for destroying the priority queue.
*   RETURN:
*       Returns pointer to the created sorted linked list on success.
*       Returns NULL on failure.
*   INPUT:
*       compare: a comparison function used to determine element's priority and
*       properly sort it inside a queue.
*   TIME COMPLEXITY:
*       O(1)
*******************************************************************************/
pq_t *PQCreate(pqueue_compare_func_t compare);

/*******************************************************************************
*   DESCRIPTION:
*       Frees the memory allocated for pqueue.
*   RETURN:
*       There is no return for this function.
*   INPUT:
*       pqueue: a pointer to a priority queue.
*   TIME COMPLEXITY:
*       O(n)
*******************************************************************************/
void PQDestroy(pq_t *pqueue);

/*******************************************************************************
*   DESCRIPTION:
*       Enqueues a new element to a priority queue.
*       Passing not valid data may cause undefined behavior.
*   RETURN:
*       0 - on success.
*       1 - if insertion failed.
*   INPUT:
*       pqueue: a pointer to a priority queue.
*       data: pointer to the user's data.
*   TIME COMPLEXITY:
*       O(n)
*******************************************************************************/
int PQEnqueue(pq_t *pqueue, void *data);

/*******************************************************************************
*   DESCRIPTION:
*       Dequeues an element with highest priority from priority queue.
*   RETURN:
*       The data of dequeued element.
*   INPUT:
*       pqueue: a pointer to a priority queue.
*   TIME COMPLEXITY:
*       O(1)
*******************************************************************************/
void *PQDequeue(pq_t *pqueue);

/*******************************************************************************
*   DESCRIPTION:
*       Returns data of the element with highest priority.
*   RETURN:
*       The pointer to a data.
*   INPUT:
*       pqueue: a pointer to a priority queue.
*   TIME COMPLEXITY:
*       O(1)
*******************************************************************************/
void *PQPeek(const pq_t *pqueue);

/*******************************************************************************
*   DESCRIPTION:
*       Checks a priority queue whether it is empty or not.
*   RETURN:
*       1 - Empty.
*       0 - Non empty.
*   INPUT:
*       pqueue: a pointer to a priority queue.
*   TIME COMPLEXITY:
*       O(1)
*******************************************************************************/
int PQIsEmpty(const pq_t *pqueue);

/*******************************************************************************
*   DESCRIPTION:
*       Count the current number of elements in a priority queue.
*   RETURN:
*       The number of elements in the priority queue.
*   INPUT:
*       pqueue: a pointer to a priority queue.
*   TIME COMPLEXITY:
*       O(n)
*******************************************************************************/
size_t PQSize(const pq_t *pqueue);

/*******************************************************************************
*   DESCRIPTION:
*       Clears a priority queue.
*   RETURN:
*       There is no return for this function.
*   INPUT:
*       pqueue: a pointer to a priority queue.
*   TIME COMPLEXITY:
*       O(n)
*******************************************************************************/
void PQClear(pq_t *pqueue);


/*******************************************************************************
*   DESCRIPTION:
*       Removes specific element determined by arbitrary logic from a priority
*       queue.
*   RETURN:
*       Ponter to the data of the erased element.
*       NULL in case of failure.
*   INPUT:
*       pqueue: a pointer to a priority queue.
*       is_match: comparisson function.
*       param: pointer to a parameter to pass to is_match function.
*   TIME COMPLEXITY:
*       O(n)
*******************************************************************************/
void *PQErase(pq_t *pqueue, pqueue_is_match_func_t is_match, void *param);

#endif  /* __NSRD_PQUEUE_H__ */ 