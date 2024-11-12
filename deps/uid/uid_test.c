/*******************************************************************************
*
* FILENAME : uid_test.c
*
* DESCRIPTION : UID unit tests.
*
* AUTHOR : Nick Shenderov
*
* DATE : 4.05.2023
* 
*******************************************************************************/

#include "uid.h"
#include "testing.h"


static void TestUID(void);

int main()
{
	TH_TEST_T TESTS[] = {
   	{"UID", TestUID},
		TH_TESTS_ARRAY_END
	};

	TH_RUN_TESTS(TESTS);

	return (0);
}

static void TestUID(void)
{
	nsrd_uid_t uid = UIDCreate();
	nsrd_uid_t uid2 = UIDCreate();

	TH_ASSERT(1 == UIDIsSame(uid, uid));
	TH_ASSERT(0 == UIDIsSame(uid, uid2));
	TH_ASSERT(0 == UIDIsSame(uid, BadUID));
	TH_ASSERT(0 == UIDIsSame(uid2, BadUID));
	TH_ASSERT(1 == UIDIsSame(BadUID, BadUID));
}