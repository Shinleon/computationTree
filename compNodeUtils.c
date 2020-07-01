#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "charnode.h"
#include "compNodeDef.h"

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

charnode *compNodeToCharnode(struct compNode *node)
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
  charnode *ret = compNodeToCharnode(node->left);
  charnode *mid = makeCharnode(operator);
  mid = append(mid, compNodeToCharnode(node->right));
  ret = append(ret, mid);
  return ret;
}

double evalCompNode(struct compNode* node)
{
  if (node)
  {
    
    switch (node->oper)
    {
    case EXP:
      // will fix this
      // https://stackoverflow.com/questions/1375953/how-to-calculate-an-arbitrary-power-root
      return (int)evalCompNode(node->left)  ^ (int)evalCompNode(node->right);
    case MUL:
      return evalCompNode(node->left) * evalCompNode(node->right);
    case QUO:
      return evalCompNode(node->left) / evalCompNode(node->right);
    case SUB:
      return evalCompNode(node->left) - evalCompNode(node->right);
    case ADD:
      return evalCompNode(node->left) + evalCompNode(node->right);
    case VAR:
      // need to make environment aka, dictionary mapping strings to computation
      // will replace soon.
      return 0;
    case NUM:
      return node->d->num;
   }
  }
  exit(-1);
}
char* compNodeToString(struct compNode* node)
{
  charnode* temp = compNodeToCharnode(node);
  char* ret = charnodeToString(temp);
  freeCharnodeList(temp);
  return ret;
}

int main()
{
  union Data *leftD = malloc(sizeof(union Data));
  (*leftD).num = 23 ;
  struct compNode *left = makeCompNode(NUM, NULL, NULL, leftD);

  union Data *rightD = malloc(sizeof(union Data));
  (*rightD).num = 7;
  struct compNode *right = makeCompNode(NUM, NULL, NULL, rightD);

  struct compNode *mid = makeCompNode(QUO, left, right, NULL);

  printf("size of uintptr_t: %ld\n", sizeof(uintptr_t));
  printf("size of double: %ld\n", sizeof(double));
  printf("size of enum: %ld\n", sizeof(NUM));
  printf("size of union Data: %ld\n", sizeof(*rightD));

  //         ++ ---------
  //         00 000000000
  //         10.123456789
  // double x = 29.33333333;
  // mod doesn't work on non integers
  // how to change float to charnode* is mystery so far;
  // printf("%f mod 10: " x, x % 10);


  char* leftStr = compNodeToString(left);
  printf("left node is: %s\n", leftStr);
  free(leftStr);
  char* rightStr = compNodeToString(right);
  printf("right node is: %s\n", rightStr);
  free(rightStr);
  char* midStr = compNodeToString(mid);
  printf("mid node is %s\n", midStr);
  free(midStr);
  printf("mid eval is: %f\n", evalCompNode(mid));

  free(leftD);
  free(left);
  free(rightD);
  free(right);
  free(mid);
}
