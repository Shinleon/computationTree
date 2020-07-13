#include <stdio.h>
#include <stdlib.h>
#include <math.h> // have to add '-lm' to the end of your gcc call

#include "charnode.h"
#include "compNodeUtils.h"
#include "environment.h"
#include "environmentTree.h"
#include "wordnode.h"

// appropriate gcc call //
// gcc -std=c11 -Wall -g -o test computer.c compNodeUtils.c charnode.c environmentTree.c wordnode.c -lm

float evalCompNode(struct compNode* node, struct environmentNode* env, struct wordnode* dependencies)
{
  if (node)
  {
    switch (node->oper)
    {
    case EXP:
      // could do original implementation, but also, computing log numerically...
      // https://stackoverflow.com/questions/1375953/how-to-calculate-an-arbitrary-power-root
      return (float)pow((double) evalCompNode(node->left, env, dependencies),
                        (double) evalCompNode(node->right, env, dependencies));
      // return 0;
    case MUL:
      return evalCompNode(node->left, env, dependencies) * evalCompNode(node->right, env, dependencies);
    case QUO:{
        float divisor = evalCompNode(node->right, env, dependencies);
        if(divisor != 0)
        {
          return evalCompNode(node->left, env, dependencies) / divisor;
        }
        printf("Division by zero detected in expression; evaluated to FLT_MAX;\n");
        return __FLT_MAX__;
      }
    case SUB:
      return evalCompNode(node->left, env, dependencies) - evalCompNode(node->right, env, dependencies);
    case ADD:
      return evalCompNode(node->left, env, dependencies) + evalCompNode(node->right, env, dependencies);
    case VAR: 
      {
        // need to make environment aka, envirTree 
        // will replace soon.

        // // STEPS
        // search environment Tree for node's varName // [ADD env as parameter]
        // get it as varEnvNode;
        // return evalCompNode(varEnvNode->expression)
        //#//  printf("looking for %s in env.\n", node->d->varName);
        struct environmentNode* envNode = getEnvironmentNode(env, node->d->varName);
        if(!envNode)
        {
          printf("NOTIFICATION: variable: %s : not declared w/ value.\n", node->d->varName);
          return 0;
        }
        // node->d->varName contains a name
        // envNode->expression contains the compNode 
        // that must be checked for recursive definitions
        //#// printf("WordList before addition of current var; ");
        //#// printWordnode(dependencies);
        struct wordnode* wordfound = inWordnodeList(dependencies, node->d->varName);
        if(wordfound)
        {
          printf("ERROR: recursively defined variable: '%s' : ", node->d->varName);
          printWordnode(dependencies);
          printf("\tSetting it(%s)to zero(0)\n", node->d->varName);
          return 0;
        }
        dependencies = linearInsert(dependencies, makeWordnode(node->d->varName));
        //#// printf("WordList after addition of current var; ");
        float ret = evalCompNode(envNode->expression, env, dependencies);
        
        //have to remove dependency so other branches won't see this branch's dependencies
        dependencies = removeWordNode(dependencies, node->d->varName);

        return ret;
      }
    case NUM:
      return node->d->num;
   }
  }
  exit(-1);
}

/*
int main()
{
  printf("start\n");
  // make expression "2.5 + x"
  union Data* topleftD = malloc(sizeof(union Data));
  topleftD->num = 2.5;
  union Data* toprightD = malloc(sizeof(union Data));
  toprightD->varName = "x";
  struct compNode* top = makeCompNode(
    ADD, 
    makeCompNode(NUM, NULL, NULL, topleftD),
    makeCompNode(VAR, NULL, NULL, toprightD), 
    NULL);

  char* temp = compNodeToString(top);
  printf("made top: %s\n", temp);
  free(temp); 

  // define x as "y - z"
  union Data* yleftD = malloc(sizeof(union Data));
  yleftD->varName = "y";
  union Data* zrightD = malloc(sizeof(union Data));
  zrightD->varName = "z";
  struct compNode* xDef = makeCompNode(
    SUB,
    makeCompNode(VAR, NULL, NULL, yleftD),
    makeCompNode(VAR, NULL, NULL, zrightD),
    NULL
  );
  temp = compNodeToString(xDef);
  printf("made x: %s\n", temp);
  free(temp);

  // define y as "2 * z"
  union Data* yzleftD = malloc(sizeof(union Data));
  yzleftD->num = 2;
  union Data* yzrightD = malloc(sizeof(union Data));
  yzrightD->varName = "z";
  struct compNode* yDef = makeCompNode(
    MUL,
    makeCompNode(NUM, NULL, NULL, yzleftD),
    makeCompNode(VAR, NULL, NULL, yzrightD),
    NULL
  );
  temp = compNodeToString(yDef);
  printf("made y: %s\n", temp );
  free(temp);

  //define z as "4"
  union Data* zData = malloc(sizeof(union Data));
  zData->num = 2.4;
  struct compNode* zDef = makeCompNode(
    NUM,
    NULL,
    NULL,
    zData
  );
  temp = compNodeToString(zDef);
  printf("made z: %s\n", temp);
  free(temp);

  struct environmentNode* env = makeEnvironmentNode("x", xDef);
  env = placeEnvironmentNode(env, makeEnvironmentNode("y", yDef));
  env = placeEnvironmentNode(env, makeEnvironmentNode("z", zDef));

  printEnvironmentNode(env);
  printf("\n");

  float f = evalCompNode(top, env, NULL);
  char* topString = compNodeToString(top);
  printf("\nEvaluation of %s using env above is %.3e.\n", topString, f);
  free(topString);

  freeEnvirontmentNode(env);
  freeCompNode(top);
}
*/