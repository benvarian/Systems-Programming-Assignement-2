#include "trove.h"

void regex(char *fn, int searchSize)
{

    // // then test the length of the word with the length then if its valid hashtable_add
    // if is valid add to gloably declared hashtable
    FILE *fp = fopen(fn, "r");

    char buf[PATH_MAX];
    while (fscanf(fp, "%1023s", buf) != EOF)
    {
        // if (validWord(buf, size))
        // {
        //     printf("%s\n", buf);
        // }
        // printf("%s:%d\n", buf, validWord(buf, searchSize));
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

void search(char *fn, int indent, int searchSize)
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
                    regex(realpath(path, buf), searchSize);
                }
                else if (S_ISDIR(info.st_mode))
                {
                    search(path, indent + 1, searchSize);
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

// The criteria for a valid word is that a letter that isnt first is a capital is invalid
// if it contains any of the listed chars in the first if statment
// if the word
bool validWord(char str[], int len)
{
    int hyphen = 0;
    int size = strlen(str);
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
            if ((str[size] == ',' && isalpha(str[size - 1])) && (str[size] == '.' && isalpha(str[size])))
                return true;
        }
        return true;
    }
    else
    {
        return false;
    }
}

void troveFile(char *file, bool b)
{
    if (strcmp(file, "/tmp/trove") == 0)
    {
        char buf2[PATH_MAX];
        if (access(realpath(file, buf2), F_OK & R_OK) == 0 && b == true)
        {
            printf("trove file found, overwriting...\n");
            remove(realpath(file, buf2));
            FILE *trove = fopen(file, "w");
            if (trove == NULL)
            {
                perror("error");
            }
            fclose(trove);
        }
        else if (b == true)
        {
            printf("Building a new one\n");
            printf("%s\n", file);
            FILE *fp = fopen(file, "w");
            if (fp == NULL)
            {
                perror("Error:");
            }
            fclose(fp);
        }
    }
    else
    {
        char buf[PATH_MAX + 1];
        char filelocation[PATH_MAX];
        sprintf(filelocation, "/tmp/%s", file);

        if (access(realpath(filelocation, buf), R_OK) == 0 && b == true)
        {
            printf("Existing trove-file has been found, overwriting\n");
            remove(realpath(filelocation, buf));
            FILE *fp = fopen(filelocation, "w");
            if (fp == NULL)
            {
                perror("error");
            }
        }
        else
        {
            printf("The trove file that was supplied doesnt exist, building a new one\n");
            char fulldir[PATH_MAX];
            strcpy(fulldir, "/tmp/");
            strcat(fulldir, file);
            FILE *fp = fopen(fulldir, "w");
            if (fp == NULL)
            {
                perror("error");
            }
        }
    }
}

char *strndup(const char *s, size_t n)
{
    char *p;
    size_t n1;

    for (n1 = 0; n1 < n && s[n1] != '\0'; n1++)
        continue;
    p = malloc(n + 1);
    if (p != NULL)
    {
        memcpy(p, s, n1);
        p[n1] = '\0';
    }
    return p;
}
