//  CITS2002 Project 2 2022
//  Student1:   23215049  Varian   Benjamin   100

#include "trove.h"

#define OPTLIST "-f:brul:"

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        usage();
    }
    else
    {
        int indent = 0;
        int opt;
        char *f = "";
        int searchSize = 4;
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
                searchSize = atoi(optarg);
                break;
            case '?':
                exit(EXIT_FAILURE);
                break;
            case ':':
                printf("needed arg for %c\n", optopt);
                break;
            case 1:
                printf("Non-option arg: %s\n", optarg);
                break;
            }
        }

        printf("%s:%d\n", f, searchSize);
        // assigns x to ideally the word or filelist, this will get checked in later functions
        for (int index = optind; index < argc; index++)
        {
            if (b | r | u)
            {
                // debug
                // printf("\nbuilding a trove file \n");
                // printf("%s", argv[index]);

                if (valid(argv[index]))
                    search(argv[index], indent);

            }
        }

        return 0;
    }
}
