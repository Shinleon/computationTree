
#include "parserDef.h"
#include "compNodeDef.h"

#ifndef PARSERUTILS_H
#define PARSERUTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h> // have to add '-lm' to the end of your gcc call

#include "charnode.h"
#include "parserDef.h"
#include "compNodeDef.h"
#include "compNodeUtils.h"
#include "charToCompTranslation.h"
#include "constants.h"

//method that returns 1 if c is alphabetic or '_'
// or returns 1 if c is a digit, but charnode* preRet isn't null

void printParseList(struct parseList* listed);

void compressParseListOverOperator(struct parseList* left, struct parseList* operator, struct parseList* right);

struct parseList* makeParseList(enum operatorBool isOperator,
                                struct compNode* compPoint,
                                struct parseList* nextParseList,
                                struct parseList* subp);


struct parseList* appendParseList(struct parseList* head, struct parseList* toAppend);

struct parseList* charnodeToParseList(struct charnode** input, int depth);

void printParseList(struct parseList* listed);

int validateParseList(struct parseList* listed);

// compressParseList_EXP/MUL/ADD need to be refactored

void compressParseList_EXP(struct parseList* listed);
void compressParseList_MUL(struct parseList* listed);

void compressParseList_ADD(struct parseList* listed);
void compressParseListOverOperator(struct parseList* left, struct parseList* operator, struct parseList* right);

void compressParseList_PAREN(struct parseList* listed);

struct compNode* compressParseList(struct parseList* listed);

#endif