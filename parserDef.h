
#include "compNodeDef.h"

#ifndef PARSERDEF_H
#define PARSERDEF_H

enum operatorBool
{
  OP_TRUE,
  OP_FALSE
};

struct parseList
{
  enum operatorBool opBool;
  struct compNode* compRoot;
  struct parseList* next;
  struct parseList* subParen;
};

#endif