
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
struct environmentHashMap
{
    int size;
    struct environmentNode** map;
};

struct environmentNode
{
  char* varName;
  int height;
  struct compNode* expression;
  struct environmentNode* leftenv;
  struct environmentNode* rightenv;
};
#endif

