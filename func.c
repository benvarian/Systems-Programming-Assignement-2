#include "trove.h"

void regex(char *fn)
{
    char *file = "grep";
    char *arg1 = "-oE";
    char *arg2 = "--null-data";
    char *regexPattern = "\\b\\w{4,}\\b";
    printf("%s\n", fn);

    if (execlp(file, file, arg1, arg2, regexPattern, fn, (char *)NULL) == -1)
        perror("error");
}

void myPrint(void)
{
    printf("hi");
}
void usage(void)
{
    printf("./trove [-f trovefile] [-b | -r | -u] [-l length] [filelist | word]\n");
}

HASHTABLE *search(char *fn, int indent)
{
    DIR *dir;
    struct dirent *entry;
    HASHTABLE *hashtable = hashtable_new();
    char path[PATH_MAX + 1];
    struct stat info;

    if ((dir = opendir(fn)) == NULL)
        perror("opendir() error");
    else
    {
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_name[0] != '.')
            {
                strcpy(path, fn);
                strcat(path, "/");
                strcat(path, entry->d_name);
                if (stat(path, &info) != 0)
                {
                    fprintf(stderr, "stat() error on %s: %s\n", path,
                            strerror(errno));
                }
                else if (S_ISREG(info.st_mode))
                {
                    // regex(path);
                    // printf("%s\n", path);
                    hashtable_add(hashtable, path);
                    // perror("error");
                }
                else if (S_ISDIR(info.st_mode))
                {
                    // printf("%s\n", path);
                    search(path, indent + 1);
                }
            }
        }
        closedir(dir);
    }
    int x = hashtable_find(hashtable, "trove-sample-data/src/mycal/first_day_of_month.c");
    printf("%d\n", x);
    return hashtable;
}

int valid(char *fn)
{
    char buf[PATH_MAX];

    char *res = realpath(fn, buf);
    if (res)
    {
        // printf("%s\n", res);
        return 1;
    }
    else
    {
        perror("Path Error");
        return 0;
    }
}
