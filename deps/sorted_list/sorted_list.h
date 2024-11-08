/*******************************************************************************
*
* FILENAME : sorted_list.h
*
* DESCRIPTION : Interface of a sorted linked list data structure.
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

/*******************************************************************************
*   DESCRIPTION:
*       A pointer to a function that compares two elements. This function
*       is used to sort elements in sorted list.
*       The actual matching and types of the input is defined by the user.
*   RETURN:
*       1 - data1 larger than data2.
*       0 - they are equal.
*       -1 - data1 smaller than data2.
*   INPUT:
*       data: a pointer to some value;
*       param: a pointer to a parameter.    
*******************************************************************************/
typedef int (*sorted_list_compare_func_t) (const void *data1, const void *data2); 

/*******************************************************************************
*   DESCRIPTION:
*       A pointer to a function that executes an action on data using the param. 
*       The actual action and types of the input are defined by the user.
*   RETURN:
*       0 - if success
*       non-zero value - if failure
*   INPUT:
*       data: a pointer to some value;
*       param: a pointer to a parameter.
*******************************************************************************/
typedef int (*sorted_list_action_func_t)(void *data, void *param);

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
typedef int (*sorted_list_is_match_func_t)(const void *data1, void *param);

/*******************************************************************************
*   DESCRIPTION:
*       Basic type definitions for management and iterator structures.
*******************************************************************************/
typedef struct sorted_list sorted_list_t;
typedef struct sorted_list_iter sorted_list_iterator_t;

/*******************************************************************************
*   DESCRIPTION:
*       The sorted listed iterator structure.
*   FIELDS:
*       internal_iter: pointer to a doubly linked list iterator.
*       parent_list: Pointer to a sorted linked list includes only in debug mode
*       for debugging purposes.
*******************************************************************************/
struct sorted_list_iter
{
    dlist_iterator_t internal_iter;
    
    #ifndef NDEBUG
    sorted_list_t *parent_list;
    #endif
};

/*******************************************************************************
*   DESCRIPTION:
*       Creates a sorted  linked list.
*       Creation may fail, due to memory allocation fail. 
*       User is responsible for destroying the linked list.
*   RETURN:
*       Returns pointer to the created sorted linked list on success.
*       Returns NULL on failure.
*   INPUT:
*       comp: a comparison function used to sort elements in sorted linked list.
*   TIME COMPLEXITY:
*       O(1)
*******************************************************************************/
sorted_list_t *SortedListCreate(sorted_list_compare_func_t comp);

/*******************************************************************************
*   DESCRIPTION:
*       Frees the memory allocated for each element of a sorted linked list.
*   RETURN:
*       There is no return for this function.
*   INPUT:
*       sorted_list: a pointer to a sorted linked list.
*   TIME COMPLEXITY:
*       O(n)
*******************************************************************************/
void SortedListDestroy(sorted_list_t *sorted_list);

/*******************************************************************************
*   DESCRIPTION:
*       Traverses the sorted linked list and returns the amount of elements.
*   RETURN:
*       The amount of elements currently in the sorted linked list. 
*   INPUT:
*       sorted_list: a pointer to a sorted linked list.
*   TIME COMPLEXITY:
*       O(n)
*******************************************************************************/
size_t SortedListSize(const sorted_list_t *sorted_list);

/*******************************************************************************
*   DESCRIPTION:
*       Checks if sorted linked list is empty.
*   RETURN:
*       1 true - if the sorted linked list is empty.
*       0 false - if it's not
*   INPUT:
*      sorted_list: a pointer to a sorted linked list.
*   TIME COMPLEXITY:
*       O(1)
*******************************************************************************/
int SortedListIsEmpty(const sorted_list_t *sorted_list);

