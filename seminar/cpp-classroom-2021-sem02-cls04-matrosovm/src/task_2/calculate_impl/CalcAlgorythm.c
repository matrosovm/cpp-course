#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "CalcError.h"
#include "CalcAlgorythm.h"

#define DOUBLE_ARGUMENT 2

static int _isScientificForm(char prev, char prevPrev);
static int _deleteSpaces(char *s);
static char *_deleteBorderBrackets(char const *s);
static int _parseString(char *s1);
static int _shiftUnarMinus(char const *s1, int i);
static double _determineConstant(char const *c);
static int _determineFunction(char const *s, elementaryFunction *func);
static double _count(leaf_t *root);

int _isScientificForm(char prev, char prevPrev)
{
  return (((prev == 'e') || (prev == 'E')) && (IsDigit(prevPrev)));
}

int min(int a, int b) {
  return a < b ? a : b;
}

char *_deleteBorderBrackets(char const *s)
{
  /*if expression is smth like '(...)' we delete border brackets*/
  char *s1 = NULL;
  int len = 0; // just as a buffer cause its easier to write then strlen(s)
  int i = 0, j = 0, p = 0;
  int shift = 0;
  int leftShift = 0;
  int rightShift = 0;

  if (!strlen(s))
  {
    s1 = (char*)malloc(1);
    s1[0] = 0;
    return s1;
  }
  len = (int)strlen(s);
  while (s[shift] == '(')
  {
    j++;  // counting number of open-brackets on the left  
    shift++;
  }
  p = j;

  j = 0;
  i = len - 1;
  while (i > shift)
  {
    switch (s[i])
    {
    case ')':
      j++;
      break;
    case '(':
      j--;
      p = min(p, j); //
      break;
    default:
      p = min(p, j);
      break;
    }
    i--;
  }

  j = p;
  while (j)
  {
    if (s[leftShift] == '(')
      j--;
    leftShift++;
  }

  while (p)
  {
    if (s[len - 1 - rightShift] == ')')
      p--;
    rightShift++;
  }

  s1 = (char*)malloc(len + 1);
  if (s1 == NULL)
  {
    SetError(ERR_MEMORY_LACK);
    return NULL;
  }
  strcpy(s1, s + leftShift);
  s1[len - rightShift - leftShift] = 0;
  return s1;
}


int _parseString(char *s1)
{
  /*Returns number of "tear" element in string - element which will divide
  our string into two parts. If there is only one part, it returns -1. It means we have 
  reached an operand.*/
  int i = 0;
  int j = 0;
  int k = 0;
  int tearPlace = -1;
  char prevSymbol = 0; // previous symbol. We need it to define unar minus
  char prevPrevSymbol = 0;
  unsigned priority = 0;
  unsigned minPriority = MAX_INT;

  for (i = 0; i < (int)strlen(s1); i++)// looking for the least priority
  {
    switch(s1[i])
    {
    case '=':
      if (!prevSymbol)
      {
        SetError(ERR_CONST_NAME);
        return i;
      }
      if (prevSymbol == ':') // it's global variable
      {
        j = 0;
        while ((s1[j] != ';') && (s1[j]))
          j++;
        if (s1[j] == ';')
          return j;
        j = i - 1;

        /*let's find end of constant name*/
        j--;
        if (j < 0) // smth like :=5 (constant has no name - it's error)
        {
          SetError(ERR_CONST_NAME);
          return i; // tearplace should be this one
        }

        s1[++j] = 0;

        k = AddConstant(s1, 1);
        s1[i - 1] = ':';

        SetError(k);
        return i; // tearplace should be this one
      }
      else // local variable
      {
        j = 0;
        while ((s1[j] != ';') && (s1[j]))
          j++;
        if (s1[j] == ';')
          return j;

        /*let's find end of constant name*/
        j = i - 1;
        if (j < 0) // smth like :=5 (constant has no name - it's error)
        {
          SetError(ERR_CONST_NAME);
          return i; // tearplace should be this one
        }

        k = 0;
        s1[++j] = 0;

        k = AddConstant(s1, 0);
        s1[i] = '=';
        SetError(k);
        return i; // tearplace should be this one
      }

    case '(':
      // in case the only thing that we have in string is function like 'sin(x)'
      priority += BR_PRIORITY;
      if ((priority < minPriority) || (minPriority == MAX_INT)) 
      {
        minPriority = priority;
        tearPlace = i;
      }
      break;
    case ')':
      priority -= BR_PRIORITY;
      break;
    case '+':
    case '-':
      if (((priority + SUM_PRIORITY) <= minPriority) || (minPriority == MAX_INT))
      {
        if ((s1[i] == '-') && (IsBinOperator(prevSymbol)))// detecting unar minus: let's define it and not count as if it's a binar minus
          break;
        else if ((s1[i] == '-') && (_isScientificForm(prevSymbol, prevPrevSymbol))) // if it's kinda 25E-4 
          break;
        else if ((s1[i] == '+') && (_isScientificForm(prevSymbol, prevPrevSymbol))) // if it's kinda 25   e + 4
          break;
        minPriority = priority + SUM_PRIORITY;
        tearPlace = i;
      }
      break;
    case '*':
    case '/':
      if ((priority + MUL_PRIORITY) <= minPriority)
      {
        minPriority = priority + MUL_PRIORITY;
        tearPlace = i;
      }
      break;
    case '^':
      if ((priority + POW_PRIORITY) < minPriority)
      {
        minPriority = priority + POW_PRIORITY;
        tearPlace = i;
      }
      break;
    default: 
      break;
    }
    prevPrevSymbol = prevSymbol;
    prevSymbol = s1[i];
  }
  return tearPlace;
}

