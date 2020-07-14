#ifndef NODEDEF_H
#define NODEDEF_H

union Data 
{
  char* varName;
  float num;
};

enum operations 
{
             // raw   // ((raw) + 1) / 2
  // SIN, COS
  EXP,       // 0     // 0 
  QUO, MUL,  // 1, 2  // 1
  ADD, SUB,  // 3, 4  // 2
  VAR, NUM   // 5, 6  // 3
};

struct compNode 
{
  enum operations oper;
  struct compNode* left;
  struct compNode* right;
  union Data* d;
};

#endif
