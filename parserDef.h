
#include "compNodeDef.h"

#ifndef PARSERDEF_H
#define PARSERDEF_H

struct parseList
{
  struct compNode* compRoot;
  struct parseList* next;
  struct parseLise* subParen;
};

struct tempList
{
  struct charnode* characters;
  struct tempList* next;
};

#endif