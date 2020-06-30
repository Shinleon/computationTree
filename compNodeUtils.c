#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "charnode.h"
#include "compNodeDef.h"

struct compNode* makeCompNode(
	       enum operations type, 
	       struct compNode* l, 
	       struct compNode* r,
	       union Data* d)
{
  struct compNode* ret = malloc(sizeof(struct compNode));
  ret->oper = type;
  ret->left = l;
  ret->right = r;
  ret->d = d;
  return ret;
}

void toCharNode(struct compNode* n)
{
  charnode* ret = NULL;
  switch(n->oper)
  {
    case EXP:
      printf("EXP(");
    case MUL:
      printf("MUL(");
    case QUO:
      printf("EXP(");
    case SUB:
      printf("EXP(");
    case ADD:
      printf("EXP(");
    case VAR:
      printf("EXP(");
    case NUM:
      printf("EXP(");

  }
}

int main() {
  union Data* leftD = malloc(sizeof(union Data));
  (*leftD).num = 8.0;
  struct compNode* left = makeCompNode(NUM, NULL, NULL, leftD);
  free(leftD);
  free(left);
  printf("size of uintptr_t: %ld\n", sizeof(uintptr_t));
  printf("size of double: %ld\n", sizeof(double));
}
