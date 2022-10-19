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
#include <fcntl.h>
#include <ctype.h>

#define die(e)                      \
    do                              \
    {                               \
        fprintf(stderr, "%s\n", e); \
        exit(EXIT_FAILURE);         \
    } while (0);

#define HASHTABLE_SIZE 997
typedef struct _list
{
    char *string;
    struct _list *next;
} LIST;

typedef LIST *HASHTABLE;

extern void usage(void);
// extern void print(bool, bool, bool, char *);
extern void myPrint(void);
extern void search(char *, int, int);
// Checks if the directory addtional arg is a directory or file
extern int isDirectory(char *);
// regex function
extern void regex(char *, int);
// remove the created results file
extern void removeFile(char *);
// validate if a word is valid or not
extern bool validWord(char[], int);
// try to locate the trove file and if it isnt found and b isnt true throw perror
extern void troveFile(char *, bool );

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

extern void hashtable_print(HASHTABLE *);

char *strndup(const char *s, size_t n) {
	char *p;
	size_t n1;
	
	for (n1 = 0; n1 < n && s[n1] != '\0'; n1++)
		continue;
	p = malloc(n + 1);
	if (p != NULL) {
		memcpy(p, s, n1);
		p[n1] = '\0';
	}						    
	return p;
}
