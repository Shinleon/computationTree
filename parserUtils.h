
#include "parserDef.h"
#include "compNodeDef.h"

#ifndef PARSERUTILS_H
#define PARSERUTILS_H

struct parseList* makeParseList(char* input);
struct compNode* parseLisToCompTree(struct parseList* lis);
void freeParsenode (struct parseList* lis);
#endif