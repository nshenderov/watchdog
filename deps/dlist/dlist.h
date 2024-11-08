/*******************************************************************************
*
* FILENAME : dlist.h
*
* DESCRIPTION : Interface of a doubly linked list data structure.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 22.04.2023
* 
*******************************************************************************/

#ifndef __NSRD_DLIST_H__
#define __NSRD_DLIST_H__

#include <stddef.h> /* size_t */

typedef struct dlist dlist_t;
typedef struct dlist_node *dlist_iterator_t;

/*
DESCRIPTION
    A pointer to a function that executes an action on data using the param. 
    The actual action and types of the input are defined by the user.
RETURN
    0 - if success
    non-zero value - if failure
INPUT
    data: a pointer to user's data.
    param: a pointer to a parameter.
*/
typedef int (*dlist_action_func_t)(void *data, void *param);

/*
DESCRIPTION
    A pointer to a function that validates if the data matches a certain 
    criteria using the param.
    The actual matching and types of the input is defined by the user.
RETURN
    1 true - if the criteria is matched.
    0 false - if it's not
INPUT
    data: a pointer to user's data.
    param: a pointer to a parameter.    
*/
typedef int (*dlist_is_match_func_t)(const void *data, void *param);

/*
DESCRIPTION
    Creates a doubly  linked list.
    Creation may fail, due to memory allocation fail. 
    User is responsible for destroying the linked list.
RETURN
    Returns pointer to the created linked list on success.
    Returns NULL on failure.
INPUT
    Doesn't recieve an input from the user.
*/
dlist_t *DListCreate(void); /* O(1) */

/*
DESCRIPTION
    Frees the memory allocated for each element of a doubly linked list.
RETURN
    There is no return for this function.
INPUT
    dlist: a pointer to a doubly linked list.
*/
void DListDestroy(dlist_t *dlist); /* O(n) */

/*
DESCRIPTION
    Traverses the doubly linked list and returns the amount of elements.
RETURN
    The amount of elements currently in the list. 
INPUT
    dlist: a pointer to a doubly linked list.
*/
size_t DListSize(const dlist_t *dlist); /* O(n) */  

/*
DESCRIPTION
    Checks if doubly linked list is empty.
RETURN
    1 true - if the list is empty.
    0 false - if it's not
INPUT
    dlist: a pointer to a doubly linked list.
*/
int DListIsEmpty(const dlist_t *dlist); /* O(1) */

/*
DESCRIPTION
    Searches in a doubly linked list in a range for the element that satisfies
    is_match function and returns the first occurance or the iterator 
    representing the "to" element of the list if the elemnt isn't found. 
    The "to" element is marks the end of the searched range 
    but is not included in it.
RETURN
    If found -- the iterator representing the first element in the range that 
    contains the value when it finds the value.
    If not -- the iterator representing the ending element.
INPUT
    from: iterator representing the element that starts the range;
    to: iterator marking the end of the range (isn't a part of the range);
    is_match: a function that checks values
    param: a parameter for the is_match function.
*/
dlist_iterator_t DListFind(dlist_iterator_t from, dlist_iterator_t to,
                                    dlist_is_match_func_t is_match, void *param); /* O(n) */ 

/*
DESCRIPTION
    Searches in a doubly linked list in a range for the elements that satisfy
    is_match function and fills up passed list with those elements from first
    to last occuerence.
RETURN
    The number of elements founded.
INPUT
    from: iterator representing the element that starts the range;
    to: iterator marking the end of the range (isn't a part of the range);
    output_list: doubly linked list to fill up with founded elements;
    is_match: a function that checks values
    param: a parameter for the is_match function.
*/
size_t DListMultiFind(dlist_iterator_t from, dlist_iterator_t to,
            dlist_t *output_list, dlist_is_match_func_t is_match, void *param); /* O(n) */

/*
DESCRIPTION
    Traverses the linked list from one point to another and performs 
    a specific action specified by the user. The "to" element is marks 
    the end of the searched range but is not included in it.
    Actions performed might fail. 
RETURN
    0: if no actions fail; 
    non-zero value: if an action failed. 
INPUT
    from: iterator representing the element that starts the range;
    to: iterator marking the end of the range (isn't a part of the range);
    action: a pointer to an action function;
    param: a parameter for the action function.
*/
int DListForEach(dlist_iterator_t from, dlist_iterator_t to, 
                                dlist_action_func_t action, void *param); /* O(n) */

/*
DESCRIPTION
    Inserts a new element to a doubly linked list in a position.
RETURN
    The address of the new iterator if success.
    The iterator corresponding to the last element if insertion failed.
INPUT
    iterator: iterator to the newly inserted data
    data: pointer to the user's data.
*/
dlist_iterator_t DListInsert(dlist_iterator_t iterator, void *data); /* O(n) */

