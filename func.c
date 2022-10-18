#include "trove.h"

void regex(char *fn)
{
    char *cmd = "grep";
    char *arg1 = "-raoE";
    char *arg2 = "--null-data";
    char *regexPattern = "\\b\\w{4,}\\b";

    // HASHTABLE *h = hashtable_new();

    int pid = fork();
    if (pid == -1)
    {
        perror("error");
    }
    if (pid == 0)
    {
        int file = open("results.txt", O_WRONLY | O_CREAT, 0777);
        if (file == -1)
        {
            perror("err");
        }
        dup2(file, STDOUT_FILENO);
        printf("%s:", fn);
        close(file);
        int err = execlp(cmd, cmd, arg1, arg2, regexPattern, fn, (char *)NULL);
        if (err == -1)
        {
            perror("couldnt run");
        }
    }
    else
    {
        int wstatus;
        wait(&wstatus);
        if (WIFEXITED(wstatus))
        {
            int statuscode = WEXITSTATUS(wstatus);
            if (statuscode != 0)
            {
                perror("error");
                exit(EXIT_FAILURE);
            }
        }
        // removeFile("results.txt");
    }
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
                    char buf[PATH_MAX];
                    // regex(realpath(path, buf));
                    // perror("error");
                    hashtable_add(hashtable, realpath(path, buf));
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
    hashtable_print(hashtable);
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
