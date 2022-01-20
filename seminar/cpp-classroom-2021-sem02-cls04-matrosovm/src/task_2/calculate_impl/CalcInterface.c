#include <stdio.h>
#include <stdlib.h>
#include "CalcError.h"
#include "CalcInterface.h"

static int _errorHandler(char const *s);

int _errorHandler(char const *s)
{   
  if (g_error != EMPTY_STRING)
    printf("%s == ", s);
  switch(g_error)
  {
  case ERR_MEMORY_LACK:
    printf("ERROR: lack of memory");
    break;
  case ERR_NUMBER_NOTATION:
    printf("ERROR: wrong number notation (possibly spaces in number)");
    break;
  case ERR_BR_OPEN_CLOSE:
    printf("ERROR: close bracket can't go after open bracket");
    break;
  case ERR_FU_UNDEF:
    printf("ERROR: undefined function in string");
    break;
  case ERR_CONST_RECURSION:
    printf("ERROR: recursion in variable definition");
    break;
  case ERR_CONST_NAME:
    printf("ERROR: wrong variable name");
    break;
  case EMPTY_STRING:
    printf("%s", s);
    break;
  case ERR_BR_NUM:
    printf("ERROR: wrong number of brackets");
    break;
  case ERR_CHAR_UNDEF:
    printf("ERROR: undefined characters in string");
    break;
  case ERR_CONST_UNDEF:
    printf("ERROR: undefined constant in expression");
    break;
  case ERR_DIV_ZERO:
    printf("ERROR: division by zero");
    break;
  case ERR_NO_OPERAND:
    printf("ERROR: binary operator has less than two operands");
    break;
  case ERR_NO_OPERATOR:
    printf("ERROR: two operands must be divided by binary operator");
    break;
  case ERR_OVERFLOW:
    printf("ERROR: double type overflow");
    break;
  case ERR_NAN:
    printf("ERROR: illegal argument of function");
    break;
  case ERR_OP_NEAR:
    printf("ERROR: two operands are not divided by operator");
    break;
  }
  g_error = 0;
  return 0;
}

char *ReadString(FILE *f) // in main procedure we will ignore comments, here we don't care
{                         // what we read
  char *s = NULL;
  char *buffer = NULL;
  int i = 0;
  int size = DEF_SIZE;

  s = (char*)malloc(size);
  while ((s[i] = (char)fgetc(f)) != '\n')
  {
    if (i == size - 1)
    {
      buffer = (char*)realloc(s, size + 100);
      if (buffer)
      {
        s = buffer;
        size += 100;
      }
      else 
      {
        printf("Lack of RAM. Please free RAM before using calculator.");
        return NULL;
      }
    }
    if (feof(f))
    {
      s[i] = 0;
      break;
    }
    i++;
  }
  s[i] = 0;
  return s;
}

int PrintResult(char const *s, double result, int isFeof)
{
  if (!g_error)
    printf("%s == %g", s, result);
  else 
    _errorHandler(s);

  if (!isFeof)
    printf("\n");
  return 0;
}