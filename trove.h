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
#include <locale.h>
#include <signal.h>

#define HASHTABLE_SIZE 10
typedef struct _list
{
    char *string;
    struct _list *next;
} LIST;

typedef LIST *HASHTABLE;
// print to stderr the usage function e
extern void usage(void);
// simple recursive directory search that checks if its reg file or dir
extern void search(char *, int, int, HASHTABLE *);
// Checks if the directory addtional arg is a directory or file
extern int isDirectory(char *);
// checks if its a valid word and if so adds it to the hashtable
extern void regex(char *, int, HASHTABLE *);
// move the hashtable data into the "trove" file.
extern void dump(HASHTABLE *, char *);
// zip the trove file using fork and execl
extern void zip(char *);
// grab the stdout into a pipe and simply make a new hashtable contianing that data then do requried operations on it and zip it again
extern void unZip(char *);
// checks if the l number is valid for length of a word
extern bool lenCheck(int);

// validate if a word is valid or not
extern bool validWord(char[], int);
// try to locate the trove file and if it isnt found and b isnt true throw perror
extern void troveFile(char *, bool);

//  ALLOCATE SPACE FOR A NEW HASHTABLE (AND ARRAY OF LISTS)
extern HASHTABLE *hashtable_new(void);

//  ADD A NEW STRING TO A GIVEN HASHTABLE
extern void hashtable_add(HASHTABLE *, char *);

//  DETERMINE IF A REQUIRED STRING ALREADY EXISTS IN A GIVEN HASHTABLE
extern bool hashtable_find(HASHTABLE *, char *);

//  'CREATE' A NEW, EMPTY LIST
extern LIST *list_new(void);

//  ADD A NEW (STRING) ITEM TO AN EXISTING LIST
extern LIST *list_add(LIST *list, char *);

//  DETERMINE IF A REQUIRED ITEM (A STRING) IS STORED IN A GIVEN LIST
extern bool list_find(LIST *list, char *wanted);

//  PRINT EACH ITEM (A STRING) IN A GIVEN LIST TO stdout
extern void list_print(LIST *list);

extern void hashtable_print(HASHTABLE *);