/*******************************************************************************
*   DESCRIPTION:
*       Inserts a new element to a sorted linked list.
*       Passing not valid data may cause undefined behavior.
*   RETURN:
*       The address of the new iterator that represents inserted
*       element if success.
*       The iterator corresponding to the last element if insertion failed.
*   INPUT:
*       sorted_list: a pointer to a sorted linked list.
*       data: pointer to the user's data.
*   TIME COMPLEXITY:
*       O(n)
*******************************************************************************/
sorted_list_iterator_t SortedListInsert(sorted_list_t *sorted_list, void *data);

/*******************************************************************************
*   DESCRIPTION
*       Removes the element represented by iterator from a sorted linked
*       list and returns the iterator representing the next element. 
*       Trying to remove from an empty list may cause undefined behavior.
*   RETURN
*       Iterator representing the next element. 
*   INPUT
*       iterator: iterator representing the element to remove.
*   TIME COMPLEXITY:
*       O(1)
*******************************************************************************/
sorted_list_iterator_t SortedListRemove(sorted_list_iterator_t iterator);

/*******************************************************************************
*   DESCRIPTION
*       Receives a pointer to a sorted linked list and returns an iterator 
*       that represents the beginning of the list. (First valid element)
*   RETURN
*       Iterator that points to the beginning of the list.
*   INPUT
*       sorted_list: a pointer to a sorted linked list.
*   TIME COMPLEXITY:
*       O(1)
*******************************************************************************/
sorted_list_iterator_t SortedListBegin(const sorted_list_t *sorted_list);

/*******************************************************************************
*   DESCRIPTION
*       Recieves a pointer to a sorted linked list and returns an iterator
*       representing the ending element of that list. The end is 
*       theoretical element which follows the last element of the list. 
*       It neither contain actual data nor point to any other element.
*   RETURN
*       Returns the iterator representing the ending element of the list.
*   INPUT
*       sorted_list: a pointer to a sorted linked list.
*   TIME COMPLEXITY:
*       O(1)
*******************************************************************************/
sorted_list_iterator_t SortedListEnd(const sorted_list_t *sorted_list);

/*******************************************************************************
*   DESCRIPTION
*       Returns an address of the sorted list iterator next to the one passed
*       by the user.
*       Passing an iterator received from the function SortedListEnd may cause
*       undefined behavior.
*   RETURN
*       Function returns a pointer to an iterator.
*   INPUT
*       iterator: a pointer to an iterator.
*   TIME COMPLEXITY:
*       O(1)
*******************************************************************************/
sorted_list_iterator_t SortedListNext(sorted_list_iterator_t iterator);

/*******************************************************************************
*   DESCRIPTION
*       Returns an address of the sorted list iterator previous to the one
*       passed by the user.
*       Passing an iterator received from the function SortedListBegin may cause
*       undefined behavior.
*   RETURN
*       Function returns a pointer to an iterator.
*   INPUT
*       iterator: a pointer to an iterator.
*   TIME COMPLEXITY:
*       O(1)
*******************************************************************************/
sorted_list_iterator_t SortedListPrev(sorted_list_iterator_t iterator);

/*******************************************************************************
*   DESCRIPTION
*       Compares two sorted list iterators to check if they are the same.
*   RETURN
*       1 if the iterators are the same;
*       0 if they are not.
*   INPUT
*       iterator1: an iterator representing an element in the sorted linked list.
*       iterator2: an iterator representing an element in the sorted linked list.
*   TIME COMPLEXITY:
*       O(1)
*******************************************************************************/
int SortedListIsSameIterator(sorted_list_iterator_t iterator1,
                                sorted_list_iterator_t iterator2);

/*******************************************************************************
*   DESCRIPTION
*       Get data from an element represented by the given sorted list iterator.
*   RETURN
*       Return a generic pointer to data.
*   INPUT
*       iterator: a pointer to an iterator.
*   TIME COMPLEXITY:
*       O(1)
*******************************************************************************/
void *SortedListGetData(sorted_list_iterator_t iterator);

