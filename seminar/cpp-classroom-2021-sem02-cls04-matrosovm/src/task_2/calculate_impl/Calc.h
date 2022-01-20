#ifndef _CALC_H_INCLUDED__
#define _CALC_H_INCLUDED__

#include "CalcInterface.h"
#include "CalcError.h"
#include "CalcAlgorythm.h"
#include "CalcInit.h"

int BadString(char *s);
int BuildTree(char *s, leaf_t *root);
int DestroyConstants();
int DestroyTree(leaf_t *root);
double GetResult(leaf_t *root);
int InitDefCharacters();
int InitConstants();
int InitLeaf(leaf_t *cur);
int IsOverFlow(double const x);
int IsNan(double const x);
int PrintResult(char const *s, double result, int isFeof);
char *ReadString(FILE *f);

#endif