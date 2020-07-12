
#ifndef CHARNODE_H
#define CHARNODE_H

typedef struct charnode {
  char data;
  struct charnode* next;
} charnode;

charnode* makeCharnode(char c);
charnode* strToCharlist(char* arg);
charnode* intToCharlist(int num);
charnode* floatToCharlist(float f);
void freeCharnodeList(charnode* head);
charnode* append(charnode* head, charnode* toAppend);
int length(charnode* head);
char* charnodeToString(charnode* head);

void printCharnode(charnode* head);

/*allows you to scan any quantity of chars :) ;*/
char* wordFromScan();

#endif
