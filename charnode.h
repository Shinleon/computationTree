
#ifndef CHARNODE_H
#define CHARNODE_H

struct charnode {
  char data;
  struct charnode* next;
};

struct charnode* makeCharnode(char c);
struct charnode* strToCharlist(char* arg);
struct charnode* intToCharlist(int num);
struct charnode* floatToCharlist(float f);

void freeCharnodeList( struct charnode* head);

struct charnode* append(struct charnode* head, struct charnode* toAppend);
int length(struct charnode* head);

char* charnodeToString(struct charnode* head);

void printCharnode(struct charnode* head);

/*allows you to scan any quantity of chars :) ;*/
char* wordFromScan();

#endif
