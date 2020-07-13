
#ifndef CHARCOMPTRANSLATION_H
#define CHARCOMPTRANSLATION_H


int validVarChar(char c, struct charnode* preRet);

int isOperator(char c);

struct charnode* getOperator(struct compNode** result, struct charnode* curr);

struct charnode* getNextVarnameAsCompNode(struct compNode** result, struct charnode* curr);

struct charnode* getNextFloatAsCompNode(struct compNode** result, struct charnode* curr);

#endif