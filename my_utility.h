#ifndef _MY_UTILITY_H_
#define _MY_UTILITY_H_

#define MY_DEBUG puts("DEBUG")

/**
 *@brief Get a string. This function will read a single line until line breaker
 *and ignore the remain characters.
 *
 * @param str the string want to get
 * @param max_length the maximum length of the string
 * @return the result of the input string
 */
void getString(char ** str, int max_length);

/**
 *@brief Ignore characters until line breaker.
 */
void eatLine();

#endif // !_MY_UTILITY_H_