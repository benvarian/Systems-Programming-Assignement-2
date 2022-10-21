//  CITS2002 Project 2 2022
//  Student1:   23215049  Varian   Benjamin   100

// program compiled and ran on macos monterey 12.6

// I had the same partner as last time who was "in my team" up until the 21st and simply removed himself from the team
// i will provide git logs for proof if anything can be done in the zipped folder

#include "trove.h"

#define OPTLIST "-f:brul:"

int main(int argc, char *argv[])
{
    // make sure the format is set at UTF-8
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
        //  as i was unable to get the zcat or gunzip function to work i cant implement the simple word search of the file 
        // but refer to the comments under the zip function for what i wouldve done 
        // for (int index = optind; index < argc; index++)
        // {
        //     printf("%s\n", argv[index]);
                // unZip(f);
        // }
        // makes sure the lenght for valid words is valid
        if (!lenCheck(searchSize))
        {
            fprintf(stderr, "Plese input a length value that is greater than 1\n");
            exit(EXIT_FAILURE);
        }
        if (b)
        {
            HASHTABLE *hash = hashtable_new();
            printf("\tBuilding a trove file...\n");
            troveFile(f, b);
            // this part just goes through the extra commands that are parsed into the program and checks if they are a 
            // directory or reg file and does the appropriate steps to futher progress the function
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
            // dumps hash contents into the trove file 
            dump(hash, f);
            zip(f);
            // this part here is ran in the parent process and waits for the child process
            // to finish to process basically allowing another fork to occur in the unzip process
            int status = 0;
            while (wait(&status) > 0)
                ;
        }
        else if (r | u)
        {
            //     // begin with reading zip file that is created with zcat into stdout
            //     // move it into hashmap then build new hashmap with new filelist
            //     // if strcmp matches simply delete the data
            //     // then move the data back into a zip folder
            printf("\tUpdating trove file...\n");
            unZip(f);
            int status = 0;
            while (wait(&status) > 0)
                ;
        }
        return 0;
    }
}
