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

struct charnode* mergeOnOperator(struct compNode* node, struct compNode* parent, char operator);

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

struct charnode *compNodeToCharnode(struct compNode* node, struct compNode* parent)
{
  if (!node)
  {
    return NULL;
  }
  switch (node->oper)
  {
  case EXP:
    return mergeOnOperator(node, parent, '^');
  case MUL:
    return mergeOnOperator(node, parent, '*');
  case QUO:
    return mergeOnOperator(node, parent, '/');
  case SUB:
    return mergeOnOperator(node, parent, '-');
  case ADD:
    return mergeOnOperator(node, parent, '+');
  case VAR:
    return strToCharlist(node->d->varName);
  case NUM:
    return floatToCharlist(node->d->num);
  }
  return NULL;
}

// 
struct charnode* mergeOnOperator(struct compNode* node, struct compNode* parent, char operator)
{
  struct charnode* ret = compNodeToCharnode(node->left, node);
  struct charnode* mid = makeCharnode(operator);
  struct charnode* aft = compNodeToCharnode(node->right, node);
  if(parent && (RAW_TO_PEMDAS(parent->oper) < RAW_TO_PEMDAS(node->oper)))
  {
    aft = append(aft, makeCharnode(')')); // aft + )
    ret = append(makeCharnode('('), ret); // ( + ret 
  }
  mid = append(mid, aft); // mid + aft + )
  ret = append(ret, mid); // ( + ret + mid  + aft + )
  return ret;
}


char* compNodeToString(struct compNode* node)
{
  struct charnode* temp = compNodeToCharnode(node, NULL);
  char* ret = charnodeToString(temp);
  freeCharnodeList(temp);
  return ret;
}

void freeData(union Data* d, enum operations oper)
{
  if (d)
  {
    if(oper == VAR)
    {
      free(d->varName);
    }
  free(d);
  }
}

void freeCompNode(struct compNode* root)
{
  if (root)
  {
    freeCompNode(root->left);
    freeCompNode(root->right);
    freeData(root->d, root->oper);
    free(root);
  }
}