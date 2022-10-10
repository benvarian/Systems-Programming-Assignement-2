#include "trove.h"

void myPrint(void)
{
    printf("hi");
}
void usage(void)
{
    printf("./trove [-f trovefile] [-b | -r | -u] [-l length] [filelist | word]\n");
}
void print(bool b, bool u, bool r, char *h)
{
    printf("b:%s\n", b ? "true" : "false");
    printf("u:%s\n", u ? "true" : "false");
    printf("r:%s\n", r ? "true" : "false");
    printf("%s\n", h);
}