int BuildTree(char *s, leaf_t *root) // works with correct string, not comments.
{
  char *right = NULL, *left = NULL; // left and right parts of the string 
  char *rLog = NULL, *lLog = NULL;
  char *s1 = NULL;
  int i = 0, j = 0;

  if (!root->left)
    root->left = (leaf_t*)malloc(sizeof(leaf_t));
  if (!root->right)
    root->right = (leaf_t*)malloc(sizeof(leaf_t));

  if ((!root->left) || (!root->right))
  {
    SetError(ERR_MEMORY_LACK);
    free(s);
    return 1;
  }

  InitLeaf(root->left);
  InitLeaf(root->right);

  /*
    Simple plan is keep building a tree until we get in each leaf 
    - an operator
    - a function
    - an operand
   */
  s1 = _deleteBorderBrackets(s);
  free(s);
  s = NULL;
  i = _parseString(s1); // it's number of character which divides two parts of expression
  if (g_error)
  {
    free(s1);
    return 1;
  }
  /*
    Now we should decide how to tear a string
    - if tear-place symbol is '*+-/^=' then 
      1. it goes to the root
      2. we strcpy left and right part to another strings, call Evaluate(s1, left); 
         Evaluate(s2, right)
         if last symbol of the string is ')' then we will delete first '(' and last ')'

    - if tear-place is '(' then the only thing we have in string is function f(x)
    if so we do:
      1. to the root we write the name of the function
      2. to the ONLY son of this root (it MUST be right son) we write argument
      3. we call Evaluate(s1, right) - evaluate an argument

    - if tear place is number we have a leaf and it has no sons!
      
  */
  if (i == -1) // no tears so current string is an operand
  {
    free(root->left);
    free(root->right);
    root->left = NULL;
    root->right = NULL;
    root->terminate = 1;
    if (!strlen(s1))
    {
      free(s1);
      return 1;
    }

    //if constant convert it to double.
    i = (int)strlen(s1);
    s1[i] = 0;
    j = i;
    i = 0;

    if (!(root->num = _determineConstant(s1 + i)))
      root->num = atof(s1 + i);

    while (s1[i])
      i++;
    if (i < (int)strlen(s1))    // we have deleted all spaces at the end                         
    {                           //so if we havent reached 0 there are 2 operands (error)
      if (!g_error)
        g_error = ERR_NO_OPERATOR;
      free(s1);
      return 1;
    }
    free(s1);
    return 0;
  }

  switch (s1[i]) 
  {
  case '=':
    right = (char*)malloc(strlen(s1 + i));
    strcpy(right, s1 + i + 1); // if we have sin(pi) right will be (pi)

    if (!strlen(right)) // if there is no first operand
    {
      if (!g_error)
      {
        g_error = ERR_NO_OPERAND;
      }
    }

    i--; // x  := 5 or x  = 5  ; ??

    if (s1[i] == ':')
      root->oper = ':'; // means it's a global assigning
    else
      root->oper = '=';
    BuildTree(right, root->right);
    _count(root);
    free(s1);
    break;
  case '+':
  case '-':
  case '*':
  case '/':
  case '^':
  case ';':
    
    if ((s1[i] == '-') && (i > 0)) // detecting unar minus
      i = _shiftUnarMinus(s1, i);
    
    root->oper = s1[i];
    j = (int)strlen(s1);
    s1[i] = 0;

    left = (char*)malloc(strlen(s1) + 1);
    if (!left)
    {
      SetError(ERR_MEMORY_LACK);
      free(s1);
      return 1;
    }
    strcpy(left, s1);

    right = (char*)malloc(j - strlen(s1) + 1);
    if (!right)
    {
      SetError(ERR_MEMORY_LACK);
      free(s1);
      free(left);
      return 1;
    }
    strcpy(right, s1 + i + 1);
    free(s1); // free what we 've taken

    if (!strlen(left)) // if ithere is no first operand
    {
      if ((root->oper != '-') && (root->oper != '+'))
      {
        SetError(ERR_NO_OPERAND);
        free(left);
        free(right);
        return 1;
      }
    }
    if (!strlen(right)) // if there is no first operand
    {
      SetError(ERR_NO_OPERAND);
      free(left);
      free(right);
      return 1;
    }
    BuildTree(left, root->left);
    BuildTree(right, root->right);
    break;

  case '(': // the left part is function
    j = 0;

    right = (char*)malloc(strlen(s1) + 1);
    if (!right)
    {
      SetError(ERR_MEMORY_LACK);
      free(s1);
      return 1;
    }
    strcpy(right, s1 + i); // if we have sin(pi) right will be (pi)

    s1[i] = 0;
    if ((strlen(s1) > 10) || (s1[0] == 0))
    {
      free(right);
      free(s1);
      SetError(ERR_FU_UNDEF);
      return 1;
    }
    if (EqualStrings(s1, "log")) // it has two arguments which is not usual for us
    {
      free(s1);
      s1 = _deleteBorderBrackets(right);
      free(right);

      root->oper = '/';
      strcpy(root->left->func, "ln");
      strcpy(root->right->func, "ln");
      i = 0;
      while (s1[i] != ',')
        i++;
      lLog = (char*)malloc(i + 1);
      if (!lLog)
      {
        SetError(ERR_MEMORY_LACK);
        free(s1);
        return 1;
      }

      for (j = 0; j < i; j++)
        lLog[j] = s1[j];
      lLog[j] = 0;

      rLog = (char*)malloc(strlen(s1 + i));
      if (!rLog)
      {
        SetError(ERR_MEMORY_LACK);
        free(s1);
        free(lLog);
        return 1;
      }
      i++; // because right[i] == ','
      strcpy(rLog, s1 + i);

      root->left->right = (leaf_t*)malloc(sizeof(leaf_t));
      root->right->right = (leaf_t*)malloc(sizeof(leaf_t));
      if ((!root->right->right) || (!root->left->right))
      {
        SetError(ERR_MEMORY_LACK);
        free(s1);
        free(lLog);
        free(rLog);
        return 1;
      }

      InitLeaf(root->left->right);
      InitLeaf(root->right->right);
      BuildTree(rLog, root->left->right);
      BuildTree(lLog, root->right->right);
    }
    else
    {
      strcpy(root->func, s1); /*here it s awfully shit!! cause big functions cause death*/
      free(root->left);
      root->left = NULL;// if not log-function then there is only one son!!11
    }
    free(s1);
    if (root->func[0])
      BuildTree(right, root->right); 
    break;
  }
  return 0;
}

