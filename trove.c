//  CITS2002 Project 2 2022
//  Student1:   23215049  Varian   Benjamin   100

#include "trovefunc.c"

#include <getopt.h>

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        usage();
    }
    else
    {
        int opt;
        char *f = "";
        int lvalue = 4;
        char *x;
        bool b, r, u;
        while ((opt = getopt(argc, argv, OPTLIST)) != -1)
        {
            switch (opt)
            {
            case 'f':
                f = strndup(optarg, strlen(optarg));
                break;
            case 'b':
                b = true;
                break;
            case 'r':
                r = true;
                break;
            case 'u':
                u = true;
                break;
            case 'l':
                lvalue = atoi(optarg);
                break;
            case '?':
                exit(EXIT_FAILURE);
                break;
            case ':':
                printf("needed arg for %c\n", optopt);
                break;
            }
        }

        printf("%s:%d\n", f, lvalue);
        // assigns x to ideally the word or filelist, this will get checked in later functions
        for (int index = optind; index < argc; index++)
            x = argv[index];

        print(b, u, r, x);
        return -1;
    }
}
