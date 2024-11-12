/*******************************************************************************
*
* FILENAME : dlist.h
*
* DESCRIPTION : Doubly linked list is an abstract data type that holds a
* collection of nodes, the nodes can be accessed in a sequential way in both
* directions.
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
    Pointer to the function that executes the action on data using the param. 
    The actual action and types of the input are defined by the user.
RETURN
    0: success
    non-zero value: failure
INPUT
    data: pointer to the user's data.
    param: pointer to the parameter.
*/
typedef int (*dlist_action_func_t)(void *data, void *param);

/*
DESCRIPTION
    Pointer to the function that validates if the data matches a certain 
    criteria using the param.
    The actual matching and types of the input is defined by the user.
RETURN
    1: matches.
    0: not matches.
INPUT
    data: pointer to the user's data.
    param: pointer to the parameter.    
*/
typedef int (*dlist_is_match_func_t)(const void *data, void *param);

/*
DESCRIPTION
    Creates a doubly  linked list.
    Creation may fail, due to memory allocation fail. 
    User is responsible for memory deallocation.
RETURN
    Returns pointer to the created linked list on success.
    Returns NULL on failure.
INPUT
    Doesn't accept any input from the user.
TIME_COMPLEXITY
    O(1)
*/
dlist_t *DListCreate(void);

/*
DESCRIPTION
    Frees the memory allocated for each element of a doubly linked list.
RETURN
    There is no return for this function.
INPUT
    dlist: pointer to ther doubly linked list.
TIME_COMPLEXITY
    O(n)
*/
void DListDestroy(dlist_t *dlist);

/*
DESCRIPTION
    Traverses the doubly linked list and returns the amount of elements.
RETURN
    The amount of elements currently in the list. 
INPUT
    dlist: pointer to the doubly linked list.
TIME_COMPLEXITY
    O(n)
*/
size_t DListSize(const dlist_t *dlist);

/*
DESCRIPTION
    Checks if doubly linked list is empty.
RETURN
    1: is empty.
    0: is not empty.
INPUT
    dlist: pointer to the doubly linked list.
TIME_COMPLEXITY
    O(1)
*/
int DListIsEmpty(const dlist_t *dlist);

/*
DESCRIPTION
    Searches in the doubly linked list in the range for the element that
    satisfies is_match function and returns the first occurance or the iterator 
    representing the "to" element of the list if the elemnt isn't found. 
    The "to" element is marks the end of the searched range 
    but is not included in it.
RETURN
    If found - the iterator representing the first element in the range that 
    contains the value when it finds the value.
    If not - the iterator representing the ending element.
INPUT
    from: iterator representing the element that starts the range;
    to: iterator marking the end of the range (isn't a part of the range);
    is_match: function that checks values.
    param: parameter for the is_match function.
TIME_COMPLEXITY
    O(n)
*/
dlist_iterator_t DListFind(dlist_iterator_t from, dlist_iterator_t to,
                                   dlist_is_match_func_t is_match, void *param);

/*
DESCRIPTION
    Searches in the doubly linked list in the range for the elements that
    satisfy is_match function and fills up passed list with those elements from
    first to last occuerence.
RETURN
    The number of elements founded.
INPUT
    from: iterator representing the element that starts the range;
    to: iterator marking the end of the range (isn't a part of the range);
    output_list: doubly linked list to fill up with founded elements;
    is_match: function that checks values.
    param: parameter for the is_match function.
TIME_COMPLEXITY
    O(n)
*/
size_t DListMultiFind(dlist_iterator_t from, dlist_iterator_t to,
            dlist_t *output_list, dlist_is_match_func_t is_match, void *param);

/*
DESCRIPTION
    Traverses the linked list from one point to another and performs 
    the action specified by the user. The "to" element is marks  the end of the
    searched range but is not included in it. Performed actions may fail. 
RETURN
    0: no actions fail; 
    non-zero value: an action failed. 
INPUT
    from: iterator representing the element that starts the range;
    to: iterator marking the end of the range (isn't a part of the range);
    action: pointer to an action function;
    param: parameter for the action function.
TIME_COMPLEXITY
    O(n)
*/
int DListForEach(dlist_iterator_t from, dlist_iterator_t to, 
                                dlist_action_func_t action, void *param);

/*
DESCRIPTION
    Inserts a new element to the doubly linked list in the position.
RETURN
    The address of the new iterator if success.
    The iterator corresponding to the last element if insertion failed.
INPUT
    iterator: iterator to the newly inserted data
    data: pointer to the user's data.
TIME_COMPLEXITY
    O(n)
*/
dlist_iterator_t DListInsert(dlist_iterator_t iterator, void *data);

