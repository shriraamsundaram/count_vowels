/*
 * count_vowels.h
 *
 *  Created on: 19 Oct 2022
 *      Author: shriraam-sundaram
 */

#ifndef COUNT_VOWELS_H_
#define COUNT_VOWELS_H_

#include <stdint.h>

typedef struct COUNT_VOWEL_ThreadArgs_t_
{
	char * fileName;
	uint64_t startIndex;
	uint64_t endIndex;
	uint64_t * result;

}COUNT_VOWEL_ThreadArgs_t;

void* count_vowels(void * args);

#endif /* COUNT_VOWELS_H_ */
