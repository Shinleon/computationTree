
#include "compNodeDef.h"
#include "environment.h"
#include "wordnode.h"

float evalCompNode(struct compNode* node, struct environmentNode* env, struct wordnode* dependencies, int* errorCode);