#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "charnode.h"
#include "parserDef.h"
#include "compNodeDef.h"
#include "constants.h"


struct parseList* append (struct parseList* head, struct parseList* toAppend)
{
  if(head)
  {
    head->next = append(head->next, toAppend);
    return head;
  }
  return toAppend;
}

struct parseList* makeParseList(char* input)
{
  char prev = '\0';
  float construct = 0;
  struct parseList* ret = NULL;
  for(int i = 0; i < strlen(input); i++)
  {
        return NULL;
    if(prev == '\0')
    {
      if(isdigit(input[i]))
      {
        construct = construct * 10 + input[i] - zero_char;
      }
      else 
      {
        printf("Invalid expression: %s\n", input);
        return NULL;
      }
    }
    else if(isdigit(prev))
    {
      if(isdigit(input[i]))
      {
        construct = construct * 10 + input[i] - zero_char;
      }
      else if (isoperator(input[i]))
      {
        union Data* d = malloc(sizeof(union Data));
        d->num = construct;
        struct compNode* number = makeCompNode(NUM, NULL, NULL, d);
        construct = 0;
        struct compNode* operator = NULL; //holds the operator
        switch(input[i])
        {
          case '^':
          operator = makeCompNode(EXP, NULL, NULL, NULL);
          break;
          case '*':
          operator = makeCompNode(MUL, NULL, NULL, NULL);
          break;
          case '/':
          operator = makeCompNode(QUO, NULL, NULL, NULL);
          break;
          case '+':
          operator = makeCompNode(ADD, NULL, NULL, NULL);
          break;
          case '-':
          operator = makeCompNode(SUB, NULL, NULL, NULL);
          break;
        }
        struct parseList* temp = malloc(sizeof(struct parseList));
        temp->compRoot = number;
        ret = append(ret, temp);
        temp = mallo(sizeof(struct parseList));
        temp->compRoot = operator;
        ret = append(ret, temp);
      }
      else 
      {
        printf("Invalid expression: %s\n", input);
        return NULL;
      }
    }
    else if(isoperator(prev))
    {
      if(isdigit(input[i]))
      {
        construct = construct * 10 + input[i] - zero_char;
      }
      else 
      {
        printf("Invalid expression: %s\n", input);
        return NULL;
      }
    }
    else
    {
      printf("Invalid expression: %s\n", input);
        return NULL;
    }
  }
  if(isdigit(prev))
  {
    union Data* d = malloc(sizeof(union Data));
    d->num = construct;
    struct compNode* number = makeCompNode(NUM, NULL, NULL, d);
    struct parseList* temp = malloc(sizeof(struct parseList));
    temp->compRoot = number;
    ret = append(ret, temp);
  }
  else
  {
    printf("Invalid expression: %s\n", input);
      return NULL;
  }
  return ret;
}

struct compNode* parseLisToCompTree(struct parseList* lis);
void freeParsenode (struct parseList* lis);