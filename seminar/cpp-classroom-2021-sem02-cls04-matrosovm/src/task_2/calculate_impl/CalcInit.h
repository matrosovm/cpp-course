#ifndef _CALC_INIT_H_INCLUDED__
#define _CALC_INIT_H_INCLUDED__

#define DEF_CHAR_NUM 52 // number of defined characters which can be in string
#define DEF_CONST_NUM 2 // number of pre-defined contstants (pi and e)
#define MAX_FUNC_NAME 11 // maximal length of function's name

#define OLD_CONSTANT 0
#define CREATE_CONSTANT 1
#define RECREATE_CONSTANT 2

#define pi 3.1415926535897932384626433832795
#define euler 2.7182818284590452353602874713527

typedef struct tag_leaf
{
  double num; // if this leaf contains number (operand). ND! num-leaf has NO ancestors
  char func[MAX_FUNC_NAME]; // if this leaf contains hash of function. NB! func-leaf has only one ancestor
  char oper; // if this leaf contains operator. NB! oper-leaf has two ancestors
  char terminate; // if it has no sons terminate == 1 else 0
  struct tag_leaf *left;
  struct tag_leaf *right;
} leaf_t;

typedef struct tag_constant
{
  char *name;
  double value;
  int global;
  int toCount;
  int isEmpty;
  int isNew;
} constant_t;

int InitDefCharacters();
int InitConstants();
int AddConstant(char *name, int isGlobal);
int DestroyConstants();
int EqualStrings(char const *s1, char const *s2);
int InitLeaf(leaf_t *cur);

char g_defchar[DEF_CHAR_NUM]; // array of defined characters
constant_t *g_constants;
int g_numOfConstants;

#endif // CALC_INIT_H