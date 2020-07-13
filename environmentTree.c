#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compNodeDef.h"
#include "compNodeUtils.h"
#include "environment.h"

#define MAX(A, B) ((A)> (B) ? (A) : (B))

struct environmentNode *makeEnvironmentNode(char *name, struct compNode *exp)
{
  struct environmentNode *ret = malloc(sizeof(struct environmentNode));
  ret->varName = name;
  ret->height = 0;
  ret->expression = exp;
  ret->leftenv = NULL;
  ret->rightenv = NULL;
  return ret;
}

struct environmentNode* placeEnvironmentNode(
  struct environmentNode* root,
  struct environmentNode* toPlace)
{
  if(root)
  {
    int x = strcmp(toPlace->varName, root->varName);
    if(x < 0)
    {
      root->leftenv = placeEnvironmentNode(root->leftenv, toPlace);
    }
    else if (x > 0)
    {
      root->rightenv = placeEnvironmentNode(root->rightenv, toPlace);
    }
    else
    {
      // this needs to be changed to allow variable redefinition;
      printf("Duplicate variable name in environment: %s\n", root->varName);
      printf("Variable not modified; retaining previous definition.");
    }
    int leftheight = root->leftenv ? root->leftenv->height : -1;
    int rightheight = root->rightenv ? root->rightenv->height : -1;
    root->height = 1 + MAX(leftheight, rightheight);

    // Want to make sure the tree is balanced.
    // AVL operations go here, to balance tree, using leftheight and rightheight

    return root;
  }
  return toPlace;
}

void printEnvironmentNode(struct environmentNode* root)
{
  if(root)
  {
    char* rootCompStr = compNodeToString(root->expression);
    printf("Name: %s, compTree = '%s' Left { ", root->varName, rootCompStr);
    free(rootCompStr);
    printEnvironmentNode(root->leftenv);
    printf(" }, Right { ");
    printEnvironmentNode(root->rightenv);
    printf(" }");
  }
  else
  {
    printf("(NIL)");
  }
}

void displayEnvironment(struct environmentNode* root)
{
  if(root)
  {
    char* rootCompStr = compNodeToString(root->expression);
    printf("Name: %s, compTree = '%s'\n", root->varName, rootCompStr);
    free(rootCompStr);
    displayEnvironment(root->leftenv);
    displayEnvironment(root->rightenv);
  }
}

struct environmentNode* getEnvironmentNode(struct environmentNode* root, char* searchKey)
{
  if(root)
  {
    int x = strcmp(searchKey, root->varName);
    if(x < 0)
    {
      return getEnvironmentNode(root->leftenv, searchKey);
    }
    else if (x > 0)
    {
      return getEnvironmentNode(root->rightenv, searchKey);
    }
    else 
    {
      return root;
    }
  }
  return NULL;
}

void freeEnvironmentNode(struct environmentNode* root)
{
  if(root)
  {
    freeEnvironmentNode(root->leftenv);
    freeEnvironmentNode(root->rightenv);
    freeCompNode(root->expression);
    free(root->varName);
    free(root);
  }
}