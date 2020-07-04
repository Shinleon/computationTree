#include <stdio.h>
#include <stdlib.h>

#include "charnode.h"
#include "compNodeDef.h"
#include "compNodeUtils.h"

float evalCompNode(struct compNode* node)
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


int main()
{
  union Data *leftD = malloc(sizeof(union Data));
  (*leftD).num = 4.5*.0000001 ;
  struct compNode *left = makeCompNode(NUM, NULL, NULL, leftD);

  union Data *rightD = malloc(sizeof(union Data));
  (*rightD).num = 2;
  struct compNode *right = makeCompNode(NUM, NULL, NULL, rightD);

  struct compNode *mid = makeCompNode(QUO, left, right, NULL);
  
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
  printf("mid eval is: %.3e\n", evalCompNode(mid));

  free(leftD);
  free(left);
  free(rightD);
  free(right);
  free(mid);
}
