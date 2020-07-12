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
int validVarChar(char c, struct charnode* preRet);

struct charnode* getNextVarnameAsCompNode(struct compNode** result, struct charnode* curr);

struct charnode* getNextFloatAsCompNode(struct compNode** result, struct charnode* curr);

struct charnode* getOperator(struct compNode** result, struct charnode* curr);

void printParseList(struct parseList* listed);

struct compNode* compressParseList(struct parseList* listed);

void compressParseListOverOperator(struct parseList* left, struct parseList* operator, struct parseList* right);

struct parseList* makeParseList(enum operatorBool isOperator,
                                struct compNode* compPoint,
                                struct parseList* nextParseList,
                                struct parseList* subp)
{
  struct parseList* ret = malloc(sizeof(struct parseList));
  ret->opBool = isOperator;
  ret->compRoot = compPoint;
  ret->next = nextParseList;
  ret->subParen = subp;
  return ret;
}

int isOperator(char c)
{
  return (c == '^') || (c == '/') || (c == '*') || (c == '-') || (c == '+') ;
}

struct parseList* appendParseList(struct parseList* head, struct parseList* toAppend)
{
  // printParseList(toAppend);
  if(head != NULL)
  {
    head->next = appendParseList(head->next, toAppend);
    return head;
  }
  return toAppend;
}

struct parseList* charnodeToParseList(struct charnode** input, int depth)
{
  if(!input)
  {
    return NULL;
  }
  struct parseList* ret = NULL;
  struct parseList* attachpoint = NULL; //append takes n time, but have yet
                                        // to implement a better O(1) time 
                                        // append;
  struct charnode* temp = *input;

  int wasOperator = 1;  //one allows next char to be var or num or paren
  int wasVarNum = 0;    //one allows the next char to be an operator 
                        //  or closing paren
  while(temp)
  {
    //printParseList(ret); printf("\n");
    // printf("wasoperator: %d, wasVarNum: %d\n", wasOperator, wasVarNum);
    // printf("%c\n", temp->data);

    if(temp->data == ')' && wasVarNum)
    {
      // printf("closing paren detected\n");
      if(depth == 0)
      {
        // printf("fautly expression: too many closing paren.\n");
        return NULL;
      }
      *input = temp->next;
      return ret;
    }
    else if (temp->data == '(' && wasOperator)
    {
      // printf("open paren detected\n");
      // printCharnode(temp);
      *input = temp->next;
      struct parseList* tempparse = makeParseList(OP_FALSE, NULL, NULL, charnodeToParseList(input, depth + 1));
      // printf("printing subparen\n");
      // printParseList(tempparse->subParen);
      // printf("\n");
      // printf("finished printing subparen\n");
      if(!tempparse->subParen)
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
      ret = appendParseList(ret, tempparse);
    }
    else if(validVarChar(temp->data, NULL) && wasOperator)
    {
      //do activity for variable name;
      struct parseList* tempparse = NULL;
      struct compNode** tempcomp = malloc(sizeof(struct compnode*));
      temp = getNextVarnameAsCompNode(tempcomp, temp);

      tempparse = makeParseList(OP_FALSE, *tempcomp, NULL, NULL);

      free(tempcomp);
      wasVarNum = 1;
      wasOperator = 0;
      ret = appendParseList(ret, tempparse);
    }
    else if ((isdigit(temp->data) || temp->data == '.') && wasOperator)
    {
      // do activity for float;
      struct parseList* tempparse = NULL;
      struct compNode** tempcomp = malloc(sizeof(struct compnode*));
      temp = getNextFloatAsCompNode(tempcomp, temp);

      tempparse = makeParseList(OP_FALSE, *tempcomp, NULL, NULL);

      free(tempcomp);
      wasVarNum = 1;
      wasOperator = 0;
      ret = appendParseList(ret, tempparse);
    }
    else if(isOperator(temp->data) && wasVarNum)
    {
      //do activity for operator;
      struct parseList* tempparse = NULL;
      struct compNode** tempcomp = malloc(sizeof(struct compnode*));
      temp = getOperator(tempcomp, temp);

      tempparse = makeParseList(OP_TRUE, *tempcomp, NULL, NULL);

      free(tempcomp);
      wasVarNum = 0;
      wasOperator = 1;
      ret = appendParseList(ret, tempparse);
    }
    else
    {
      printf("invalid expression syntax; aborting\n");
      return NULL;
    }
  }
  return ret;
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
    // printf("entered loop\n");
    preRet = append(preRet, makeCharnode(curr->data));
    // printCharnode(preRet);
    curr = curr->next;
  }
  char* temp = charnodeToString(preRet);
  // printf("'%s'\n", temp);
  float f_temp = strtod(temp, NULL);
  union Data* d = malloc(sizeof(union Data));
  d->num = f_temp;
  *result = makeCompNode(NUM, NULL, NULL, d);
  printCharnode(preRet);
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

