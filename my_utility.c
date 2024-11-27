#include "my_utility.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * getString(char ** str, int max_length)
{
	char * temp_string = (char *)malloc(max_length * sizeof(char));
	if (!temp_string) /* failed to allocate memory */
	{
		// print error info
		fprintf(stderr, "Can't allocate memory for temp_string in %s()\n",
		        __func__);
		exit(EXIT_FAILURE); /* terminate program */
	}

	fgets(temp_string, max_length, stdin); /* get temp string */

	char * find = strchr(temp_string, '\n'); /* find \n in temp string */
	if (find)                                /* if find */
		*find = '\0';                        /* replace with '\0' */
	else                                     /* not find */
		eatLine(); /* remove the remain characters until line breaker */

	*str = (char *)malloc(strlen(temp_string) * sizeof(char));
	if (!(*str)) /* failed to allocate memory */
	{
		// print error message
		fprintf(stderr, "Can't allocate memory for *str in %s()\n", __func__);
		exit(EXIT_FAILURE); /* terminate program */
	}

	strcpy(*str, temp_string); /* copy string */
	free(temp_string);         /* free temp string's memory */

	return *str;
}

void eatLine()
{
	while (getchar() != '\n') /* read until line breaker */
		continue;
}