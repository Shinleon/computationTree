
#include "compNodeDef.h"

#ifndef PARSERDEF_H
#define PARSERDEF_H

struct parseList
{
  struct compNode* compRoot;
  struct parseList* next;
  struct parseLise* end;
};

#endif