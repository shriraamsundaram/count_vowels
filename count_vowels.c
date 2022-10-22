/*
 * count_vowels.c
 *
 *  Created on: 19 Oct 2022
 *      Author: shriraam-sundaram
 */
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "count_vowels.h"

#define E_NOT_OK (-1)

void* count_vowels(void * args)
{
	COUNT_VOWEL_ThreadArgs_t * arguments = (COUNT_VOWEL_ThreadArgs_t *)args;
	const char * fileName = arguments->fileName;

	uint64_t numVowels = 0u;
	char ch;
	int fd = open(fileName, O_RDONLY);

	if(E_NOT_OK != fd)
	{
		lseek(fd, arguments->startIndex, SEEK_SET);

		for(uint64_t i = 0u; i < (arguments->endIndex - arguments->startIndex); i++)
		{
			read(fd, &ch, 1);
			switch(ch)
			{
				case 'a':
				case 'e':
				case 'i':
				case 'o':
				case 'u':
				case 'A':
				case 'E':
				case 'I':
				case 'O':
				case 'U':
					numVowels++;
					break;
				default:
					break;
			}
		}
		close(fd);
	}
	*(arguments->result) = numVowels;
	return NULL;
}
