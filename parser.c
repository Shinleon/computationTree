#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "charnode.h"
#include "compNodeDef.h"
#include "compNodeUtils.h"

int isoperator(char c);
int parsingrecursion(char* input, int index, struct compNode* root, int depth);


int main()
{
  int active = 1;
  while(active != 0)
  {
    printf("parser here, gimme an input:: ");
    char* input = wordFromScan();
    active = strcmp(input, "quit");
    printf("This is the string you gave: %d : '%s'\n", active, input);

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
  }
  printf("{%d}", i);
  return strlen(input);
}