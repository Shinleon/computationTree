#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "nodeDef.h"

struct Node* makeNode(
	       enum operations type, 
	       struct Node* l, 
	       struct Node* r,
	       union Data* d)
{
  struct Node* ret = malloc(sizeof(struct Node));
  ret->oper = type;
  ret->left = l;
  ret->right = r;
  ret->d = d;
  return ret;
}

void printNode(struct Node* n)
{
  switch(n->oper)
  {
    case EXP:
      printf("EXP(");
    case MUL:
      printf("MUL(");
    case QUO:
      printf("EXP                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   ccfffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff(");
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
  struct Node* left = makeNode(NUM, NULL, NULL, leftD);
  free(leftD);
  free(left);
  printf("size of uintptr_t: %ld\n", sizeof(uintptr_t));
  printf("size of double: %ld\n", sizeof(double));
}
