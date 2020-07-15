#include <stdlib.h>
#include <stdio.h>

#include "parserUtils.h"
#include "compNodeUtils.h"
#include "charnode.h"
#include "environment.h"
#include "environmentTree.h"
#include "computer.h"

void splitOnEqual(char* original, char** left, char** right);

int verify(char* name);

void intro()
{
  printf("Hello! Welcome to a small computing unit.\n");
  printf("This computing program was made by Leonard Shin.\n");
  printf("It can hold variables using input format of \n<variable name>");
  printf("=<expression>. An expression can be \"3+4\" or anything that makes\n");
  printf("mathematical sense in that it has operators, parenthesis, constants, and variables\n");
  printf("An expression such as \"(3+(2-4)^2 + 5\" will be interpreted as \"(3+(2-4)^2 + 5)\"\n");
  printf("Any missing closing parethesis will be automatically added to the end while extra closing\n");
  printf("parenthesis will cause an error.\n");
  printf("This is version 1.0\n");
  printf("Type \"quit\" or \"exit\" to quit. [Ctrl+d/EOF will triggers an infinite loop when\n");
  printf("used alone on the input promt, and is otherwise just ignored].\n");
  printf("Also, please never type CTRL+D, or at least have CTRL+C ready.\n Thanks for using this!\n");

}

int main()
{
  intro();
  struct environmentNode* env = NULL;

  int exit = 0;
  while(!exit)
  { 
    printf("in  <<<:  ");
    char* command = wordFromScan(); // should be trimmed already...
    // printf("\"%s\"\n", command);
    if(strcmp("exit", command) == 0 || strcmp("quit", command) == 0)
    {
      exit = 1;
    }
    else if (strcmp("env", command) == 0 || strcmp("", command) == 0)
    {
      printf("\n");
      if(env == NULL)
      {
        printf("Empty Environment.\n");
      }
      displayEnvironment(env);
      printf("\n");
    }
    else
    {
      // printf("%s\n", command);
      char** varN = malloc(sizeof(char*));
      char** expPtr = malloc(sizeof(char*));

      splitOnEqual(command, varN, expPtr);

      struct charnode* tempchar = strToCharlist(*expPtr);
      // printf("%s\n", *expPtr);
      free(*expPtr);
      free(expPtr);
      struct charnode** input = malloc(sizeof(struct charnode*));
      *input = tempchar;

      struct parseList* tempParse = charnodeToParseList(input, 0);
      // printCharnode(tempchar);
      free(input);
      freeCharnodeList(tempchar);
      if(tempParse)
      {
        struct compNode* compRoot = compressParseList(tempParse);
        /*
        char* __toPrint = compNodeToString(compRoot);
        printf("%s\n", __toPrint);
        free(__toPrint);
        //*/
        if(*varN)
        {
          int valid = verify( *varN );
          if(valid)
          {
            struct environmentNode* toAdd = makeEnvironmentNode(*varN, compRoot);
            env = placeEnvironmentNode(env, toAdd);
            printf("\n");
            displayEnvironment(env);
            printf("\n");
          }
          else
          {
            printf("\"%s\" isn't a valid variable name.\n", *varN);
            free(*varN);
            freeCompNode(compRoot);
          }
        }
        else
        {
          int* errorCatch = malloc(sizeof(int*));
          *errorCatch = 0;
          float f = evalCompNode(compRoot, env, NULL, errorCatch);
          if(*errorCatch == 0)
          {
            printf("out :>>>  %e\n", f);
          }
          free(errorCatch);
          freeCompNode(compRoot);
        }
      }
      else
      {
        
      }
      free(varN);
    }
    free(command);
  }
  //full free of environment
  freeEnvironmentNode(env);
}

// splits original on the first equal sign '='
// copies lef of equal to left and copies right of equal to right;
void splitOnEqual(char* original, char** left, char** right)
{
  struct charnode* leftTemp = NULL;

  struct charnode* rightTemp = NULL;
  struct charnode* rightAddPoint = NULL;

  int equal = 0; //tracks if we've seen an equal sign yet
  struct charnode* lastnonWhitespace = NULL;
  for(int i = 0; i < (int) strlen(original); i++)
  {
    if(original[i] == '=' && !equal)
    {
      equal = 1;
      // printCharnode(rightTemp);
      freeCharnodeList(lastnonWhitespace->next);
      lastnonWhitespace->next = NULL;

      leftTemp = rightTemp;
      rightTemp = NULL;
      rightAddPoint = NULL;
    }
    else
    {
      if(rightTemp)
      {
        rightAddPoint->next = makeCharnode(original[i]);
        rightAddPoint = rightAddPoint->next;
      }
      else
      {
        rightTemp = makeCharnode(original[i]);
        rightAddPoint = rightTemp;
      }
      
      if(original[i] != ' ')
      {
        lastnonWhitespace = rightAddPoint;
      }
    }
  }
  // here we need t store charnodetostring(rightTemp) in *right;
  if(rightTemp)
  {
    char* rightString = charnodeToString(rightTemp);
    *right = rightString;
  }
  else
  {
    *right = NULL;
  }
  if(leftTemp)
  {
    char* leftString = charnodeToString(leftTemp);
    // printf("leftString: \"%s\"\n", leftString);
    *left = leftString;
  }
  else
  {
    *left = NULL;
  }
  //now free the charnodes;
  freeCharnodeList(rightTemp);
  freeCharnodeList(leftTemp);
}

int verify(char* name)
{
  if(strcmp("exit", name) == 0 || strcmp("quit", name) == 0 || strcmp("env", name) == 0)
  {
    return 0;
  }
  else 
  {
    struct charnode* trigger = NULL;
    for(int i = 0; i < (int) strlen(name); i++)
    {
      if(i == 1)
      {
        trigger = makeCharnode('\t');
      }
      int ret = validVarChar(name[i], trigger);
      if(ret == 0)
      {
        free(trigger);
        return 0;
      }
    }
    free(trigger);
  }
  return 1;
}

