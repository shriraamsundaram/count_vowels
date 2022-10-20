/*
 * count_vowels.c
 *
 *  Created on: 19 Oct 2022
 *      Author: shriraam-sundaram
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "count_vowels.h"

#define E_NOT_OK (-1)

void* count_vowels(void * args)
{
	COUNT_VOWEL_ThreadArgs_t * arguments = (COUNT_VOWEL_ThreadArgs_t *)args;
	const char * fileName = arguments->fileName;
	const char * mode = "r";

	uint64_t numVowels = 0u;
	char ch;
	FILE *fp = fopen(fileName, mode);

	if(NULL != fp)
	{
		fseek(fp, arguments->startIndex, SEEK_SET);

		for(uint64_t i = 0u; i < (arguments->endIndex - arguments->startIndex); i++)
		{
			ch = getc(fp);
			if(feof(fp))
			{
				break;
			}
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
		fclose(fp);
	}
	*(arguments->result) = numVowels;
	return NULL;
}
