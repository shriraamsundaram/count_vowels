/*
 * main.c
 *
 *  Created on: 19 Oct 2022
 *      Author: shriraam-sundaram
 */
#define _XOPEN_SOURCE 700
#define E_NOT_OK (-1)
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

static bool fileFound = false;

void pf(const char * fileName)
{
	if(strstr(fileName, "findMe.txt"))
	{
		printf("call back called \n");
		printf("Match found!\n");
		fileFound = true;
	}
}

void bfs (void(*pf)(const char *), const char * fileToSearch)
{
	DIR *directoryPtr;
	struct dirent *entryPtr;

	/*Get directory pointer of the said directory*/
	directoryPtr = opendir ("/home/shriraam-sundaram/find/test/");

	if (directoryPtr != NULL)
	{
		/* Get each directory entry which returns an entry pointer for further processing */
		while ((entryPtr = readdir (directoryPtr)) != NULL)
		{
		  puts (entryPtr->d_name);
		  if (strstr(entryPtr->d_name, fileToSearch) )
		  {
			  pf(entryPtr->d_name);
		  }
		}

		(void) closedir (directoryPtr);
		return;
	}
	else
	{
		perror ("Couldn't open the directory");
		return;
	}
}

int main(int argc, char ** argv)
{
	printf("Hello World! \n");
	printf("%s \n", argv[1]);

	bfs(pf, argv[1]);

	fileFound = true;

	if(fileFound)
	{
		pthread_t thread1, thread2;

		uint64_t thread1VowelCount = 0u;
		uint64_t thread2VowelCount = 0u;
		int thread1Ret = 0;
		int thread2Ret = 0;
		int fd = -1;
		struct flock region;

		fd = open("/home/shriraam-sundaram/find/test/findMe.txt", O_RDONLY);

		if(fd != -1)
		{
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
				printf("Could not obtain lock \n");
			}
			else
			{

				/*Create worker threads to complete the job*/
				thread1Ret = pthread_create( &thread1, NULL, count_vowels, (void *)(&((COUNT_VOWEL_ThreadArgs_t){argv[1], 1, 5,  &thread1VowelCount})));
				thread2Ret = pthread_create( &thread2, NULL, count_vowels, (void *)(&((COUNT_VOWEL_ThreadArgs_t){argv[1], 6, 11, &thread2VowelCount})));


				/*Wait until the worker thread completes its job*/
				pthread_join(thread1, NULL);
				pthread_join(thread2, NULL);

				printf("Value of thread 1: ""%" PRIu64 "\n", thread1VowelCount);
				printf("Value of thread 2: ""%" PRIu64 "\n", thread2VowelCount);
				printf("Result : ""%" PRIu64 "\n", thread1VowelCount + thread2VowelCount);
			}
			region.l_type = F_UNLCK;
			if(E_NOT_OK == fcntl(fd, F_SETLK, &region))
			{
				printf("FAILED TO UNLOCK\n");
			}
		}
	}
	return 0;
}
