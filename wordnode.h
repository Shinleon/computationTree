
#ifndef WORDNODE_H
#define WORDNODE_H

struct wordnode {
  char* word;
  struct wordnode* next;
};

struct wordnode* makeWordnode(char* word);
void freeWordnode(struct wordnode* head);
void printWordnode(struct wordnode* head);
struct wordnode* linearInsert(struct wordnode* head, struct wordnode* searchkey);
struct wordnode* removeWordNode(struct wordnode* head, char* searchkey);
struct wordnode* inWordnodeList(struct wordnode* head, char* searchkey);

#endif
