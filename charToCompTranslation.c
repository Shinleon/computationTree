#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "compNodeDef.h"
#include "compNodeUtils.h"
#include "charnode.h"
#include "constants.h"

int reservedVarName(char* possibleVar)
{
  // printf("In reserved var: \"%s\" \n", possibleVar);
  return  (strcmp(possibleVar,"env")  == 0 ) ||
          (strcmp(possibleVar,"quit")  == 0 ) ||
          (strcmp(possibleVar,"exit")  == 0 );
}

int validVarChar(char c, struct charnode* preRet)
{
  int ret = 0;
  //check between 'A' and 'Z', check between 'a' and 'z', check for '_'
  if (( c > 64 && c < 91 ) || ( c > 96 && c < 123 ) || c == '_')
  {
    ret = 1;
  }
  //if varchar has something in it, it can include a digit, it cannot
  // start with a digit, though.
  if(preRet && c >= zero_char && c <= zero_char + 9)
  {
    ret = 1;
  }
  return ret;
}

int isOperator(char c)
{
  return (c == '^') || (c == '/') || (c == '*') || (c == '-') || (c == '+') ;
}

struct charnode* getNextVarnameAsCompNode(struct compNode** result, struct charnode* curr)
{
  struct charnode* preRet = NULL;
  while(curr && validVarChar(curr->data, preRet))
  {
    preRet = append(preRet, makeCharnode(curr->data));
    curr = curr->next;
  }
  char* varName = charnodeToString(preRet);
  if(reservedVarName(varName))
  {
    printf("Cannot use reserved variable name, %s\n", varName);
    *result = NULL;
    freeCharnodeList(preRet);
    free(varName);
    return NULL;
  }
  union Data* d = malloc(sizeof(union Data));
  d->varName = varName;
  *result = makeCompNode(VAR, NULL, NULL, d);
  freeCharnodeList(preRet);
  return curr;
}

//returns NULL and makes *result = NULL if the float isn't properly formated
struct charnode* getNextFloatAsCompNode(struct compNode** result, struct charnode* curr)
{
  struct charnode* preRet = NULL;
  struct charnode* attachPoint = NULL; // should change appending to O(1) time instead of O(n)
  int period = 0;
  while(curr && (isdigit(curr->data) || curr->data == '.'))
  {
    if(curr->data == '.')
    {
      if(period)
      {
        printf("ERROR, too many periods for this float;\n");
        freeCharnodeList(preRet);
        *result = NULL;
        return NULL;
      }
      period = 1;
    }
    // printf("entered loop\n");
    if(attachPoint)
    {
      append(attachPoint, makeCharnode(curr->data));
      attachPoint = attachPoint->next;
    }
    else
    {
      preRet = append(preRet, makeCharnode(curr->data));
      attachPoint = preRet;
    }
    // printCharnode(preRet);
    curr = curr->next;
  }
  char* temp = charnodeToString(preRet); // allocating memory for a string
  // printf("'%s'\n", temp);
  float f_temp = strtod(temp, NULL);
  union Data* d = malloc(sizeof(union Data));
  d->num = f_temp;
  *result = makeCompNode(NUM, NULL, NULL, d);
  // printCharnode(preRet);
  freeCharnodeList(preRet);
  free(temp); //freeing the string that was used to make the float->f_temp
  return curr;
}

struct charnode* getOperator(struct compNode** result, struct charnode* curr)
{
  if(curr->data ==  '^')
  {
    *result = makeCompNode(EXP, NULL, NULL, NULL);
  }
  else if(curr->data ==  '*')
  {
    *result = makeCompNode(MUL, NULL, NULL, NULL);
  }
  else if(curr->data ==  '/')
  {
    *result = makeCompNode(QUO, NULL, NULL, NULL);
  }
  else if(curr->data ==  '+')
  {
    *result = makeCompNode(ADD, NULL, NULL, NULL);
  }
  else if(curr->data ==  '-')
  {
    *result = makeCompNode(SUB, NULL, NULL, NULL);
  }
  else 
  {
    *result = NULL;
  }
  return curr->next;
}