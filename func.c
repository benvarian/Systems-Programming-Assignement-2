#include "trove.h"

void regex(char *fn)
{
    // // reads space by space or other characters like - _ +
    // // then test the length of the word with the length then if its valid hashtable_add
    // if is valid add to gloably declared hashtable
    FILE *fp = fopen(fn, "r");
    int size = 4;
    char buf[PATH_MAX];
    while (fscanf(fp, "%1023s", buf) != EOF)
    {
        // if (validWord(buf, size))
        // {
        //     printf("%s\n", buf);
        // }
        printf("%s:%d\n", buf, validWord(buf, size));
    }
    fclose(fp);
}
void myPrint(void)
{
    printf("hi");
}
void usage(void)
{
    printf("./trove [-f trovefile] [-b | -r | -u] [-l length] [filelist | word]\n");
}

void search(char *fn, int indent)
{
    DIR *dir;
    struct dirent *entry;
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
                    char buf[PATH_MAX];
                    regex(realpath(path, buf));
                    // perror("error");
                    // printf("%s\n", realpath(path, buf));
                }
                else if (S_ISDIR(info.st_mode))
                {
                    search(path, indent + 1);
                }
            }
        }
        closedir(dir);
    }
}

int isDirectory(char *fn)
{
    char buf[PATH_MAX];
    char *res = realpath(fn, buf);
    struct stat statbuf;
    if (!res)
    {
        perror("Path Error");
        return 0;
    }
    if (stat(res, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

void removeFile(char *file)
{
    if (remove("results.txt") != 0)
    {
        perror("Coludnt Remove File");
    }
}

bool validWord(char str[], int len)
{
    int hyphen = 0;
    int size = strlen(str);
    // printf("%s:%d\n", str, size);
    // printf("%c:%s:%d\n", str[0], str, isspace(str[0]));

    if (isspace(str[0]) == 0 && ispunct(str[0]) == 0 && size >= len)
    {
        for (int i = 0; i < size; i++)
        {
            if (str[i] == '!' || str[i] == '@' || str[i] == '#' || str[i] == '$' || str[i] == '%' || str[i] == '^' || str[i] == '&' || str[i] == '*' || str[i] == '(' || str[i] == ')' || str[i] == '{' || str[i] == '}' || str[i] == '[' || str[i] == ']' || str[i] == ':' || str[i] == ';' || str[i] == '"' || str[i] == '<' || str[i] == '>' || str[i] == '.' || str[i] == '/' || str[i] == '?' || str[i] == '~' || str[i] == '`')
            {
                return false;
            }
            if (isalpha(str[i]))
                continue;
            if (isupper(str[i]))
                return false;
            if (str[size - 1] == ',' || str[size - 1] == '.')
                return true;
            if (str[i] == '\'' && str[i - 1] != '\0')
                return true;
            if (str[i] == '-')
            {

                // Only 1 hyphen is allowed
                if (++hyphen > 1)
                    return false;
                // hyphen should be surrounded
                // by letters
                if (i - 1 < 0 || !isalpha(str[i - 1]) || i + 1 >= size || !isalpha(str[i + 1]))
                    return false;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}
