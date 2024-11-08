/*******************************************************************************
*
*  FILENAME : uid.h
*
* DESCRIPTION : Interface of a UID data structure.
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

/*******************************************************************************
*   Specifies the size of ip field in UID structure.
*   Don't change it directly.
*******************************************************************************/
#define IP_SA_DATA_LENGTH (14)

typedef struct UID nsrd_uid_t;

/*******************************************************************************
*   Structure returned by create function in case of failure.
*   Use UIDIsSame() and this value to check for failures.
*******************************************************************************/
extern const nsrd_uid_t BadUID;

/*******************************************************************************
*   DESCRIPTION:
*       The UID structure.
*   FIELDS:
*       counter: UID's number.
*       timestamp: timestamp at the moment of creation of UID.
*       pid: callers PID.
*       ip: callers ip address.
*   NOTE:
*       Never access the fields of the uid directly. 
*       The names, types, and number of fields can change in future versions.
*       Only use the provided functions.
*******************************************************************************/
struct UID
{
    size_t counter;
    time_t timestamp;
    pid_t pid;
    char ip[IP_SA_DATA_LENGTH];
};

/*******************************************************************************
*   DESCRIPTION:
*       Creates a UID structure.
*       Creation may fail, in this case function returns BadUID. 
*       Use UIDIsSame() and BadUID value to check for failures.
*   RETURN:
*       Returns new UID on success.
*       Returns BadUID on failure.
*   INPUT:
*       Nothing
*   TIME COMPLEXITY:
*       O(1)
*******************************************************************************/
nsrd_uid_t UIDCreate(void);

/*******************************************************************************
*   DESCRIPTION:
*       Compares two UIDs.
*   RETURN:
*       1 - Equal.
*       0 - Not equal.
*   INPUT:
*       uid1: UID structure to compare.
*       uid2: UID structure to compare.
*   TIME COMPLEXITY:
*       O(1)
*******************************************************************************/
int UIDIsSame(nsrd_uid_t uid1, nsrd_uid_t uid2);


#endif  /* __NSRD_UID_H__ */ 