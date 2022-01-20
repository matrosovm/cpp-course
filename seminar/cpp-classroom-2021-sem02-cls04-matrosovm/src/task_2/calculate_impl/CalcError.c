#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "CalcInit.h"
#include "CalcError.h"

static int _isSymbol(char const c);
static int _isNumberSymbol(char const c);
static int _isOperator(char const c);
static int _checkCharacters(char const *s);

int _isSymbol(char const c)
{
  if ((IsLetter(c)) || (IsDigit(c)))
    return 1;
  return 0;
}

int _isNumberSymbol(char const c)
{
  if ((c == '.') || (c == 'E'))
    return 1;
  if ((c >= '0') && (c <= '9'))
    return 1;
  if ((c >= 'a') && (c <= 'z'))
    return 1;
  return 0;
}

int DeleteSpaces(char *s)
{
  int i = 0;
  int j = 0;
  int k = 0;
  int len = (int)strlen(s);
  char prevSymbol = 0;

  while (s[i])
  {
    if (isspace((unsigned char)s[i]))
    {
      j = i;
      k = i;
      len = (int)strlen(s);
      while (isspace((unsigned char)s[++j]));
      
      /*let's find errors in number notation like 2. 2 2*/
      if ((_isNumberSymbol(prevSymbol)) && (_isNumberSymbol(s[j])))
      {
        SetError(ERR_NUMBER_NOTATION);
        return 1;
      }
      while (j <= len)
      {
        s[k] = s[j];
        j++;
        k++;
      }
    }
    else
    {
      if (!prevSymbol)
      {
        if ((s[i] == '/') && (s[i + 1] == '/'))
        {
          SetError(EMPTY_STRING);
          return 1;
        }
      }
      if ((_isOperator(prevSymbol)) && (s[i] == '+')) // we delete sensless unar pluses 
        s[i] = ' ';
      else
        prevSymbol = s[i++];
    }
  }
  return 1;
}

int _isOperator(char const c)
{
  return ((IsBinOperator(c)) || (c == '-'));
}

int IsBinOperator(char const c)
{
  switch (c)
  {
  case '+':
  case '*':
  case '/':
  case '^':
    return 1;
  }
  return 0;
}

int IsLetter(char const c)
{
  return ((c >= 'a') && (c <= 'z'));
}

int IsDigit(char const c)
{
  return ((c >= '0') && (c <= '9'));
}

int IsNan(double const x)
{
  if (x != x)
  {
    SetError(ERR_NAN);
    return 1;
  }
  return 0;
}

int IsOverFlow(double const x)
{
  if ((x > 1e308) || (x < -1e308))
  {
    SetError(ERR_OVERFLOW);
    return 1;
  }
  return 0;
}

int IsDivisionZero(double const x)
{
  if (x == 0)
  {
    SetError(ERR_DIV_ZERO);
    return 1;
  }
  return 0;
}

int BadString(char *s) // returns code of first detected error or 0 if there are no errors
{
  /*
    returns 0 if string is a correct math expression
    returns error code if string is incorrect
  */
  int len = 0;
  int i = 0;
  int k = 0;
  int numOfBrackets = 0;

  DeleteSpaces(s);
  len = (int)strlen(s);
  if (len == 0) // just empty string
    g_error = EMPTY_STRING;

  if (g_error)
    return EMPTY_STRING;

  // if there are some undefined symbols in string
  if ((k = _checkCharacters(s)) != 0)
  {
    g_error = ERR_CHAR_UNDEF;
    return ERR_CHAR_UNDEF;
  }

  /*new code*/
  for (i = 0; i < len; i++)
  {
    if (s[i] == '(')
    {
      if (s[i + 1] == ')')
        g_error = ERR_BR_OPEN_CLOSE;
      numOfBrackets++;
    }
    else if (s[i] == ')')
      numOfBrackets--;
    if (numOfBrackets < 0)
    {
      g_error = ERR_BR_NUM;
      return ERR_BR_NUM;
    }
  }
  if (numOfBrackets > 0)
  {
    g_error = ERR_BR_NUM;
    return ERR_BR_NUM;
  }
  /*end of new code*/
  return 0;
}

int _checkCharacters(char const *s)
{
  int i, j;
  int flag = 0;
  
  for (i = 0; i < (int)strlen(s); i++)
  {
    flag = 0;
    for (j = 0; j < DEF_CHAR_NUM; j++)
    {
      if (s[i] == g_defchar[j])
        flag = 1;
    }
    if (!flag)// returning first unknown character
      return s[i];
  }
  return 0;
}

void SetError(int error)
{
  if (!g_error)
    g_error = error;
}