/*******************************************************************************
*   DESCRIPTION
*       Removes the element from the end of a sorted linked list and returns the
*       data of the removed element.
*       Trying to pop from an empty list may cause undefined behavior.
*   RETURN
*       Data of the removed element.
*   INPUT
*       sorted_list: a pointer to a sorted linked list.
*   TIME COMPLEXITY:
*       O(1)
*******************************************************************************/
void *SortedListPopBack(sorted_list_t *sorted_list);                 

/*******************************************************************************
*   DESCRIPTION
*       Removes the element from the beginning of a sorted linked list and
*       returns the data of the removed element.
*       Trying to pop from an empty list may cause undefined behavior.
*   RETURN
*       Data of the removed element.
*   INPUT
*       sorted_list: a pointer to a sorted linked list.
*   TIME COMPLEXITY:
*       O(1)
*******************************************************************************/
void *SortedListPopFront(sorted_list_t *sorted_list);

/*******************************************************************************
*   DESCRIPTION
*       Traverses the sorted linked list from one point to another and performs 
*       a specific action specified by the user. The "to" element is marks 
*       the end of the searched range but is not included in it.
*       Action function that breaks sorting in a list may cause undefined
*       behavior.
*       Actions performed might fail.
*   RETURN
*       0: if no actions fail; 
*       non-zero value: if an action failed. 
*   INPUT
*       from: iterator representing the element that starts the range;
*       to: iterator marking the end of the range (isn't a part of the range);
*       action: a pointer to an action function;
*       param: a parameter for the action function.
*   TIME COMPLEXITY:
*       O(n)
*******************************************************************************/
int SortedListForEach(sorted_list_iterator_t from, sorted_list_iterator_t to, 
                               sorted_list_action_func_t action, void *param);

/*******************************************************************************
*   DESCRIPTION
*       Splice two sorted linked lists together and returns the pointer that
*       points to the first one.
*       In the case of equal elements, the element from the second list will be
*       inserted after the element from the first one.
*       The sorting function in both lists has to be the same.
*   RETURN
*       Pointer to the destination list.
*   INPUT:
*       sorted_list1: a pointer to a sorted linked list.
*       sorted_list2: a pointer to a sorted linked list.
*   TIME COMPLEXITY:
*       O(n+m)
*******************************************************************************/
sorted_list_t *SortedListMerge(sorted_list_t *sorted_list1,
                                sorted_list_t *sorted_list2);

/*******************************************************************************
*   DESCRIPTION
*       Searches for a specific element in a sorted linked list that matches
*       provided data.
*       The function compares elements data of the list to passed data and
*       returns the first founded occurrence or "to" iterator in case of
*       failure.
*   RETURN
*       iterator: a pointer to an iterator that represents the founded element
*       or pointer to the "to" iterator in case of failure.
*   INPUT:
*       sorted_list: a pointer to a sorted linked list.
*       from: iterator representing the element that starts the range;
*       to: iterator marking the end of the range (isn't a part of the range);
*       data: pointer to the data to compare to.
*   TIME COMPLEXITY:
*       O(n)
*******************************************************************************/
sorted_list_iterator_t SortedListFind(sorted_list_t *sorted_list,
                                        sorted_list_iterator_t from,
                                        sorted_list_iterator_t to, void *data);

/*******************************************************************************
*   DESCRIPTION
*       Searches for a specific element in a sorted linked list that satisfies
*       an is_match function and returns the first occurence.
*       The function compares elements of the list using the is_match function
*       and the passed parameter.
*   RETURN
*       iterator: a pointer to an iterator that represents the founded element
*       or pointer to the "to" iterator in case of failure.
*   INPUT:
*       from: iterator representing the element that starts the range;
*       to: iterator marking the end of the range (isn't a part of the range);
*       data: pointer to the data to compare to.
*       data: pointer to the data to compare to.
*   TIME COMPLEXITY:
*       O(n)
*******************************************************************************/
sorted_list_iterator_t SortedListFindIf(sorted_list_iterator_t from,
                            sorted_list_iterator_t to,
                            sorted_list_is_match_func_t is_match, void *param);

#endif  /* __NSRD_SORTEDLIST_H__ */