void printParseList(struct parseList* listed)
{
  if(listed)
  {
    // printf(" %p ", listed->compRoot);
    if(listed->compRoot)
    {
      char* temp = compNodeToString(listed->compRoot);
      printf("'%s' -> ", temp);
      free(temp);
    }
    else if (listed->subParen)
    {
      printf(" ( -> ");
      printParseList(listed->subParen);
      printf("  ) -> ");
    }
  printParseList(listed->next);
  }
  else
  {
    printf("(NIL)");
  }
}

int validateParseList(struct parseList* listed)
{
  enum operatorBool temp = OP_TRUE;
  while(listed)
  {
    if(temp == listed->opBool)
    {
      return 0;
    }
    if(listed->subParen)
    {
      printf("validating subparen\n");
      int parenValid = validateParseList(listed->subParen);
      if(!parenValid)
      {
        return parenValid;
      }
    }
    temp = listed->opBool;
    listed = listed->next;
  }
  return 1;
}

// compressParseList_EXP/MUL/ADD need to be refactored

void compressParseList_EXP(struct parseList* listed)
{
  // START
  //                      listed
  //                        | possibleoperator
  //                        |    |   right
  //                        |    |     |
  //                        V    V     V
  // 32 -> '-' -> 4 -> * -> 6 -> ^ -> 0.5 -> ^ -> 5

  // x =  ^
  //    // \ 
  //    6  0.5
  // MID 
  //                      listed
  //                        | possibleoperator
  //                        |    |    right
  //                        |    |     |
  //                        V    V     V
  // 32 -> '-' -> 4 -> * -> x -> ^ -> 0.5 -> ^ -> 5
  // 
  // END
  //                      listed
  //                        | possibleoperator
  //                        |    |   right
  //                        |    |    |
  //                        V    V    V
  // 32 -> '-' -> 4 -> * -> x -> ^ -> 5
  while(listed)
  {
    struct parseList* possibleOperator = listed->next;
    if(possibleOperator && possibleOperator->opBool == OP_TRUE && (possibleOperator->compRoot->oper+1)/2 == 0)
    {
      struct parseList* right = possibleOperator->next;
      compressParseListOverOperator(listed, possibleOperator, right);
    }
    else
    {
      listed = listed->next;
    }
  }
}

void compressParseList_MUL(struct parseList* listed)
{
  while(listed)
  {
    struct parseList* possibleOperator = listed->next;
    if(possibleOperator && possibleOperator->opBool == OP_TRUE && (possibleOperator->compRoot->oper+1)/2 == 1)
    {
      struct parseList* right = possibleOperator->next;
      compressParseListOverOperator(listed, possibleOperator, right);
    }
    else
    {
      listed = listed->next;
    }
  }
}

void compressParseList_ADD(struct parseList* listed)
{
  while(listed)
  {
    struct parseList* possibleOperator = listed->next;
    if(possibleOperator && possibleOperator->opBool == OP_TRUE && (possibleOperator->compRoot->oper+1)/2 == 2)
    {
      struct parseList* right = possibleOperator->next;
      compressParseListOverOperator(listed, possibleOperator, right);
    }
    else
    {
      listed = listed->next;
    }
  }
}

void compressParseListOverOperator(struct parseList* left, struct parseList* operator, struct parseList* right)
{
  operator->compRoot->left = left->compRoot;
  operator->compRoot->right = right->compRoot;
  left->compRoot = operator->compRoot;
  left->next = right->next;
  free(operator);
  free(right);
}

void compressParseList_PAREN(struct parseList* listed)
{
  while(listed)
  {
    if(listed->subParen)
    {
      listed->compRoot = compressParseList(listed->subParen);
      listed->subParen = NULL;
    }
    listed = listed->next;
  }
}

struct compNode* compressParseList(struct parseList* listed)
{
  compressParseList_PAREN(listed);
  compressParseList_EXP(listed);
  compressParseList_MUL(listed);
  compressParseList_ADD(listed);
  struct compNode* ret = listed->compRoot;
  free(listed);
  return ret;
}

int main()
{
  char* temp = "5^2^2^6-4*3+(9^(3-2))";
  //char** result = malloc(sizeof(char*));
  printf("%s\n", temp);
  struct charnode* number = strToCharlist(temp);
  struct charnode** input = malloc(sizeof(struct charnode*));
  *input = number;
  struct parseList* listed = charnodeToParseList(input, 0);
  free(input);
  //printf("number printing: ");
  //printCharnode(number);
  freeCharnodeList(number);
  printParseList(listed);
  printf("\n");
  compressParseList_PAREN(listed);
  printf("digesting parens: ");
  printParseList(listed);
  printf("\n");
  compressParseList_EXP(listed);
  printf("digesting exponents: ");
  printParseList(listed);
  printf("\n");
  compressParseList_MUL(listed);
  printf("digesting multiplication and div: ");
  printParseList(listed);
  printf("\n");
  compressParseList_ADD(listed);
  printf("digesting addition and subtraction: ");
  printParseList(listed);
  printf("\n");
  printf("validating parselist: %d\n", validateParseList(listed));
  //printParseList(listed); printf("\n");
  freeCompNode(listed->compRoot);
  free(listed);
  //printf("%s\n", *result);
}

