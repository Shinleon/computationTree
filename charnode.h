
#ifndef CHARNODE_H
#define CHARNODE_H

typedef struct charnode {
  char data;
  struct charnode* next;
} charnode;

charnode* makeCharnode(char c);
charnode* strToCharlist(char* arg);
charnode* intToCharlist(int num);
void freeCharnodeList(charnode* head);
void freeCharnode(charnode* singlular);
charnode* append(charnode* head, charnode* toAppend);
void printCharnode(charnode* head);
int length(charnode* head);
char* charnodeToString(charnode* head);

/*allows you to scan any quantity of chars :) ;*/
char* wordFromScan();

#endif
