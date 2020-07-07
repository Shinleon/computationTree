/* FINAL: wordnode.c
 * Leonard Shin
 * CSE 374
 * SPRING 2020
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct wordnode {
  char* word;
  struct wordnode* next;
} wordnode;

wordnode* makeWordnode(char* word);
void freeWordnode(wordnode* head);
void printWordnode(wordnode* head);
wordnode* linearInsert(wordnode* head, wordnode* searchkey);
int stringCompare(char* former, char* latter);

wordnode* makeWordnode(char* word) {
  wordnode* ret = malloc(sizeof(wordnode));
  ret->word = word;
  ret->next = NULL;
  return ret;
}

void freeWordnode(wordnode* head) {
  if (head) {
    freeWordnode(head->next);
    free(head->word);
    free(head);
  }
}

void printWordnode(wordnode* head) {
  if (head) {
    printf("%s -> ", head->word);
    printWordnode(head->next);
  } else {
    printf("(NULL)\n");
  }
}

/**
 * assumes list given by head is already sorted.
 * assumes all wordnodes have accompanying, non-NULL words;
 * assumes searchkey is a singular wordnode whose next is NULL;
 * returns the list with searchkey inserted in place.
 */
wordnode* linearInsert(wordnode* head, wordnode* searchkey) {
  if (head) {
    if (stringCompare(head->word, searchkey->word) >= 0) {
      // head->word is after searchkey->word or the same;
      searchkey->next = head;
      return searchkey;
    }
    // only pass this point is searchkey->word comes after
    //  head->word;
    wordnode* prev = head;
    while (prev->next) {
      wordnode* aft = prev->next;
      if (stringCompare(aft->word, searchkey->word) >= 0) {
        // aft->word is after searchkey->word or the same;
        searchkey->next = aft;
        prev->next = searchkey;
        return head;
      }
      prev = prev->next;
    }
    // only pass htis point is searchkey->word is to be placed
    //  at the end of the list;
    prev->next = searchkey;
    return head;
  }
  // only pass this point is head is NULL
  return searchkey;
}

wordnode* removeWordNode(wordnode* head, char* searchkey)
{
  if(head)
  {
    int comparison = strcmp(head->word, searchkey);
    if(comparison == 0)
    {
      wordnode* temp = head->next;
      free(head);
      return temp;
    }
    head->next = removeWordNode(head->next, searchkey);
  }
  return head;
}

wordnode* inWordnodeList(wordnode* head, char* searchkey)
{
  if(head)
  {
    return (strcmp(searchkey, head->word) == 0) ? head : inWordnodeList(head->next, searchkey);
  }
  return NULL;
}

int stringCompare(char* former, char* latter) {
  char* lformer = malloc(sizeof(char) * (strlen(former) + 1));
  char* llatter = malloc(sizeof(char) * (strlen(latter) + 1));

  int i = 0;
  for (; i < strlen(former); i++) {
    // printf("%d ", i);
    lformer[i] = tolower(former[i]);
  }
  lformer[i] = '\0';
  // printf("\n");

  i = 0;
  for (; i <strlen(latter); i++) {
    // printf("%d ", i);
    llatter[i] = tolower(latter[i]);
  }
  llatter[i] = '\0';
  // printf("\n");

  int ret = strcmp(lformer, llatter);
  free(lformer);
  free(llatter);
  return ret;
}
