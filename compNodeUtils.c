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
charnode* toCharNode(struct compNode* node)
{
  printf("node is at: %p\n", node);
  printf("node is of type: %d\n", node->oper);
  if(!node){
    return NULL;
  }
  charnode* ret = NULL;
  charnode* mid = NULL;
  switch(node->oper)
  {
    case EXP:
      ret = toCharNode(node->left);
      charnode* mid = makeCharnode('^');
      mid = append(mid, toCharNode(node->right));
      ret = append(ret, mid);
      return ret;
      break;
    case MUL:
      ret = toCharNode(node->left);
      mid = makeCharnode('*');
      mid = append(mid, toCharNode(node->right));
      ret = append(ret, mid);
      return ret;
      break;
    case QUO:
      ret = toCharNode(node->left);
      mid = makeCharnode('/');
      mid = append(mid, toCharNode(node->right));
      ret = append(ret, mid);
      return ret;
      break;
    case SUB:
      ret = toCharNode(node->left);
      mid = makeCharnode('-');
      mid = append(mid, toCharNode(node->right));
      ret = append(ret, mid);
      return ret;
      break;
    case ADD:
      ret = toCharNode(node->left);
      mid = makeCharnode('+');
      mid = append(mid, toCharNode(node->right));
      ret = append(ret, mid);
      return ret;
      break;
    case VAR:
      return strToCharlist(node->d->varName);
      break;
    case NUM:
      return intToCharlist(node->d->num);
      break;
  }
  return NULL;
}

int main() {
  union Data* leftD = malloc(sizeof(union Data));
  (*leftD).num = 23;
  struct compNode* left = makeCompNode(NUM, NULL, NULL, leftD);

  union Data* rightD = malloc(sizeof(union Data));
  (*rightD).varName = "x";
  struct compNode* right = makeCompNode(VAR, NULL, NULL, rightD);

  struct compNode* mid = makeCompNode(MUL, left, right, NULL);

  printf("size of uintptr_t: %ld\n", sizeof(uintptr_t));
  printf("size of double: %ld\n", sizeof(double));
  printf("left node is: %s\n", toString( toCharNode(left)));
  printf("right node is: %s\n", toString( toCharNode(right)));
  printf("mid node is %s\n", toString( toCharNode(mid)));

  free(leftD);
  free(left);
}
