/*
 * main.c
 *
 *  Created on: 19 Oct 2022
 *      Author: shriraam-sundaram
 */
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>
#include "count_vowels.h"


int main(int argc, char ** argv)
{
	printf("Hello World! \n");
	printf("%s \n", argv[1]);

	pthread_t thread1, thread2;

	uint64_t thread1VowelCount = 0u;
	uint64_t thread2VowelCount = 0u;
	int thread1Ret = 0;
	int thread2Ret = 0;

	/*Create worker threads to complete the job*/
	thread1Ret = pthread_create( &thread1, NULL, count_vowels, (void *)(&((COUNT_VOWEL_ThreadArgs_t){argv[1], 1, 5,  &thread1VowelCount})));
	thread2Ret = pthread_create( &thread2, NULL, count_vowels, (void *)(&((COUNT_VOWEL_ThreadArgs_t){argv[1], 6, 11, &thread2VowelCount})));


	/*Wait until the worker thread completes its job*/
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	printf("Value of thread 1: ""%" PRIu64 "\n", thread1VowelCount);
	printf("Value of thread 2: ""%" PRIu64 "\n", thread2VowelCount);
	printf("Result : ""%" PRIu64 "\n", thread1VowelCount + thread2VowelCount);

	return 0;
}

