/* FINAL: charnode.h
 * Leonard Shin
 * CSE 374
 * SPRING 2020
 */

#ifndef CHARNODE_H
#define CHARNODE_H

typedef struct charnode {
  char data;
  struct charnode* next;
} charnode;

charnode* makeCharnode(char c);
void freeCharnode(charnode* head);
void printCharnode(charnode* head);
int length(charnode* head);
char* toString(charnode* head);

/*allows you to scan any quantity of chars;*/
char* wordFromScan();

#endif
