/*******************************************************************************
*
* FILENAME : sorted_list.h
*
* DESCRIPTION : Sorted list abstract data type (ADT) is a collection of
* elements that are arranged in a specific order such that the elements can be
* easily accessed and manipulated according to their position in the list.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 24.04.2023
* 
*******************************************************************************/

#ifndef __NSRD_SORTEDLIST_H__
#define __NSRD_SORTEDLIST_H__

#include <stddef.h> /* size_t, NULL */

#include "dlist.h" /* doubly linked list */

/*
DESCRIPTION
    Pointer to the user's function that compares data1 and data2.
    The actual matching and types of the input are defined by the user.
    Elements will be in sorted list in acsending order based on this function.
RETURN
    Positive: data1 > data2.
    Negative: data1 < data2.
    0: data1 == data2.
INPUT
    data1: pointer to the user's data.
    data2: pointer to the user's data.
*/
typedef int (*sorted_list_compare_func_t) (const void *data1, const void *data2); 

/*
DESCRIPTION
    Pointer to the user's function that executes an action on data using the
    param. The actual action and types of the input are defined by the user.
RETURN
    0: success.
    non-zero: fail.
INPUT
    data: pointer to the user's data.
    param: pointer to the parameter.
*/
typedef int (*sorted_list_action_func_t)(void *data, void *param);

/*
DESCRIPTION
    Pointer to the user's function that compares if the data matches a certain
    criteria, using the param. The actual matching and types of the input are
    defined by the user.
RETURN
    1: matches.
    0: not mathes.
INPUT
    data: pointer to the user's data.
    param: pointer to the parameter.
*/
typedef int (*sorted_list_is_match_func_t)(const void *data1, void *param);

typedef struct sorted_list sorted_list_t;
typedef struct sorted_list_iter sorted_list_iterator_t;

struct sorted_list_iter
{
    dlist_iterator_t internal_iter;
    
    #ifndef NDEBUG
    sorted_list_t *parent_list;
    #endif
};

/*
DESCRIPTION
    Creates new sorted linked list. List will use pattern of sorting, based
    on the user's compare function - "comp".
    Creation may fail, due to memory allocation fail.
    User is responsible for memory deallocation.
RETURN
    Pointer to the created sorted linked list on success.
    NULL if allocation failed.
INPUT
    comp: pointer to the compare function.
TIME_COMPLEXITY:
    O(1)
*/
sorted_list_t *SortedListCreate(sorted_list_compare_func_t comp);

/*
DESCRIPTION
    Frees the memory allocated for each element of the sorted linked list
    and the list itself.
    All data inside the list will be lost.
RETURN
    Doesn't return anything.
INPUT
    sorted_list: pointer to the sorted linked list.
TIME_COMPLEXITY:
    O(n)
*/
void SortedListDestroy(sorted_list_t *sorted_list);

/*
DESCRIPTION
    Traverses the sorted linked list and returns the amount of elements.
RETURN
    Number of elements in the sorted linked list.
INPUT
    sorted_list: pointer to the sorted linked list.
TIME_COMPLEXITY:
    O(n)
*/
size_t SortedListSize(const sorted_list_t *sorted_list);

/*
DESCRIPTION
    Checks if sorted linked list is empty.
RETURN
    1: is empty.
    0: is not empty.
INPUT
    sorted_list: pointer to the sorted linked list.
TIME_COMPLEXITY:
    O(1)
*/
int SortedListIsEmpty(const sorted_list_t *sorted_list);

/*
DESCRIPTION
    Inserts a new element to the sorted linked list on the position, according
    to sorted_list pointer and compare function. If some elements are equal
    according to the sorted_list compare function, new element will go first.
    Insertion may fail, due to memory allocation fail.
RETURN
    Iterator representing new element if success.
    Iterator representing the end of a sorted linked list (theoretical element)
    if fail.
INPUT
    sorted_list: pointer to the sorted linked list.
    data: pointer to the user's data.
TIME_COMPLEXITY:
    O(n)
*/
sorted_list_iterator_t SortedListInsert(sorted_list_t *sorted_list, void *data);

/*
DESCRIPTION
    Removes the element represented by the iterator from the sorted linked list.
    Passing an iterator received from the SortedListEnd() function is
    undefined behavior.
RETURN
    Iterator representing the next element.
INPUT
    iterator: iterator representing the element to remove.
TIME_COMPLEXITY:
    O(1)
*/
sorted_list_iterator_t SortedListRemove(sorted_list_iterator_t iterator);

/*
DESCRIPTION
    Returns the iterator representing the first element in passed sorted linked
    list.
RETURN
    Iterator representing the first element in sorted linked list.
INPUT
    sorted_list: pointer to the sorted linked list.
TIME_COMPLEXITY:
    O(1)
*/
sorted_list_iterator_t SortedListBegin(const sorted_list_t *sorted_list);

/*
DESCRIPTION
    Returns the iterator representing the end of passed sorted linked list.
    The end is theoretical element which follows the last element of the list.
    It doesn't contain actual data.
RETURN
    Iterator representing the the end of a sorted linked list
    (theoretical element).
INPUT
    sorted_list: pointer to the sorted linked list.
TIME_COMPLEXITY:
    O(1)
*/
sorted_list_iterator_t SortedListEnd(const sorted_list_t *sorted_list);

/*
DESCRIPTION
    Returns the iterator representing the next element in sorted linked list.
    Passing an iterator received from the SortedListEnd() function may cause
    undefined behavior.
RETURN
    Iterator representing the next element.
INPUT
    iterator: iterator representing the element.
TIME_COMPLEXITY:
    O(1)
*/
sorted_list_iterator_t SortedListNext(sorted_list_iterator_t iterator);

