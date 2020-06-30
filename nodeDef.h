#include <stdio.h>
#include <stdlib.h>

#ifndef NODEDEF_H
#define NODEDEF_H

union Data 
{
  char* varName;
  double num;
} Data;

enum operations 
{
  EXP,       // 0
  QUO, MUL,  // 1, 2
  ADD, SUB,  // 3, 4
  VAR, NUM   // 5, 6
};

struct Node 
{
  enum operations oper;
  struct Node* left;
  struct Node* right;
  union Data* d;
};

#endif
