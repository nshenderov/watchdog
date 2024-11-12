/*******************************************************************************
*
* FILENAME : uid.h
*
* DESCRIPTION : UID stands for "Unique Identifier". It is a unique code assigned
* to an entity or object, such as a user, process, or file, to distinguish it
* from other similar entities or objects.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 4.05.2023
* 
*******************************************************************************/

#ifndef __NSRD_UID_H__
#define __NSRD_UID_H__

#include <sys/types.h> /* pid_t */ 
#include <time.h> /* time_t */

#define IP_SA_DATA_LENGTH (14)

typedef struct UID nsrd_uid_t;

/*
DESCRIPTION
	Constant for an element representing non-existent UID. Use it to check
	for functions failures.
*/
extern const nsrd_uid_t BadUID;

/*
DESCRIPTION
	The declaration of struct UID, only for definition of UID vars.
    User should never access the fields of the UID directly and should
    use only the provided functions.
*/
struct UID
{
    size_t counter;
    time_t timestamp;
    pid_t pid;
    char ip[IP_SA_DATA_LENGTH];
};

/*
DESCRIPTION
	Creates unique ID of a process. 
	Creation may fail.
RETURN
	UID on successful creation;
	BadUID on failure;
INPUT
	There is no input for this function.  
TIME COMPLEXITY:
    O(1)
*/
nsrd_uid_t UIDCreate(void);

/*
DESCRIPTION
	Checks if the UIDs are the same. 
RETURN
	1: UIDs are the same;
	0: UIDs are not the same.
INPUT
	uid1: UID passed by value;
	uid1: UID passed by value;
TIME COMPLEXITY:
    O(1)
*/
int UIDIsSame(nsrd_uid_t uid1, nsrd_uid_t uid2);


#endif  /* __NSRD_UID_H__ */ 