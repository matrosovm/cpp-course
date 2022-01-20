#ifndef _CALC_ALGORYTHM_H_INCLUDED__
#define _CALC_ALGORYTHM_H_INCLUDED__

#include "CalcInit.h"

#define MAX_INT (unsigned)0xFFFFFFFF

#define BR_PRIORITY 4
#define POW_PRIORITY 3
#define MUL_PRIORITY 2
#define SUM_PRIORITY 1

#define PI 20
#define EULER 21

int BuildTree(char *s, leaf_t *root);
double GetResult(leaf_t *root);
int DestroyTree(leaf_t *root);

#endif // CALC_ALGORYTHM_H