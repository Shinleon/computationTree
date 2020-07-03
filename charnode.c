
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define zero_char 48

typedef struct charnode {
  char data;
  struct charnode* next;
} charnode;

charnode* makeCharnode(char c);
charnode* strToCharlist(char* arg);
charnode* intToCharlist(int num);
charnode* doubleToCharlist(double x);
void freeCharnodeList(charnode* head);
void freeCharnode(charnode* singlular);
charnode* append(charnode* head, charnode* toAppend);
void printCharnode(charnode* head);
int length(charnode* head);
charnode* trimCharnode(charnode* head);
char* charnodeToString(charnode* head);
char* wordfromscan();

charnode* makeCharnode(char c) {
  charnode* ret = malloc(sizeof(charnode));
  ret->data = c;
  ret->next = NULL;
  return ret;
}

charnode* strToCharlist(char* arg){
  charnode* ret = NULL;
  charnode* temp = ret;
  for(int i = 0; i < strlen(arg); i++){
    if(ret == NULL){
      ret = makeCharnode(arg[i]);
      temp = ret;
    } else {
      temp->next = makeCharnode(arg[i]);
      temp = temp->next;
    }
  }
  return ret;
}

charnode* intToCharlist(int num){
  charnode* ret = NULL;
  charnode* temp = ret;
  while(num != 0){
    char numeral = (char)(num % 10 + zero_char);
    if(ret == NULL){
      ret = makeCharnode(numeral);
      temp = ret;
    } else {
      ret = makeCharnode(numeral);
      ret->next = temp;
      temp = ret;
    }
    num /= 10;
  }
  return ret;
}

charnode* floatToCharlist(float f){
  char output[50];
  snprintf(output, 50, "%.3e", f);
  return strToCharlist(output);
}
// charnode* floatToCharlist(float f){
//   printf("printing float: %f: ", f);
//   long int size = sizeof(f);
//   unsigned char bytes_temp[size];
//   memcpy(bytes_temp, (&f), size);
//   for(int i = 0; i < size; i++)
//   {
//     printf("%d ", bytes_temp[i]);
//   }
//   printf("\nEnd print\n");
//   return NULL;
// }
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

charnode* append(charnode* head, charnode* toAppend){
  if(!head){
    return toAppend;
  }
  charnode* temp = head;
  while(temp->next){
    temp = temp->next;
  }
  temp->next = toAppend;
  return head;
}

void printCharnode(charnode* head) {
  if (head) {
    printf("%c -> ", head->data);
    printCharnode(head->next);
  } else {
    printf("(nil)\n");
  }
}

int length(charnode* head) {
  if (head) {
    return 1 + length(head->next);
  }
  return 0;
}

/* cuts white space out of a charnode list
 * so that when taking in the string '    hello  '
 * the char node holds h->e->l->l->o->(nil)
 */
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

char* charnodeToString(charnode* head) {
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
  if (c != '\n') {
    // printf("\"%c",c);
    if( c != ' ' || beginning != 1 ) {
      item = makeCharnode(c);
      temp = item;
    }
    c = getc(stdin);
    while (c != '\n') {
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
  char* ret = charnodeToString(item);
  freeCharnodeList(item);
  return ret;
}