int _shiftUnarMinus(char const *s1, int i)
/*if tear-symbol is '-' we should be sure it's first operator after left operand.
if it's not we need to find first one and make it tear-symbol.*/
{
  int j = 0;

  while (s1[i] == '-')
    i--;
  i++;
  j = i;
  if (i > 0)
  {
    while (s1[i - 1] == '-') // if we have a construction like 2* -3
    {
      j = i - 1;
      i--;
      if (i == 0)
        break;
    }
    if (IsBinOperator(s1[i - 1])) // if our exprssion is eg 2^ -2 it is 2^(-2) our operand should be ^
      i--;
    else i = j; // if it's not 2* -3 but just 2  - 3 our opernad should be -
  }
  return i;
}

double ctan(double x)
{
  return cos(x) / sin(x);
}

double actan(double x)
{
  return (double)pi / 2 - atan(x);
}

int _determineFunction(char const *s, elementaryFunction *func)
{
  *func = fabs;
  if (EqualStrings("sin", s))
    *func = sin;
  if (EqualStrings("cos", s))
    *func = cos;
  if (EqualStrings("tg", s))
    *func = tan;
  if (EqualStrings("ctg", s))
    *func = ctan;
  if (EqualStrings("arcsin", s))
    *func = asin;
  if (EqualStrings("arccos", s))
    *func = acos;
  if (EqualStrings("arctg", s))
    *func = atan;
  if (EqualStrings("arcctg", s))
    *func = actan;
  if (EqualStrings("ln", s))
    *func = log;
  if (EqualStrings("sqrt", s))
    *func = sqrt;
  if (EqualStrings("ceil", s))
    *func = ceil;
  if (EqualStrings("floor", s))
    *func = floor;
  if (EqualStrings("log", s))
  {
    *func = log;
    return DOUBLE_ARGUMENT;
  }
  if (*func != fabs)
    return 1;
  return 0;
}

