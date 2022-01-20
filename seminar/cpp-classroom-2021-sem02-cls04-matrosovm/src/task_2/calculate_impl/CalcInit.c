#include <string.h>
#include <stdlib.h>
#include "CalcError.h"
#include "CalcInit.h"

int InitDefCharacters()
{
  int i;
  for (i = 0; i < 26; i++)
    g_defchar[i] = (char)i + 'a';
  g_defchar[26] = '(';
  g_defchar[27] = ')';
  g_defchar[28] = '+';
  g_defchar[29] = '-';
  g_defchar[30] = '/';
  g_defchar[31] = '*';
  g_defchar[32] = '^';
  g_defchar[33] = '.';
  for (i = 34; i < 44; i++)
    g_defchar[i] = (char)i - 34 + '0';
  g_defchar[44] = ' ';
  g_defchar[45] = '_';
  g_defchar[46] = '	'; // tab
  g_defchar[47] = 'E';
  g_defchar[48] = ':';
  g_defchar[49] = '=';
  g_defchar[50] = ';';
  g_defchar[51] = ',';
  return 0;
}

int InitConstants()
{
  int i;

  g_numOfConstants = 2;
  g_constants = (constant_t*)malloc(g_numOfConstants * sizeof(constant_t));
  for (i = 0; i < g_numOfConstants; i++)
  {
    g_constants[i].global = 1;
    g_constants[i].isEmpty = 0;
    g_constants[i].toCount = 0;
    g_constants[i].isNew = OLD_CONSTANT;
    g_constants[i].name = (char*)malloc(3);
  }
  strcpy(g_constants[0].name, "pi");
  g_constants[0].value = pi;
  strcpy(g_constants[1].name, "e");
  g_constants[1].value = euler;
  return 0;
}

/*returns error Code or 0 if there is no error*/
int AddConstant(char *name, int isGlobal)
{
  int n;
  int emptyPlace = -1;
  int len = (int)strlen(name);

  /*let's check if the name of constant is ok*/
  if (!len)
    return ERR_CONST_NAME;
  for (n = 0; n < len; n++)
  {
    if (!IsLetter(name[n]))
      return ERR_CONST_NAME;
  }

  /*let's find out if this constant already exists*/
  for (n = 0; n < g_numOfConstants; n++) 
  { 
    if (g_constants[n].isEmpty)
      emptyPlace = n;
    else if (EqualStrings(name, g_constants[n].name))
    {
      g_constants[n].isNew = RECREATE_CONSTANT;
      break;
    }
  }
  if ((n == 1) || (n == 0))// we can't delete constants pi and e
  {
    g_error = ERR_CONST_NAME;
    return 0;
  }
  else if ((n == g_numOfConstants) && (emptyPlace > 0))
    n = emptyPlace;

  /*it doesn't exist so we create it*/
  if (n == g_numOfConstants)
  {
    g_constants = realloc(g_constants, (n + 1) * sizeof(constant_t));
    g_constants[n].name = (char *)malloc(strlen(name) + 1);
    g_constants[n].value = 0;
    g_numOfConstants++;
    g_constants[n].isNew = CREATE_CONSTANT;
  }
  if (!g_constants[n].name)
  {
    g_constants[n].name = (char*)malloc(len + 1);
    g_constants[n].name[len] = 0;
    g_constants[n].isNew = CREATE_CONSTANT;
  }
  if ((int)strlen(g_constants[n].name) < len)
    g_constants[n].name = realloc(g_constants[n].name, len + 1);

  g_constants[n].global = isGlobal;
  g_constants[n].toCount = 1;
  g_constants[n].isEmpty = 0;

  strcpy(g_constants[n].name, name);
  return 0;
}

int DestroyConstants()
{
  int i; 
  for (i = 0; i < g_numOfConstants; i++)
  {
    if (g_constants[i].name)
      free(g_constants[i].name);
  }
  free(g_constants);
  return 0;
}

int EqualStrings(char const *s1, char const *s2)
{
  int i = 0;

  for (i = 0; i <= (int)strlen(s1); i++)
    if (s1[i] - s2[i])
      return 0;
  return 1;
}

int InitLeaf(leaf_t *cur)
{
  cur->func[0] = 0;
  cur->left = NULL;
  cur->right = NULL;
  cur->num = 0;
  cur->oper = ' ';
  cur->terminate = 0;
  return 0;
}