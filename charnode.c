/* FINAL: charnode.c
 * Leonard Shin
 * CSE 374
 * SPRING 2020
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct charnode {
  char data;
  struct charnode* next;
} charnode;

charnode* makeCharnode(char c);
void freeCharnodeList(charnode* head);
void freeCharnode(charnode* singlular);
void printCharnode(charnode* head);
int length(charnode* head);
charnode* trimCharnode(charnode* head);
char* toString(charnode* head);
char* wordfromscan();

charnode* makeCharnode(char c) {
  charnode* ret = malloc(sizeof(charnode));
  ret->data = c;
  ret->next = NULL;
  return ret;
}

void freeCharnodeList(charnode* head) {
  //printf("entering freecharnode list: %p\n", head);
  if (head) {
    //printf("head: %p \"%c\"\n", head, head->data);
    freeCharnodeList(head->next);
    free(head);
  }
  //printf("exiting freecharnode list\n");
}

void freeCharnode(charnode* singular){
  free(singular);
}

void printCharnode(charnode* head) {
  if (head) {
    printf("%c -> ", head->data);
    printCharnode(head->next);
  } else {
    printf("(nil char)\n");
  }
}

int length(charnode* head) {
  if (head) {
    return 1 + length(head->next);
  }
  return 0;
}

charnode* trimCharnode(charnode* head) {
  if(head){
    if(head->data == ' '){
      charnode* temp = trimCharnode(head->next);
      if (temp == NULL) {
        freeCharnode(head);
        return trimCharnode(temp);
      }
      head->next = temp;
      return head;
    } else {
      head->next = trimCharnode(head->next);
      return head;
    }
  } else {
    return NULL;
  }
}

char* toString(charnode* head) {
  //head = trimCharnode(head);
  int len = length(head);
  char* ret = malloc(sizeof(char)*(len + 1));
  int i = 0;
  for (; i < len; i++) {
    ret[i] = head->data;
    head = head->next;
  }
  ret[i] = '\0';
  return ret;
}

char* wordFromScan() {
  charnode* item = NULL;
  charnode* temp = NULL;
  char c = getc(stdin);
  int beginning = 1;
  if (c != '\n' && c != EOF) {
    // printf("\"%c",c);
    if( c != ' ' || beginning != 1 ) {
      item = makeCharnode(c);
      temp = item;
    }
    c = getc(stdin);
    while (c != '\n' && c != EOF) {
      // printf("%c",c);
      if(temp) {
	temp->next = makeCharnode(c);
	temp = temp->next;
      } else {
	if( c != ' ' || beginning != 1) {
          item = makeCharnode(c);
	  temp = item;
	  beginning = 0;
        }
      }
      c = getc(stdin);
    }
    // printf("\"\n");
  }
  trimCharnode(item);
  // printCharnode(item);
  char* ret = toString(item);
  freeCharnodeList(item);
  return ret;
}
