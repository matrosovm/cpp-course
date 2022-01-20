#ifndef _CALC_INTERFACE_H_INCLUDED__
#define _CALC_INTERFACE_H_INCLUDED__

#include <stdio.h>

#define DEF_SIZE 100

char *ReadString(FILE *f);
int PrintResult(char const *s, double result, int isFeof);

#endif // CALC_INTERFACE_H