//  CITS2002 Project 2 2022
//  Student1:   23215049  Varian   Benjamin   100

#include "trove.h"

#define OPTLIST "-f:brul:"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "en_US.UTF-8");
    if (argc <= 1)
    {
        usage();
    }
    else
    {
        int indent = 0;
        int opt;
        char *f = "/tmp/trove";
        int searchSize = 4;
        bool b, r, u;
        while ((opt = getopt(argc, argv, OPTLIST)) != -1)
        {
            switch (opt)
            {
            case 'f':
                f = NULL;
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
        // test for just searching for words and as well as supplying trove file name
        if (b)
        {
            HASHTABLE *hash = hashtable_new();
            printf("Building a trove file...\n");
            troveFile(f, b);
            for (int index = optind; index < argc; index++)
            {
                if (isDirectory(argv[index]))
                {
                    search(argv[index], indent, searchSize, hash);
                }
                else
                {
                    regex(argv[index], searchSize, hash);
                }
            }

            dump(hash, f);

            zip(f);
        }
        while (wait(NULL) != -1 || errno != ECHILD)
        {
            printf("done1\n");
        }
        if (r | u)
        {
            //     // begin with reading zip file that is created with zcat into stdout
            //     // move it into hashmap then build new hashmap with new filelist
            //     // if strcmp matches simply delete the data
            //     // then move the data back into a zip folder
            printf("Updating trove file...\n");
            unZip(f);
        }
        while (wait(NULL) != -1 || errno != ECHILD)
        {
            printf("done2\n");
        }

        return 0;
    }
}
