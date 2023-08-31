#include <stdint.h>
#ifndef MPX_STDLIB_H
#define MPX_STDLIB_H

/**
 @file stdlib.h
 @brief A subset of standard C library functions.
*/

/**
 Convert an ASCII string to an integer
 @param s A NUL-terminated string
 @return The value of the string converted to an integer
*/
uint32_t hex2int(char *hex);
char itoa(int i);
int atoi(const char *s);
int isAlpha(int c);
#endif
