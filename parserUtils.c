#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h> // have to add '-lm' to the end of your gcc call

#include "charnode.h"
#include "parserDef.h"
#include "compNodeDef.h"
#include "compNodeUtils.h"
#include "charToCompTranslation.h"
#include "constants.h"

//method that returns 1 if c is alphabetic or '_'
// or returns 1 if c is a digit, but charnode* preRet isn't null


void freeParseList(struct parseList* listed)
{
  if(listed)
  {
    freeParseList(listed->subParen);
    freeParseList(listed->next);
    freeCompNode(listed->compRoot);
    free(listed);
  }
}

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

// need to do proper free's when charToCompTranslation's return NULL;
// returns NULL on error and if *input = NULL;
struct parseList* charnodeToParseList(struct charnode** input, int depth)
{
  if(!input)
  {
    return NULL;
  }
  struct parseList* ret = NULL;
  //struct parseList* attachpoint = NULL; //append takes O(n) time, but have yet
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
        // need to do a full free;
        freeParseList(ret);
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
      if(*tempcomp == NULL) //should only happen if the varName of struct  
                            //was going to hold "quit", "exit", or "env"
      {
        //fully free all objects and return null
        freeParseList(ret);
        free(tempcomp);
        return NULL;
      }
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

      if(*tempcomp == NULL) //should only happen if the varName of struct  
                            //was going to hold "quit", "exit", or "env"
      {
        //fully free all objects and return null
        freeParseList(ret);
        free(tempcomp);
        return NULL;
      }
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
    else if(temp->data == ' ')
    {
      temp = temp->next;
    }
    else 
    {
      //need to full free;
      freeParseList(ret);
      printf("invalid expression syntax; aborting\n");
      return NULL;
    }
  }
  return ret;
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
  enum operatorBool previousStatus = OP_TRUE;
  while(listed)
  {
    if(previousStatus == listed->opBool)
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
    previousStatus = listed->opBool;
    listed = listed->next;
  }
  return 1;
}

// compressParseList_EXP/MUL/ADD need to be refactored

void compressParseList_EXP(struct parseList* listed)
{
  /*
  START
                       listed
                         | possibleoperator
                         |    |   right
                         |    |     |
                         V    V     V
  32 -> '-' -> 4 -> * -> 6 -> ^ -> 0.5 -> ^ -> 5

  x =  ^
     // \ 
     6  0.5
  MID 
                       listed
                         | possibleoperator
                         |    |    right
                         |    |     |
                         V    V     V
  32 -> '-' -> 4 -> * -> x -> ^ -> 0.5 -> ^ -> 5
  
  END
                       listed
                         | possibleoperator
                         |    |   right
                         |    |    |
                         V    V    V
  32 -> '-' -> 4 -> * -> x -> ^ -> 5
  */
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
  if(!listed)
  {
    return NULL;
  }
  compressParseList_PAREN(listed);
  compressParseList_EXP(listed);
  compressParseList_MUL(listed);
  compressParseList_ADD(listed);
  struct compNode* ret = listed->compRoot;
  free(listed);
  return ret;
}


