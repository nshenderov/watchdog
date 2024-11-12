/*******************************************************************************
*
* FILENAME : uid.c
*
* DESCRIPTION : UID implementation.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 4.05.2023
* 
*******************************************************************************/

#include <assert.h> /* assert */
#include <unistd.h> /* ifaddr */
#include <ifaddrs.h> /* getifaddrs */
#include <string.h> /* memcpy, memcmp */
#include <pthread.h> /* mutexes */

#include "uid.h"

enum {SUCCESS, FAILURE};
enum {FALSE, TRUE};

const nsrd_uid_t BadUID = {0};
static size_t g_counter = 0;

static void SetCounter(size_t *counter);
static void SetPID(pid_t *pid);
static void SetTime(time_t *timestamp);
static int SetIP(char *ip);
static int IsInterfaceLookingFor(struct ifaddrs *ifa);

nsrd_uid_t UIDCreate(void)
{
	nsrd_uid_t new_uid = {0};

	SetCounter(&new_uid.counter);
	SetPID(&new_uid.pid);
	SetTime(&new_uid.timestamp);
	SetIP(new_uid.ip);

	if (FAILURE == SetIP(new_uid.ip))
	{
		return (BadUID);
	}

	return (new_uid);
}

int UIDIsSame(nsrd_uid_t uid1, nsrd_uid_t uid2)
{
	if (uid1.counter != uid2.counter
	|| uid1.timestamp != uid2.timestamp
	|| uid1.pid != uid2.pid
	|| SUCCESS != memcmp(uid1.ip, uid2.ip, IP_SA_DATA_LENGTH))
	{
		return (FALSE);
	}

	return (TRUE);
}

static void SetCounter(size_t *counter)
{
	assert(NULL != counter);

	*counter = __sync_fetch_and_add(&g_counter, 1);
}

static void SetPID(pid_t *pid)
{
	assert(NULL != pid);

	*pid = getpid();
}

static void SetTime(time_t *timestamp)
{
	assert(NULL != timestamp);

	*timestamp = time(NULL);
}

static int SetIP(char *ip)
{
	struct ifaddrs *ifap = NULL;
	struct ifaddrs *ifa = NULL;

	assert(NULL != ip);

	if (SUCCESS != getifaddrs(&ifap))
	{
		return (FAILURE);
	}

	for (ifa = ifap; NULL != ifa; ifa = ifa -> ifa_next)
	{
		if (IsInterfaceLookingFor(ifa))
		{
			memcpy(ip, ifa -> ifa_addr -> sa_data, IP_SA_DATA_LENGTH);
			break;
		}
	}

	freeifaddrs(ifap);

	return (SUCCESS);
}

static int IsInterfaceLookingFor(struct ifaddrs *ifa)
{
	unsigned short family_looking_for = AF_INET;
	char *exception = "lo";

	if (NULL == ifa -> ifa_addr 
	|| family_looking_for != ifa -> ifa_addr -> sa_family 
	|| SUCCESS == strcmp(ifa -> ifa_name, exception))
	{
		return (FALSE);
	}

	return (TRUE);
}