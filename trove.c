#include "trovefunc.c"

#define OPTLIST "f:brul:"
int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        usage();
    }
    int opt;
    while ((opt = getopt(argc, argv, OPTLIST)) != -1)
    {
        switch (opt)
        {
        case 'f':
            printf("");
            break;
        }
    }
    return -1;
}
