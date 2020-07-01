#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "charnode.h"
#include "compNodeDef.h"

charnode *mergeOnOperator(struct compNode *node, char operator);

struct compNode *makeCompNode(
    enum operations type,
    struct compNode *l,
    struct compNode *r,
    union Data *d)
{
  struct compNode *ret = malloc(sizeof(struct compNode));
  ret->oper = type;
  ret->left = l;
  ret->right = r;
  ret->d = d;
  return ret;
}
charnode *toCharnode(struct compNode *node)
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
    return intToCharlist(node->d->num);
  }
  return NULL;
}

charnode *mergeOnOperator(struct compNode *node, char operator)
{
  charnode *ret = toCharnode(node->left);
  charnode *mid = makeCharnode(operator);
  mid = append(mid, toCharnode(node->right));
  ret = append(ret, mid);
  return ret;
}

int main()
{
  union Data *leftD = malloc(sizeof(union Data));
  (*leftD).num = 23;
  struct compNode *left = makeCompNode(NUM, NULL, NULL, leftD);

  union Data *rightD = malloc(sizeof(union Data));
  (*rightD).varName = "x";
  struct compNode *right = makeCompNode(VAR, NULL, NULL, rightD);

  struct compNode *mid = makeCompNode(QUO, left, right, NULL);

  printf("size of uintptr_t: %ld\n", sizeof(uintptr_t));
  printf("size of double: %ld\n", sizeof(double));
  printf("left node is: %s\n", charnodeToString(toCharnode(left)));
  printf("right node is: %s\n", charnodeToString(toCharnode(right)));
  printf("mid node is %s\n", charnodeToString(toCharnode(mid)));

  free(leftD);
  free(left);
}
