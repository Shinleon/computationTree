
#include "charnode.h"
#include "compNodeDef.h"

#ifndef COMP_NODE_UTILS_H
#define COMP_NODE_UTILS_H
struct compNode* makeCompNode(
    enum operations type,
    struct compNode* l,
    struct compNode* r,
    union Data* d);
char* compNodeToString(struct compNode* node);
void freeCompNode(struct compNode* root);
#endif