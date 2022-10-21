/*
 * main.c
 *
 *  Created on: 19 Oct 2022
 *      Author: shriraam-sundaram
 */
#define _XOPEN_SOURCE 700
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
#include "count_vowels.h"


static int display_info(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
{
    char *fileName = "findMe.txt";
    /* fpath holds the full path of the file from the specified starting directory */
    if (strstr(fpath, fileName) )
    {
        printf("Match found!\n");
    }
    return 0;
}

void pf(const char * fileName)
{
	if(strstr(fileName, "findMe.txt"))
	{
		printf("call back called \n");
		printf("Match found!\n");
	}
}

void bfs (void(*pf)(const char *), const char * fileToSearch)
{
	DIR *directoryPtr;
	struct dirent *entryPtr;

	/*Get directory pointer of the said directory*/
	directoryPtr = opendir ("/home/shriraam-sundaram/find/");

	if (directoryPtr != NULL)
	{
		/* Get each directory entry which returns an entry pointer for further processing */
		while ((entryPtr = readdir (directoryPtr)) != NULL)
		{
			entryPtr->d_off
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
	return 0;

#if 0
	/*Remember where we are*/
	int fd = open(".", O_RDONLY);
	chdir("/");
	fchdir(fd);
	close(fd);
	/* If a starting directory isn't specified, use the current dir */
	if (nftw("/home", display_info, 20, 0) == -1)
	{
		printf("Error could not find file");

	}


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
#endif
}
