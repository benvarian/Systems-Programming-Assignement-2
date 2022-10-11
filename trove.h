#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdint.h>

#define HASHTABLE_SIZE 991
typedef struct _list
{
    char *string;
    struct _list *next;
} LIST;

typedef LIST *HASHTABLE;

extern void usage(void);
// extern void print(bool, bool, bool, char *);
extern void myPrint(void);
extern HASHTABLE *search(char *, int);
// Checks if the directory addtional arg is a directory or file
extern int valid(char *);
// regex function
extern void regex(char *);

//  ALLOCATE SPACE FOR A NEW HASHTABLE (AND ARRAY OF LISTS)
extern HASHTABLE *hashtable_new(void);

//  ADD A NEW STRING TO A GIVEN HASHTABLE
extern void hashtable_add(HASHTABLE *, char *string);

//  DETERMINE IF A REQUIRED STRING ALREADY EXISTS IN A GIVEN HASHTABLE
extern bool hashtable_find(HASHTABLE *, char *string);

//  'CREATE' A NEW, EMPTY LIST
extern LIST *list_new(void);

//  ADD A NEW (STRING) ITEM TO AN EXISTING LIST
extern LIST *list_add(LIST *list, char *newstring);

//  DETERMINE IF A REQUIRED ITEM (A STRING) IS STORED IN A GIVEN LIST
extern bool list_find(LIST *list, char *wanted);

//  PRINT EACH ITEM (A STRING) IN A GIVEN LIST TO stdout
extern void list_print(LIST *list);


