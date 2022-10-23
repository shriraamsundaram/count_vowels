/*
 * count_vowels.c
 *
 *  Created on: 19 Oct 2022
 *      Author: shriraam-sundaram
 */
#define _LARGEFILE64_SOURCE
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "count_vowels.h"

#define E_NOT_OK (-1)
#define NUM_RETRIES (100)

void* count_vowels(void * args)
{
	uint64_t numVowels = 0u;

	COUNT_VOWEL_ThreadArgs_t * arguments = (COUNT_VOWEL_ThreadArgs_t *)args;
	const char * fileName = arguments->fileName;

	uint64_t sizeToRead = (arguments->endIndex - arguments->startIndex);
	char * ch = malloc(sizeToRead * sizeof(char));

	int fd = open(fileName, O_RDONLY);

	if(E_NOT_OK != fd)
	{
		char * bufPos = ch;
		lseek64(fd, arguments->startIndex, SEEK_SET);

		uint64_t totalRead = 0u;
		for(uint64_t i = 0u; i < NUM_RETRIES; i++)
		{
			if(sizeToRead > 0u)
			{
				/*We get the actual read size because this is not guaranteed  that size requested = size read in the man page*/
				ssize_t actualReadSize = read(fd, bufPos, sizeToRead);

				/* We could also try mmap() if we think most of the files are large
				 * ch = (char *)mmap(0, sizeToRead*sizeof(char), PROT_READ, MAP_SHARED, fd, 0);
				 */
				if(E_NOT_OK != actualReadSize)
				{
					totalRead += actualReadSize;
					for(uint64_t j = 0u; j < actualReadSize; j++)
					{
						switch(ch[j])
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
					sizeToRead = sizeToRead - actualReadSize;
					if(sizeToRead <= 0)
					{
						/* We have completed reading so we end here*/
						break;
					}
					else
					{
						/*This is because of the fact that the size requested is less than the size read*/
						lseek(fd, arguments->startIndex + totalRead, SEEK_SET);
						bufPos = (char *)realloc(ch, sizeToRead * sizeof(char));
					}
				}
			}
		}
		close(fd);
	}
	*(arguments->result) = numVowels;
	free(ch);
	//munmap() here if mmap was used;
	return NULL;
}
