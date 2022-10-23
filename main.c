/*
 * main.c
 *
 *  Created on: 19 Oct 2022
 *      Author: shriraam-sundaram
 */
#define _XOPEN_SOURCE 700
#define E_NOT_OK (-1)
#define __USE_LARGEFILE64
#define __USE_FILE_OFFSET64

#include <ftw.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdbool.h>
#include "count_vowels.h"

#define NUM_THREADS (20u)

int main(int argc, char ** argv)
{
#if defined(DEBUG_MODE)
	clock_t begin = clock();
#endif

	pthread_t thread1[NUM_THREADS];
	COUNT_VOWEL_ThreadArgs_t threadArgs[NUM_THREADS];

	uint64_t threadVowelCount[10];
	(void)memset(threadVowelCount, 0, NUM_THREADS*sizeof(uint64_t));
	int threadRet = 0;
	int fd = -1;
	off_t size = 0;
	struct flock region;

	fd = open(argv[1], O_RDONLY);

	if(fd != -1)
	{
		/* To get the file statistics which will include file size */
		struct stat fileStats;

		if(E_NOT_OK != fstat(fd, &fileStats))
		{
			size = fileStats.st_size;
#if defined(DEBUG_MODE)
			printf("Size: %lu\n", (uint64_t)fileStats.st_size);
#endif
		}

		/*We apply read lock*/
		region.l_type = F_RDLCK;

		/*Relative position --It defines the offset to which l_start, the first byte in the region*/
		region.l_whence = SEEK_SET;
		region.l_start = 0;

		/*Length 0 means the whole file is locked for read*/
		region.l_len = 0;
		region.l_pid = -1;


		if(E_NOT_OK == fcntl(fd, F_SETLK, &region))
		{
			printf("Could not obtain lock \n"); // We could add retry mechanisms.
		}
		else
		{
			/*Create worker threads to complete the job*/
			for(int i = 0; i < NUM_THREADS; i++)
			{
				threadArgs[i].startIndex= (i * size)/NUM_THREADS;
				threadArgs[i].endIndex = ((i+1) * size)/NUM_THREADS;
				threadArgs[i].fileName = argv[1];
				threadArgs[i].result = (uint64_t *)malloc(sizeof(uint64_t));
				threadRet = pthread_create( &thread1[i], NULL, count_vowels, (void *)(&(threadArgs[i])));
				if(threadRet != 0)
				{
					printf("Thread creation failed: Thread %d\n", i);
				}
			}

			uint64_t threadTotalVowelCount = 0u;

			/*Wait until the worker thread completes its job*/
			for(int i = 0; i < NUM_THREADS; i++)
			{
				pthread_join(thread1[i], NULL);
			}

			for(int i = 0; i < NUM_THREADS; i++)
			{
#if defined(DEBUG_MODE)
				printf("Value of thread %d: ""%" PRIu64 "\n", i, *(threadArgs[i].result));
#endif
				threadTotalVowelCount += *(threadArgs[i].result);
				free(threadArgs[i].result);
			}

			printf("Result : ""%" PRIu64 "\n", threadTotalVowelCount);
		}
		region.l_type = F_UNLCK;
		if(E_NOT_OK == fcntl(fd, F_SETLK, &region))
		{
			printf("FAILED TO UNLOCK\n");
		}

		/* here, do your time-consuming job */
#if defined(DEBUG_MODE)
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		printf("Total time spent : %f\n", time_spent);
#endif
	}
	return 0;
}
