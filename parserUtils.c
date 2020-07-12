#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h> // have to add '-lm' to the end of your gcc call

#include "charnode.h"
#include "parserDef.h"
#include "compNodeDef.h"
#include "compNodeUtils.h"
#include "constants.h"

// struct parseList* makeParseList(char* input);
// struct compNode* parseLisToCompTree(struct parseList* lis);
// void freeParsenode (struct parseList* lis);

//method that returns 1 if c is alphabetic or '_'
// or returns 1 if c is a digit, but charnode* preRet isn't null

struct parseList* makeParseList(struct charnode** input, int depth)
{
  if(!input)
  {
    return NULL;
  }
  struct parseList* ret = NULL;
  struct charnode* temp = *input;

  int wasOperator = 1;  //one allows next char to be var or num or paren
  int wasVarNum = 0;    //one allows the next char to be an operator 
                        //  or closing paren
  while(temp)
  {
    if(temp->data == ')' && wasVarNum)
    {
      if(depth == 0)
      {
        printf("fautly expression: too many closing paren.\n");
        return NULL;
      }
      *input = temp->next;
      return ret;
    }
    else if (temp->data == '(' && wasOperator)
    {
      struct parseList* temp = malloc(sizeof(struct parseList));
      *input = temp->next;
      temp->subParen = makeParseList(input, depth + 1);
      if(!temp->subParen)
      {
        // set both flags to false so program aborts as subparen was
        //   invalid
        wasVarNum = 0;
        wasOperator = 0;
      }
      //input should now point to charnode after matching closing paren;
      temp = *input;
      wasOperator = 0;
      wasVarNum = 1;
    }
    else if(validVarChar(temp->data, NULL) && wasOperator)
    {
      //do activity for variable name;
      wasVarNum = 1;
      wasOperator = 0;
    }
    else if ((isdigit(temp->data) || temp->data == '.') && wasOperator)
    {
      // do activity for float;
      wasVarNum = 1;
      wasOperator = 0;
    }
    else if(isOperator(temp->data) && wasVarNum)
    {
      //do activity for operator;
      wasVarNum = 0;
      wasOperator = 1;
    }
    else
    {
      printf("invalid expression syntax; aborting\n");
      return NULL;
    }
  }
  temp = temp->next;
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

struct charnode* getNextFloatAsCompNode(struct compNode** result, struct charnode* curr)
{
  struct charnode* preRet = NULL;
  while(curr && (isdigit(curr->data) || curr->data == '.'))
  {
    printf("entered loop\n");
    preRet = append(preRet, makeCharnode(curr->data));
    printCharnode(preRet);
    curr = curr->next;
  }
  char* temp = charnodeToString(preRet);
  printf("'%s'\n", temp);
  float f_temp = strtod(temp, NULL);
  union Data* d = malloc(sizeof(union Data));
  d->num = f_temp;
  *result = makeCompNode(NUM, NULL, NULL, d);
  freeCharnodeList(preRet);
  free(temp);
  return curr;
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
  union Data* d = malloc(sizeof(union Data));
  d->varName = varName;
  *result = makeCompNode(VAR, NULL, NULL, d);
  freeCharnodeList(preRet);
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

int main()
{
  char* temp = "var/2";
  char** result = malloc(sizeof(char*));
  charnode* number = strToCharlist(temp);
  getNextVarname(result, number);
  printf("%s\n", *result);
}