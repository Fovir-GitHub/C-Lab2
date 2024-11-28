#include "my_utility.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getString(char ** str, int max_length)
{
    char * temp_string = (char *) malloc(max_length * sizeof(char));
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

    *str = (char *) malloc((strlen(temp_string) + 1) * sizeof(char));
    if (!(*str)) /* failed to allocate memory */
    {
        // print error message
        fprintf(stderr, "Can't allocate memory for *str in %s()\n", __func__);
        exit(EXIT_FAILURE); /* terminate program */
    }

    strcpy(*str, temp_string); /* copy string */
    free(temp_string);         /* free temp string's memory */

    return;
}

void eatLine()
{
    while (getchar() != '\n') /* read until line breaker */
        continue;
}

int compare2Strings(const char * first_string, const char * second_string)
{
    // compare the length first
    int first_length = strlen(first_string),
        second_length = strlen(second_string);

    if (first_length != second_length)
        return first_length > second_length ? 1 : -1;
    // compare the alphabetical order
    int compare_result = strcmp(first_string, second_string);

    if (compare_result == 0) /* the same strings */
        return 0;

    return compare_result > 0 ? 1 : -1;
}