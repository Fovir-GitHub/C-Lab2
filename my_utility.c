#include "my_utility.h"
#include "constants.h"
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define ENCRYPTION_OFFSET 47           /* the offset of encryption */
#define MIN_VISIBLE_CHARACTER_ASCII 32 /* the minimal visible character */
#define VISIBLE_CHARACTER_NUMBER 95    /* the number of all visible character */

/**
 *@brief Encode single character.
 *
 * @param ch the character to be encoded
 * @return char the character after encoding
 */
static char encodeCharacter(char ch);

/**
 *@brief Decode single character.
 *
 * @param ch the character to be decoded.
 * @return char the character after decode
 */
static char decodeCharacter(char ch);

char * getString(char ** str, int max_length)
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

    return *str;
}

void eatLine()
{
    while (getchar() != '\n' && !feof(stdin)) /* read until line breaker */
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

char * staticString2dynamicString(char * static_string)
{
    // allocate memory
    char * dynamic_string =
        (char *) malloc((strlen(static_string) + 1) * sizeof(char));

    if (!dynamic_string)
    {
        fprintf(stderr, "Can't allocate memory for dynamic_string in %s()\n",
                __func__);  /* print error message */
        exit(EXIT_FAILURE); /* terminate program */
    }

    // copy string
    strcpy(dynamic_string, static_string);

    return dynamic_string;
}

void clearScreen()
{
// On Windows platform
#ifdef _WIN32
    system("cls");
#endif

// On Unix system like Linux, MacOS and so on
#ifdef __unix__
    system("clear");
#endif

    return;
}

bool legalString(char * str)
{
    static const char * ILLEGAL_CHRACTERS = "@\t";
    char * iter = str;

    if (!str) /* NULL pointer */
        return false;

    if (strcmp(str, "quit") == 0)
        return false;

    if (emptyString(str)) /* empty string */
        return false;

    while (*iter)
        if (strchr(ILLEGAL_CHRACTERS, *iter++))
            return false;

    return true;
}

void encodeString(char * str)
{
    while (*str) *str++ = encodeCharacter(*str);
}

void decodeString(char * str)
{
    while (*str) *str++ = decodeCharacter(*str);
}

void ensureFolder(const char * dir_name)
{
// corss-plateform complie
#ifdef _WIN32

#include <direct.h>
#define mkdir(dir, mode) _mkdir(dir)

#endif

    DIR * dir = opendir(dir_name);

    if (dir)
        closedir(dir);
    else
    {
        if (mkdir(dir_name, 0755) != 0)
        {
            fprintf(stderr, "Can't create folder %s\n", dir_name);
            exit(EXIT_FAILURE);
        }
    }
}

bool emptyString(char * str)
{
    return strcmp(str, "") == 0;
}

int getNumberofDigits(int number)
{
    int number_of_digits = 1;

    while (number /= 10) number_of_digits++;

    return number_of_digits;
}

int readNumberOrAlpha()
{
    // read in a line
    char * input = getString(&input, READ_NUMBER_OR_ALPHA_BUFFER_SIZE);
    int result = 0; /* the result */

    // the input is not a number
    if (sscanf(input, "%d", &result) != 1)
    {
        // the input is not a single character
        if (sscanf(input, "%c", &result) != 1)
            return READ_NUMBER_OR_ALPHA_ERROR; /* return error */

        // the input is a single character
        switch (tolower(result))
        {
        case 'p':
            result = READ_NUMBER_OR_ALPHA_P;
            break;
        case 'q':
            result = READ_NUMBER_OR_ALPHA_Q;
            break;
        case 'n':
            result = READ_NUMBER_OR_ALPHA_N;
            break;
        default: /* invalid choice */
            result = READ_NUMBER_OR_ALPHA_ERROR;
        }
    }

    free(input); /* free the memory space */

    return result;
}

char encodeCharacter(char ch)
{
    return (ch - MIN_VISIBLE_CHARACTER_ASCII + ENCRYPTION_OFFSET) %
               VISIBLE_CHARACTER_NUMBER +
           MIN_VISIBLE_CHARACTER_ASCII;
}

char decodeCharacter(char ch)
{
    return (ch - MIN_VISIBLE_CHARACTER_ASCII - ENCRYPTION_OFFSET +
            VISIBLE_CHARACTER_NUMBER) %
               VISIBLE_CHARACTER_NUMBER +
           MIN_VISIBLE_CHARACTER_ASCII;
}