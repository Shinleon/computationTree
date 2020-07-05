#include <stdio.h>
#include <stdlib.h>
//#include <stdint.h>

#include "charnode.h"
#include "compNodeDef.h"

// compiler design and symbolic computing stuff
// might be worth looking into...
// https://www.youtube.com/playlist?list=PLEbnTDJUr_IcPtUXFy2b1sGRPsLFMghhS

// also, for algorithms,
// this guy makes himself a god
// https://www.youtube.com/channel/UCx-kFfzekMbhODaBss-ZnsA

// also, CSE 331 for summer 2020
// [watch on 2x speed, he talks slow]
// https://www.youtube.com/watch?v=hX-hF1sDR1A

// link on encapsulating in c
// not sure if I can implement this
// https://stackoverflow.com/a/29461983  

char* compNodeToString(struct compNode* node);

charnode *mergeOnOperator(struct compNode *node, char operator);

struct compNode *makeCompNode(
    enum operations type,
    struct compNode* l,
    struct compNode* r,
    union Data* d)
{
  struct compNode *ret = malloc(sizeof(struct compNode));
  ret->oper = type;
  ret->left = l;
  ret->right = r;
  ret->d = d;
  return ret;
}

charnode *compNodeToCharnode(struct compNode* node)
{
  if (!node)
  {
    return NULL;
  }
  switch (node->oper)
  {
  case EXP:
    return mergeOnOperator(node, '^');
  case MUL:
    return mergeOnOperator(node, '*');
  case QUO:
    return mergeOnOperator(node, '/');
  case SUB:
    return mergeOnOperator(node, '-');
  case ADD:
    return mergeOnOperator(node, '+');
  case VAR:
    return strToCharlist(node->d->varName);
  case NUM:
    return floatToCharlist(node->d->num);
    //return intToCharlist((int)node->d->num);
  }
  return NULL;
}

charnode *mergeOnOperator(struct compNode *node, char operator)
{
  charnode *ret = compNodeToCharnode(node->left);
  charnode *mid = makeCharnode(operator);
  charnode *aft = compNodeToCharnode(node->right);
  aft = append(aft, makeCharnode(')'));
  mid = append(mid, aft);
  ret = append(ret, mid);
  ret = append(makeCharnode('('), ret);
  return ret;
}


char* compNodeToString(struct compNode* node)
{
  charnode* temp = compNodeToCharnode(node);
  char* ret = charnodeToString(temp);
  freeCharnodeList(temp);
  return ret;
}