/*
DESCRIPTION
    Removes the element represented by iterator from a doubly linked
    list and returns the iterator representing the next element. 
    Trying to remove from an empty list may cause undefined behavior.
RETURN
    Iterator representing the next element. 
INPUT
    iterator: iterator representing the element to remove
TIME_COMPLEXITY
    O(1)
*/
dlist_iterator_t DListRemove(dlist_iterator_t iterator);

/*
DESCRIPTION
    Receives pointer to the doubly linked list and returns iterator 
    that represents the beginning of the list.
RETURN
    Iterator that points to the beginning of the list.
INPUT
    dlist: pointer to the doubly linked list.
TIME_COMPLEXITY
    O(1)
*/
dlist_iterator_t DListBegin(const dlist_t *dlist);

/*
DESCRIPTION
    Recieves pointer to a doubly linked list and returns an iterator
    representing the ending element of that list. The end is 
    theoretical element which follows the last element of the list. 
    It neither contain actual data nor point to any other element.
RETURN
    Returns the iterator representing the ending element of the list.
INPUT
    dlist: pointer to a doubly linked list.
TIME_COMPLEXITY
    O(1)
*/
dlist_iterator_t DListEnd(const dlist_t *dlist);

/*
DESCRIPTION
    Returns an address of the iterator next to the one passed by the user.
    Passing an iterator received from the function DListEnd may cause
    undefined behavior.
RETURN
    Function returns pointer to an iterator
INPUT
    iterator: pointer to an iterator.
TIME_COMPLEXITY
    O(1)
*/
dlist_iterator_t DListNext(dlist_iterator_t iterator);

/*
DESCRIPTION
    Returns an address of the iterator previous to the one passed by the user.
    Passing an iterator received from the function DListBegin may cause
    undefined behavior.
RETURN
    Function returns pointer to an iterator
INPUT
    iterator: pointer to an iterator.
TIME_COMPLEXITY
    O(1)
*/
dlist_iterator_t DListPrev(dlist_iterator_t iterator);

/*
DESCRIPTION
    Compares two iterators to check if they are the same.
RETURN
    1 if the iterators are the same;
    0 if they are not.
INPUT
    iterator1: an iterator representing an element in the list.
    iterator2: an iterator representing another element in the list.
TIME_COMPLEXITY
    O(1)
*/
int DListIsSameIterator(dlist_iterator_t iterator1, dlist_iterator_t iterator2);

/*
DESCRIPTION
    Get data from an element represented by the given iterator.
RETURN
    Return a generic pointer to data.
INPUT
    iterator: pointer to an iterator.
TIME_COMPLEXITY
    O(1)
*/
void *DListGetData(dlist_iterator_t iterator);

/*
DESCRIPTION
    Set data in an element represented by the given iterator.
    Trying to put data in the element representing the end or beginning
    of the list may result in undefined behavior.
RETURN
    Function returns pointer to an iterator
INPUT
    iterator: pointer to an iterator.
TIME_COMPLEXITY
    O(1)
*/
void DListSetData(dlist_iterator_t iterator, void *data);

/*
DESCRIPTION
    Inserts a new element into beginning of a doubly linked list.
RETURN
    The address of the new iterator if success.
    The iterator corresponding to the last element if insertion failed.
INPUT
    dlist: doubly linked list to push to.
    data: pointer to the user's data.
TIME_COMPLEXITY
    O(n)
*/
dlist_iterator_t DListPushFront(dlist_t *dlist, void *data);

/*
DESCRIPTION
    Removes the element from the beginning of a doubly linked
    list and returns the iterator representing the next element. 
    Trying to pop from an empty list may cause undefined behavior.
RETURN
    Iterator representing the next element. 
INPUT
    dlist: doubly linked list to pop from.
TIME_COMPLEXITY
    O(1)
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
TIME_COMPLEXITY
    O(n)
*/
dlist_iterator_t DListPushBack(dlist_t * dlist, void *data);

/*
DESCRIPTION
    Removes the element from the end of a doubly linked
    list and returns the iterator representing the next element. 
    Trying to pop from an empty list may cause undefined behavior.
RETURN
    Iterator representing the next element. 
INPUT
    dlist: doubly linked list to pop from.
TIME_COMPLEXITY
    O(1)
*/
void *DListPopBack(dlist_t *dlist);

/*
DESCRIPTION
    Splice two doubly linked lists and returns iterator that represents position
    where were pasting. Elements from where-iterator to previous element of the
    end-iterator will be passed before where-iterator. Pasted elements will be
    removed from list they were passed.
RETURN
    Iterator that represents position where splice occurred.
INPUT
    where: iterator represents position where past to.
    begin: iterator represents element to past from.
    end: iterator represents element to past to.
TIME_COMPLEXITY
    O(1)
*/
dlist_iterator_t DListSplice(dlist_iterator_t where, dlist_iterator_t begin,
                                                          dlist_iterator_t end);

#endif  /* __NSRD_DLIST_H__ */