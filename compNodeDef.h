#ifndef COMPNODEDEF_H
#define COMPNODEDEF_H

#define RAW_TO_PEMDAS(X) (((X)+1)/2)

union Data 
{
  char* varName;
  float num;
};

enum operations 
{
                      // raw   // ((raw) + 1) / 2 
  FUNC = 0,           // 0     // 0  // highest precedence
  EXP = 2,            // 2     // 1 
  QUO = 3,  MUL = 4,  // 3, 4  // 2
  ADD = 5,  SUB = 6,  // 5, 6  // 3 
  VAR = 7,  NUM = 8   // 7, 8  // 4  // lowest precedence
};

struct compNode 
{
  enum operations oper;
  struct compNode* left;
  struct compNode* right;
  union Data* d;
};

#endif
