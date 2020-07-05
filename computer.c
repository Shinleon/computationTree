#include <stdio.h>
#include <stdlib.h>
#include <math.h> // have to add '-lm' to the end of your gcc call

#include "charnode.h"
#include "compNodeUtils.h"
#include "environment.h"
#include "environmentTree.h"

// appropriate gcc call //
// gcc -std=c11 -Wall -g -o test computer.c compNodeUtils.c charnode.c environmentTree.c -lm

float evalCompNode(struct compNode* node, struct environmentNode* env)
{
  if (node)
  {
    switch (node->oper)
    {
    case EXP:
      // could do original implementation, but also, computing log numerically...
      // https://stackoverflow.com/questions/1375953/how-to-calculate-an-arbitrary-power-root
      return (float)pow((double) evalCompNode(node->left, env),
                        (double) evalCompNode(node->right, env));
      // return 0;
    case MUL:
      return evalCompNode(node->left, env) * evalCompNode(node->right, env);
    case QUO:
      return evalCompNode(node->left, env) / evalCompNode(node->right, env);
    case SUB:
      return evalCompNode(node->left, env) - evalCompNode(node->right, env);
    case ADD:
      return evalCompNode(node->left, env) + evalCompNode(node->right, env);
    case VAR: 
      {
        // need to make environment aka, envirTree 
        // will replace soon.

        // // STEPS
        // search environment Tree for node's varName // [ADD env as parameter]
        // get it as varEnvNode;
        // return evalCompNode(varEnvNode->expression)
        struct environmentNode* envNode = getEnvironmentNode(env, node->d->varName);
        if(envNode)
        {
          return evalCompNode(envNode->expression, env);
        }
        printf("NOTIFICATION: varName: %s : not declared w/ value.\n", node->d->varName);
        return 0;
      }
    case NUM:
      return node->d->num;
   }
  }
  exit(-1);
}

int main()
{
  union Data *leftD = malloc(sizeof(union Data));
  (*leftD).num = 4.25;
  struct compNode *left = makeCompNode(NUM, NULL, NULL, leftD);

  union Data *rightD = malloc(sizeof(union Data));
  (*rightD).num = 2;
  struct compNode *right = makeCompNode(NUM, NULL, NULL, rightD);

  struct compNode *mid = makeCompNode(EXP, left, right, NULL);
  
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
  printf("mid eval is: %.3e\n", evalCompNode(mid, NULL));

  union Data* topData = malloc(sizeof(union Data));
  topData->varName = "abc";
  struct compNode* top_r = makeCompNode(VAR, NULL, NULL, topData);

  struct compNode* top = makeCompNode(SUB, mid, top_r, NULL); 

  // EnvironmentNode tests. 
  struct environmentNode* temp = makeEnvironmentNode("hello", NULL);
  temp = placeEnvironmentNode(NULL, temp);
  struct environmentNode* temp2 = makeEnvironmentNode("oh no", NULL);
  temp = placeEnvironmentNode(temp, temp2);
  union Data* abcData = malloc(sizeof(union Data));
  abcData->num = 12.3; 
  temp = placeEnvironmentNode(temp, makeEnvironmentNode("abc", makeCompNode(NUM, NULL, NULL, abcData)));
  printEnvironmentNode(temp);
  
  printf("\n");

  char* topNodeStr = compNodeToString(top);
  printf("top node: %s\n", topNodeStr);
  free(topNodeStr);
  printf("top eval is: %.3e\n", evalCompNode(top, temp));

  // Not sure I care about freeing variables during testing
  // free(leftD);
  // free(left);
  // free(rightD);
  // free(right);
  // free(mid);
}
