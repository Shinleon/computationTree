#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "charnode.h"
#include "compNodeDef.h"
#include "compNodeUtils.h"

#define zero_char 48

int isoperator(char c);
int parsingrecursion(char* input, int index, struct compNode* root, int depth);
int parenthesisCheck(char* input);
int isValidVarChar(char prevchar);


struct compNode* smallparse(char* input, int* index, char prev, struct compNode* prevOperator);

int main()
{
  int active = 1;
  while(active != 0)
  {
    printf("parser here, gimme an input:: ");
    char* input = wordFromScan();
    active = strcmp(input, "quit");
    printf("This is the string you gave: %d : '%s'\n", active, input);
    printf("This is the paren check: %d :\n", parenthesisCheck(input));

    int prevdigit = 0;
    int prevoperator = 1;
    for(int i = 0; i < strlen(input); i++)
    {
      char c = input[i];
      if(isdigit(c) || c == '.')
      {
        prevoperator = 0;
        prevdigit += 1;
      }
      if(isoperator(c))
      {
        printf("digit := ");
        for(int j = i - prevdigit; j < i; j++)
        {
          printf("%c", input[j]);
        }
        printf("\noperator: %c\n", input[i]);
        prevdigit = 0;
        if(prevoperator)
        {
          printf("invalid character squence: %s\n", input);
          i = strlen(input);
        }
        prevoperator = 1;
      }
    }
    for(int j = strlen(input) - prevdigit; j < strlen(input); j++)
    {
      printf("%c", input[j]);
    }
    printf("\n");
    parsingrecursion(input, 0, NULL, 0);
    int* x = malloc(sizeof(int));
    *x = 0;
    printf("\n\nsmall recursion\n");
    struct compNode* temp = smallparse(input, x, '\0', NULL);
    char* x_ = compNodeToString(temp);
    printf("%s", x_);
    free(input);
  }
}

int isoperator(char c)
{
  return c == '+' || c == '^' || c == '/' || c == '*';
}

int parsingrecursion(char* input, int index, struct compNode* root, int depth)
{
  int i = index;
    int prevdigit = 0;
    int prevoperator = depth == 0;
  for(; i < strlen(input); i++)
  {
    for(int je = 0; je < depth; je++)
    {
      printf("==");
    }

    printf("%s\n", input+i);
    if(input[i] == '(')
    {
      i = parsingrecursion(input, i+1, root, depth+1);
      printf("[%d]", i);
    }
    else if(input[i] == ')')
    {
      return i;
    }
      char c = input[i];
      if(isdigit(c) || c == '.')
      {
        prevoperator = 0;
        prevdigit += 1;
      }
      if(isoperator(c))
      {
        printf("digit := ");
        for(int j = i - prevdigit; j < i; j++)
        {
          printf("%c", input[j]);
        }
        printf("\noperator: %c\n", input[i]);
        prevdigit = 0;
        if(prevoperator)
        {
          printf("invalid character squence: %s\n", input);
          i = strlen(input);
        }
        prevoperator = 1;
      }
    }
    for(int j = strlen(input) - prevdigit; j < strlen(input); j++)
    {
      printf("%c", input[j]);
    }
    printf("\n");
  printf("{%d}", i);
  for(int j = strlen(input) - prevdigit; j < strlen(input); j++)
  {
    printf("%c", input[j]);
  }
  return strlen(input);
}

int parenthesisCheck(char* input)
{
  int ret = 0;
  for(int i = 0; i <strlen(input); i++)
  {
    ret += (input[i] == '(') ? 1 : (input[i] == ')'? -1 : 0);
    if(ret < 0)
    {
      return 0;
    }
  }
  return ret == 0;
}
/*
struct compNode* temporaryFunctionName(char* input, int* index, char prev)
{
  char prevchar = '\0';
  int period = 0;
  struct compNode* ret = NULL;
  for(; *index < strlen(input); (*index += 1))
  {
    if (prevchar == '\0')
    {
      if(input[*index] == '(')
      {
        *index += 1;
        struct compNode* toPlace = temporaryFunctionName(input, index);
        if (!toPlace)
        {
          goto errorMark;
        }

      }
      else if (isdigit(input[*index]))
      {
        union Data* d = malloc(sizeof(union Data));
        d->num = input[*index] - zero_char;
        struct compNode* toPlace = makeCompNode(NUM, NULL, NULL, d);
      }
      else 
      {
        goto errorMark;
      }
    }
    else if (isdigit(prevchar)) // 
    {

    }
    else if (prevchar == '(') // opening paren
    {

    }
    else if (prevchar == ')') // closing paren
    {

    }
    else if (prevchar == '-')
    {

    }
    else if (prevchar == '+')
    {

    }
    else if (prevchar == '*')
    {

    }
    else if (prevchar == '/')
    {

    }
    else if (prevchar == '^')
    {

    }
    else
    {
      goto errorMark;
    }
    prevchar = input[*index];
  }

  if (isdigit(prevchar) && prevchar == '(')
  {
    return ret;
  }

 errorMark:
  return NULL;
}
*/

