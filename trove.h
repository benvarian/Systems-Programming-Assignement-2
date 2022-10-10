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

extern void usage(void);
// extern void print(bool, bool, bool, char *);
extern void myPrint(void);
extern void search(char *, int);
// Checks if the directory addtional arg is a directory or file
extern int valid(char *);
