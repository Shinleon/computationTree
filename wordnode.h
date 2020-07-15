
#ifndef WORDNODE_H
#define WORDNODE_H

typedef struct wordnode {
  char* word;
  struct wordnode* next;
} wordnode;

wordnode* makeWordnode(char* word);
void freeWordnode(wordnode* head);
void printWordnode(wordnode* head);
wordnode* linearInsert(wordnode* head, wordnode* searchkey);
wordnode* removeWordNode(wordnode* head, char* searchkey);
wordnode* inWordnodeList(wordnode* head, char* searchkey);

#endif
