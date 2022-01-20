#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Calc.h"

int main(int argc, char **argv)
{
  char *s = NULL;
  char *s1 = NULL;
  double result = 0;
  int isFeof = 0;
  int opened = 0;
  FILE *f = NULL;
  leaf_t *root = NULL;
  
  InitDefCharacters();
  InitConstants();

  switch (argc)
  {
  case 1:
    //f = fopen("input.txt", "r");
    //opened = 1;
    f = stdin;
    break;
  case 2:
    if ((f = fopen(argv[1], "r")) == NULL)
    {
      printf("ERROR: file not found");
      return 1;
    }
    opened = 1;
    break;
  case 3:
    printf("ERROR: too many input parameters!");
    return 1;
  }

  do
  {
    s = ReadString(f);
    root = (leaf_t*)malloc(sizeof(leaf_t));
    InitLeaf(root); 
    s1 = (char*)malloc(strlen(s) + 1);
    strcpy(s1, s);
    isFeof = feof(f);

    if (!BadString(s1))
    {
      BuildTree(s1, root);
      result = GetResult(root);
      IsNan(result);
      IsOverFlow(result);
      DestroyTree(root);
    }
    else  // bad string or comment
    {
      free(root);
      if (s1)
        free(s1);
    }
    s1 = NULL;

    PrintResult(s, result, isFeof);
    if (s)
    {
      free(s);
      s = NULL;
    }
  } while (!feof(f));
  if (opened)
    fclose(f);

  DestroyConstants();
  return 0;
}