// default: int* index points to a 0
//          prev = '\0'
//          prevOperator = NULL

/*so you need to look at this operator and
   also the node holding the prevOperator
   for 32+4*5^6/4
   if tree is
     +
    /
   32
   prevOperator should point to the PLUS
   temp should point to {4}
   and tree should be established as 
      +
     / \
    32  4
   however, when the multiplication symbol is seen
   it should look at the pervOperator and evaluate using ((oper)+1)/2
   if it is lower, it should take the position of the right subtree and take
   the current right subtree as it's left subtree
   making the tree
     +
    / \
   32  *
      /
     4
   now the 5 is seen and placed to prevOperator's right subtree,
     +
    / \
   32  *
      / \
     4   5
   the power is seen ; prevOperator is now POw
     +
    / \
   32  *
      / \
     4   ^
        /
       5
   now the 6 is seen ; preOperator is still POW
     +
    / \
   32  *
      / \
     4   ^
        / \
       5   6
*/

struct compNode* smallparse(char* input, int* index, char prev, struct compNode* prevOperator)
{
  float construct = 0;
  for(; *index < strlen(input); (*index)++)
  {
    printf("%c, %c\n", prev, input[*index]);
    if(prev == '\0')
    {
      if(isdigit(input[*index]))
      {
        construct = construct * 10 + input[*index] - zero_char;
      }
      else 
      {
        printf("Invalid expression: %s\n", input);
      }
    }
    else if(isdigit(prev))
    {
      if(isdigit(input[*index]))
      {
        construct = construct * 10 + input[*index] - zero_char;
      }
      else if (isoperator(input[*index]))
      {
        union Data* d = malloc(sizeof(union Data));
        d->num = construct;
        struct compNode* temp = makeCompNode(NUM, NULL, NULL, d);
        construct = 0;
        struct compNode* top = NULL; //holds the operator
        switch(input[*index])
        {
          case '^':
          top = makeCompNode(EXP, NULL, NULL, NULL);
          break;
          case '*':
          top = makeCompNode(MUL, NULL, NULL, NULL);
          break;
          case '/':
          top = makeCompNode(QUO, NULL, NULL, NULL);
          break;
          case '+':
          top = makeCompNode(ADD, NULL, NULL, NULL);
          break;
          case '-':
          top = makeCompNode(SUB, NULL, NULL, NULL);
          break;
        }
        if(!prevOperator) 
        {
          top->left = temp;
          prevOperator = top;
        }
        else if ((prevOperator->oper + 1)/2 > (top->oper + 1)/2)  
        //previous operation has lower precedence or equal
        {
          top->left = temp;
          prevOperator->right = top;
          *index += 1;
          top->right = smallparse(input, index, input[*index-1], top); 
        }
        else
        {
          prevOperator->right = temp;
          *index -= 1;
          return prevOperator;
        }
      }
      else 
      {
        printf("Invalid expression: %s\n", input);
      }
    }
    else if(isoperator(prev))
    {
      if(isdigit(input[*index]))
      {
        construct = construct * 10 + input[*index] - zero_char;
      }
      else 
      {
        printf("Invalid expression: %s\n", input);
      }
    }
    else
    {
      printf("Invalid expression: %s\n", input);
    }
    prev = input[*index];
  }

  if(isdigit(prev))
  {
    union Data* d = malloc(sizeof(union Data));
    d->num = construct;
    struct compNode* temp = makeCompNode(NUM, NULL, NULL, d);
    prevOperator->right = temp;        
  }
  return prevOperator;
}



