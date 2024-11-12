/*******************************************************************************
*
* FILENAME : testing.h
*
* DESCRIPTION : Provides useful macros for C programs testing.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 7.05.2023
* 
*******************************************************************************/

#ifndef __NSRD_TESTING_H__
#define __NSRD_TESTING_H__

#include <stdio.h> /* printf*/

int TH_G_ASSERTS_FAILED = 0;
int TH_G_ASSERTS_PASSED = 0;
int TH_G_IS_CURR_TEST_PASSED = 0;

enum {TH_FAILED, TH_SUCCESS};

#define TH_YELLOW "\033[0;33m"
#define TH_GREEN "\033[0;32m"
#define TH_RED "\033[0;31m"
#define TH_NC "\033[0m"

typedef struct TH_TEST TH_TEST_T;

typedef void (*TH_TEST_FUNC_T)(void);

struct TH_TEST
{
	char *TH_INFO_MESSAGE;
	TH_TEST_FUNC_T TH_TEST_FUNC;
};

#define TH_TESTS_ARRAY_END {NULL, NULL}

#define TH_ASSERT(BOOL) 														\
do 																				\
{ 																				\
    if (BOOL)																	\
    { 																			\
    	++TH_G_ASSERTS_PASSED;													\
    }																			\
    else																		\
    {																			\
    	printf(TH_RED"ASSERTION ON THE LINE %d\n"TH_NC, __LINE__);				\
		++TH_G_ASSERTS_FAILED;													\
		TH_G_IS_CURR_TEST_PASSED = 0;											\
    }																			\
    																			\
}																				\
while (0)

#define TH_RUN_TESTS(TESTS) 													\
do 																				\
{																				\
	int FAILED_TEST_COUNTER = 0;												\
	int PASSED_TEST_COUNTER = 0;												\
	TH_TEST_T *RUNNER = TESTS;													\
																				\
	while(NULL != RUNNER->TH_TEST_FUNC && NULL != RUNNER->TH_INFO_MESSAGE)		\
	{																			\
		TH_G_IS_CURR_TEST_PASSED = 1;											\
		printf("\nTEST: "TH_YELLOW"%s\n"TH_NC, RUNNER->TH_INFO_MESSAGE);			\
		RUNNER->TH_TEST_FUNC();													\
		++RUNNER;																\
		if (TH_G_IS_CURR_TEST_PASSED)											\
		{																		\
			printf("%s\n", TH_GREEN"SUCCESS"TH_NC);								\
			++PASSED_TEST_COUNTER;												\
		}																		\
		else																	\
		{																		\
			printf("%s\n", TH_RED"FAILURE"TH_NC);								\
			++FAILED_TEST_COUNTER;												\
		}																		\
	}																			\
																				\
	printf("\n### SUMMARY: ###\n");												\
	printf("TESTS PASSED: "TH_GREEN"%d\n"TH_NC, PASSED_TEST_COUNTER);			\
	printf("TESTS FAILED: "TH_RED"%d\n"TH_NC, FAILED_TEST_COUNTER);				\
	printf("ASSERTS PASSED: "TH_GREEN"%d\n"TH_NC, TH_G_ASSERTS_PASSED);			\
	printf("ASSERTS FAILED: "TH_RED"%d\n"TH_NC, TH_G_ASSERTS_FAILED);			\
}																				\
while (0)

#endif /* __NSRD_TESTING_H__ */