double _determineConstant(char const *c)
{
  int i;
  for (i = 0; i < g_numOfConstants; i++)
  {
    if (!g_constants[i].isEmpty)
    {
      if (EqualStrings(g_constants[i].name, c))
      {
        if ((g_constants[i].toCount) && (g_constants[i].isNew == CREATE_CONSTANT))
        {
          g_constants[i].global = 0;
          SetError(ERR_CONST_RECURSION);
        }
        return g_constants[i].value;
      }
    }
  }

  /*well, it's not a constant. Let's find out if it's a correct number*/
  i = 0; 
  while (IsDigit(c[i]))
    i++;
  if (c[i] == '.')
    i++;
  while (IsDigit(c[i]))
    i++;

  if ((c[i] == 'e') || (c[i] == 'E'))
  {
    i++;
    if ((c[i] == '-') || (c[i] == '+'))
      i++;
  }
  while (IsDigit(c[i]))
    i++;

  if (i < (int)strlen(c)) // it's mot a correct number
  {
    SetError(ERR_CONST_UNDEF);
  }
  return 0;
}

double _count(leaf_t *root)
{
  double buffer = 0;
  int i = 0;
  elementaryFunction *f = NULL;

  if (g_error) // it makes no sense to count any more
    return 1;

  if (root->terminate)
    return root->num;

  switch (root->oper)
  {
  case '=':
  case ':':
    while ((!g_constants[i].toCount) && (i < g_numOfConstants))
      i++;
    if (i < g_numOfConstants)
    {
      g_constants[i].value = _count(root->right); 
      g_constants[i].toCount = 0;
      return g_constants[i].value;
    }
    else // recursion in defining constant detcted
    {
      g_constants[i].global = 0;
      SetError(ERR_CONST_RECURSION);
      return 1;
    }
  case ';':
    return _count(root->right);
  case '+':
    return _count(root->left) + _count(root->right);
  case '-':
      return _count(root->left) - _count(root->right);
  case '*':
      return _count(root->left) * _count(root->right);
  case '/':
    buffer = _count(root->right);
    IsDivisionZero(buffer);
    return (double)_count(root->left) / buffer;
  case '^':
    buffer = _count(root->left);
    buffer = pow(buffer, _count(root->right));

    IsNan(buffer);
    return buffer;
  }

  if (root->func[0])
  {
    f = (elementaryFunction*)malloc(sizeof(elementaryFunction));
    if (!_determineFunction(root->func, f))
    {
      free(f);
      SetError(ERR_FU_UNDEF);
      return 0;
    }
    buffer = (*f)(_count(root->right));
    free(f);

    IsNan(buffer);
    IsOverFlow(buffer);
    return buffer;
  }
  return 0;
}

double GetResult(leaf_t *root)
{
  double result = 0;
  int i;

  i = g_numOfConstants - 1;
  while (((!g_constants[i].isNew) || (g_constants[i].isEmpty)) && (i > 0))
    i--;

  if (g_constants[i].isNew)
  {
    g_constants[i].isNew = 0;
    result = g_constants[i].value;
  }
  else
    result = _count(root);

  for (i = 0; i < g_numOfConstants; i++)
  {
    /*if it's a local variable or there was an error and we haven't 
    counted this constant, then we delete it from memory*/
    g_constants[i].isNew = 0;
    if ((!g_constants[i].global) || ((g_constants[i].toCount) && (g_constants[i].isNew)))
    {
      g_constants[i].value = 0;
      free(g_constants[i].name);
      g_constants[i].name = NULL;
      g_constants[i].isEmpty = 1;
    }
  }
  return result;
}

int DestroyTree(leaf_t *root)
{
  if (root->left)
    DestroyTree(root->left);
  if (root->right)
    DestroyTree(root->right);
  free(root);
  root = NULL;
  return 0;
}