/*
DESCRIPTION
    Returns the iterator representing the previous element in sorted 
    linked list.
    Passing an iterator received from the SortedListBegin() function may cause
    undefined behavior.
RETURN
    Iterator representing the previous element.
INPUT
    iterator: iterator representing the element.
TIME_COMPLEXITY:
    O(1)
*/
sorted_list_iterator_t SortedListPrev(sorted_list_iterator_t iterator);

/*
DESCRIPTION
    Compares two iterators to check if they are the same.
RETURN
    1: iterators are the same.
    0: iterators are not the same.
INPUT
    iterator1: iterator representing an element in the sorted linked list.
    iterator2: iterator representing an element in the sorted linked list.
TIME_COMPLEXITY:
    O(1)
*/
int SortedListIsSameIterator(sorted_list_iterator_t iterator1,
                                sorted_list_iterator_t iterator2);

/*
DESCRIPTION
    Gets data from the element represented by the given iterator.
    Data is stored in sorted linked list by reference.
    Passing an iterator received from the SortedListEnd() function may cause
    undefined behavior. 
RETURN
    Pointer to the data inside the element.
INPUT
    iterator: iterator representing element in the sorted linked list.
TIME_COMPLEXITY:
    O(1)
*/
void *SortedListGetData(sorted_list_iterator_t iterator);

/*
DESCRIPTION
    Removes the last element of passed sorted linked list.
    Trying to pop empty sorted linked list may cause undefined behavior.
RETURN
    Pointer to the data of the removed element.
INPUT
    sorted_list: pointer to the sorted linked list.
TIME_COMPLEXITY:
    O(1)
*/
void *SortedListPopBack(sorted_list_t *sorted_list);                 

/*
DESCRIPTION
    Removes the first element of passed sorted linked list.
    Trying to pop empty sorted linked list may cause undefined behavior.
RETURN
    Pointer to the data of the removed element.
INPUT
    sorted_list: pointer to the sorted linked list.
TIME_COMPLEXITY:
    O(1)
*/
void *SortedListPopFront(sorted_list_t *sorted_list);

/*
DESCRIPTION
    Traverses the sorted linked list in the certain range for all elements and
    call user's action function passing element's data and param as input.
    The "to" element is marks the end of the range, but is not included in it.
    Passing "from" and "to" from different lists may cause undefined behavior.
    Actions perfomed might fail.
RETURN
    0: no actions failed.
    non-zero: at least one action failed.
INPUT
    from: iterator representing the element that starts the range.
    to: iterator marking the end of the range (isn't a part of the range).
    action: user's function that executes an action.
    param: parameter for the is_match function.
TIME_COMPLEXITY:
    O(n)
*/
int SortedListForEach(sorted_list_iterator_t from, sorted_list_iterator_t to, 
                               sorted_list_action_func_t action, void *param);

/*
DESCRIPTION
    Merges two sorted linked lists. The merged elements move to the 
    sorted_list1, and get removed from the sorted_list2. If some elements are
    equal according to the sorted_list compare function, the element from 
    sorted_list2 will go last in the merged list.
    Passing lists sorted by different compare functions may cause undefined
    behavior.
    Passing the same list as sorted_list1 and sorted_list2 may cause 
    undefined behavior.
RETURN
    Pointer to the merged list.
INPUT
    sorted_list1: pointer to the sorted list. The list will contain all the
    elements from both lists after the operation.
    sorted_list2: pointer to the sorted linked list. The list will be empty
    after the operation.
TIME_COMPLEXITY:
    O(n + m)
*/
sorted_list_t *SortedListMerge(sorted_list_t *sorted_list1,
                                sorted_list_t *sorted_list2);

/*
DESCRIPTION
    Traverses the sorted linked list in the certain range for element that
    corresponds to the passed data according to the sorted_list compare 
    function. Returns the first occurance.
    The "to" element marks the end of the range, but is not included in it.
    Passing "from" and "to" from different lists may cause undefined behavior.
RETURN
    The iterator representing the first found element.
    If nothing found - the "to" iterator.
INPUT
    sorted_list: pointer to the sorted linked list.
    from: iterator representing the element that starts the range.
    to: iterator marking the end of the range (isn't a part of the range).
    param: a parameter for the is_match function.
TIME_COMPLEXITY:
    O(n)
*/
sorted_list_iterator_t SortedListFind(sorted_list_t *sorted_list,
                                        sorted_list_iterator_t from,
                                        sorted_list_iterator_t to, void *data);

/*
DESCRIPTION
    Traverses the sorted linked list in the certain range for element that
    satisfies is_match function's criteria. Returns the first occurance.
    The "to" element marks the end of the range, but is not included in it.
    Passing "from" and "to" from different lists may cause undefined behavior.
RETURN
    The iterator representing the first element in the range that matches the
    is_match function's criteria.
    If no matches found - the "to" iterator.
INPUT
    from: iterator representing the element that starts the range.
    to: iterator marking the end of the range (isn't a part of the range).
    is_match: user's function that checks if the criteria is matched.
    param: parameter for the is_match function.
TIME_COMPLEXITY:
    O(n)
*/
sorted_list_iterator_t SortedListFindIf(sorted_list_iterator_t from,
                            sorted_list_iterator_t to,
                            sorted_list_is_match_func_t is_match, void *param);

#endif  /* __NSRD_SORTEDLIST_H__ */