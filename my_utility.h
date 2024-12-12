#ifndef _MY_UTILITY_H_
#define _MY_UTILITY_H_

#include <stdbool.h>

#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MY_DEBUG puts("DEBUG")

/**
 *@brief Get a string. This function will read a single line until line breaker
 *and ignore the remain characters.
 *
 * @param str the string want to get
 * @param max_length the maximum length of the string
 *
 * @return the string read in
 */
char * getString(char ** str, int max_length);

/**
 *@brief Ignore characters until line breaker.
 */
void eatLine();

/**
 *@brief Compare two strings in alphabetical order
 *
 * @param first_string the first string
 * @param second_string the second string
 * @return If first string is alphabetically less than the second one, it
 *returns -1. If the first string is alphabetically greater than the second one,
 *it returns 1. Otherwise, it returns 0.
 */
int compare2Strings(const char * first_string, const char * second_string);

/**
 *@brief Convert static string to string with dynamic memory allocation.
 *
 * @param static_string the original static string
 * @return char* the dynamic string after convertation.
 */
char * staticString2dynamicString(char * static_string);

/**
 *@brief Clear screen output.
 */
void clearScreen();

/**
 *@brief Judge whether the string is legal.
 *
 * @param str the string to be judged
 * @return true the string is legal
 * @return false the string is illegal
 */
bool legalString(char * str);

/**
 *@brief Encode string.
 *
 * @param str the string to be encode
 */
void encodeString(char * str);

/**
 *@brief Decode string.
 *
 * @param str the string to be decode
 */
void decodeString(char * str);

/**
 *@brief Judge whether the directory exists. If not exists, create the folder.
 *
 * @param dir_name the folder name
 */
void ensureFolder(const char * dir_name);

/**
 *@brief Judge whether a string is empty string.
 *
 * @param str the string to be judged
 * @return true the string is empty
 * @return false the string is not empty
 */
bool emptyString(char * str);

int getNumberofDigits(int number);

#endif // !_MY_UTILITY_H_