
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "constants.h"

#include "charnode.h"

void freeCharnode(struct charnode* singlular);
struct charnode* trimCharnode(struct charnode* head);

struct charnode* makeCharnode(char c) 
{
  struct charnode* ret = malloc(sizeof(struct charnode));
  ret->data = c;
  ret->next = NULL;
  return ret;
}

struct charnode* strToCharlist(char* arg)
{
  struct charnode* ret = NULL;
  struct charnode* temp = ret;
  for(int i = 0; i < (int) strlen(arg); i++)
  {
    if(ret == NULL)
    {
      ret = makeCharnode(arg[i]);
      temp = ret;
    } 
    else 
    {
      temp->next = makeCharnode(arg[i]);
      temp = temp->next;
    }
  }
  return ret;
}

struct charnode* intToCharlist(int num)
{
  struct charnode* ret = NULL;
  struct charnode* temp = ret;
  while(num != 0)
  {
    char numeral = (char)(num % 10 + ZERO_CHAR);
    if(ret == NULL)
    {
      ret = makeCharnode(numeral);
      temp = ret;
    } 
    else 
    {
      ret = makeCharnode(numeral);
      ret->next = temp;
      temp = ret;
    }
    num /= 10;
  }
  return ret;
}

struct charnode* floatToCharlist(float f)
{
  char output[50];
  snprintf(output, 50, "%.3e", f);
  return strToCharlist(output);
}

void freeCharnodeList(struct charnode* head) 
{
  //printf("entering freecharnode list: %p\n", head);
  if (head) 
  {
    //printf("head: %p \"%c\"\n", head, head->data);
    freeCharnodeList(head->next);
    free(head);
  }
  //printf("exiting freecharnode list\n");
}

void freeCharnode(struct charnode* singular)
{
  free(singular);
}

struct charnode* append(struct charnode* head, struct charnode* toAppend)
{
  if(!head)
  {
    return toAppend;
  }
  struct charnode* temp = head;
  while(temp->next)
  {
    temp = temp->next;
  }
  temp->next = toAppend;
  return head;
}

void printCharnode(struct charnode* head) 
{
  if (head) 
  {
    printf("%c -> ", head->data);
    printCharnode(head->next);
  } 
  else 
  {
    printf("(nil)\n");
  }
}

int length(struct charnode* head) 
{
  if (head) 
  {
    return 1 + length(head->next);
  }
  return 0;
}

/* cuts white space out of a charnode list
 * so that when taking in the string '    hello  '
 * the char node holds h->e->l->l->o->(nil)
 */
struct charnode* trimCharnode(struct charnode* head) 
{
  if(head)
  {
    if(head->data == ' ')
    {
      struct charnode* temp = trimCharnode(head->next);
      if (temp == NULL) 
      {
        freeCharnode(head);
        return trimCharnode(temp);
      }
      head->next = temp;
      return head;
    } 
    else 
    {
      head->next = trimCharnode(head->next);
      return head;
    }
  } 
  else 
  {
    return NULL;
  }
}

char* charnodeToString(struct charnode* head) 
{
  //head = trimCharnode(head);
  int len = length(head);
  char* ret = malloc(sizeof(char)*(len + 1));
  int i = 0;
  for (; i < len; i++) 
  {
    ret[i] = head->data;
    head = head->next;
  }
  ret[i] = '\0';
  return ret;
}

char* wordFromScan() 
{
  struct charnode* item = NULL;
  struct charnode* temp = NULL;
  char c = getc(stdin);
  int beginning = 1;
  if (c != '\n') 
  {
    // printf("\"%c",c);
    if( c != ' ' || beginning != 1 ) 
    {
      item = makeCharnode(c);
      temp = item;
    }
    c = getc(stdin);
    while (c != '\n' && c != EOF) 
    {
      // printf("%c",c);
      if(temp) {
	      temp->next = makeCharnode(c);
	      temp = temp->next;
      }
      else 
      {
	      if( c != ' ' || beginning != 1)
         {
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
