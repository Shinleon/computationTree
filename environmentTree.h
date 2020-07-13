#include "compNodeDef.h"

#ifndef ENVIRONMENT_TREE_H
#define ENVIRONMENT_TREE_H

struct environmentNode *makeEnvironmentNode(char* name, struct compNode *exp);

struct environmentNode* placeEnvironmentNode(
  struct environmentNode* root,
  struct environmentNode* toPlace);

void printEnvironmentNode(struct environmentNode* root);
void freeEnvironmentNode(struct environmentNode* root);

struct environmentNode* getEnvironmentNode(struct environmentNode* root, char* searchKey);

#endif