/*
DESCRIPTION
    Removes the element represented by iterator from a doubly linked
    list and returns the iterator representing the next element. 
    Trying to remove from an empty list may cause undefined behaviour
RETURN
    Iterator representing the next element. 
INPUT
    iterator: iterator representing the element to remove
*/
dlist_iterator_t DListRemove(dlist_iterator_t iterator); /* O(1) */

/*
DESCRIPTION
    Receives a pointer to a doubly linked list and returns an iterator 
    that represents the beginning of the list.                                                  
RETURN
    Iterator that points to the beginning of
    the list.
INPUT
    dlist: a pointer to a doubly linked list.
*/
dlist_iterator_t DListBegin(const dlist_t *dlist); /* O(1) */

/*
DESCRIPTION
    Recieves a pointer to a doubly linked list and returns an iterator
    representing the ending element of that list. The end is 
    theoretical element which follows the last element of the list. 
    It neither contain actual data nor point to any other element.
RETURN
    Returns the iterator representing the ending element of the list.
INPUT
    dlist: a pointer to a doubly linked list.
*/
dlist_iterator_t DListEnd(const dlist_t *dlist); /* O(1) */

/*
DESCRIPTION
    Returns an address of the iterator next to the one passed by the user.
    Passing an iterator received from the function DListEnd may cause
    undefined behavior.
RETURN
    Function returns a pointer to an iterator
INPUT
    iterator: a pointer to an iterator.
*/
dlist_iterator_t DListNext(dlist_iterator_t iterator); /* O(1) */

/*
DESCRIPTION
    Returns an address of the iterator previous to the one passed by the user.
    Passing an iterator received from the function DListBegin may cause
    undefined behavior.
RETURN
    Function returns a pointer to an iterator
INPUT
    iterator: a pointer to an iterator.
*/
dlist_iterator_t DListPrev(dlist_iterator_t iterator); /* O(1) */

/*
DESCRIPTION
    Compares two iterators to check if they are the same.
RETURN
    1 if the iterators are the same;
    0 if they are not.
INPUT
    iterator1: an iterator representing an element in the list.
    iterator2: an iterator representing another element in the list.
*/
int DListIsSameIterator(dlist_iterator_t iterator1, dlist_iterator_t iterator2); /* 0(1) */

/*
DESCRIPTION
    Get data from an element represented by the given iterator.
RETURN
    Return a generic pointer to data.
INPUT
    iterator: a pointer to an iterator.
*/
void *DListGetData(dlist_iterator_t iterator); /* O(1) */

/*
DESCRIPTION
    Set data in an element represented by the given iterator.
    Trying to put data in the element representing the end or beginning
    of the list may result in undefined behavior.
RETURN
    Function returns a pointer to an iterator
INPUT
    iterator: a pointer to an iterator.
*/
void DListSetData(dlist_iterator_t iterator, void *data); /* O(1) */

/*
DESCRIPTION
    Inserts a new element into beginning of a doubly linked list.
RETURN
    The address of the new iterator if success.
    The iterator corresponding to the last element if insertion failed.
INPUT
    dlist: doubly linked list to push to.
    data: pointer to the user's data.
*/
dlist_iterator_t DListPushFront(dlist_t *dlist, void *data);

/*
DESCRIPTION
    Removes the element from the beginning of a doubly linked
    list and returns the iterator representing the next element. 
    Trying to pop from an empty list may cause undefined behaviour.
RETURN
    Iterator representing the next element. 
INPUT
    dlist: doubly linked list to pop from.
*/
void *DListPopFront(dlist_t *dlist);

/*
DESCRIPTION
    Inserts a new element to the end of a doubly linked list.
RETURN
    The address of the new iterator if success.
    The iterator corresponding to the last element if insertion failed.
INPUT
    dlist: doubly linked list to push to.
    data: pointer to the user's data.
*/
dlist_iterator_t DListPushBack(dlist_t * dlist, void *data);

/*
DESCRIPTION
    Removes the element from the end of a doubly linked
    list and returns the iterator representing the next element. 
    Trying to pop from an empty list may cause undefined behaviour.
RETURN
    Iterator representing the next element. 
INPUT
    dlist: doubly linked list to pop from.
*/
void *DListPopBack(dlist_t *dlist);

/*
DESCRIPTION
    Splice two doubly linked lists and returns iterator
    that represents position where were pasting.
    Elements from where-iterator to previous element
    of the end-iterator will be passed before where-iterator.
    Pasted elements will be removed from list they were passed.
RETURN
    Iterator that represents position where splice occurred.
INPUT
    where: iterator represents position where past to.
    begin: iterator represents element to past from.
    end: iterator represents element to past to.
*/
dlist_iterator_t DListSplice(dlist_iterator_t where,
                                                    dlist_iterator_t begin,
                                                    dlist_iterator_t end);
                         
                         

#endif  /* __NSRD_DLIST_H__ */