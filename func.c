#include "trove.h"

void regex(char *fn, int searchSize, HASHTABLE *hash)
{

    // // then test the length of the word with the length then if its valid hashtable_add
    // if is valid add to gloably declared hashtable
    FILE *fp = fopen(fn, "r");
    // char b[PATH_MAX ];
    char buf[PATH_MAX];
    char wordbuf[PATH_MAX];
    while (fscanf(fp, "%1023s", wordbuf) != EOF)
    {
        if (validWord(wordbuf, searchSize))
        {
            // printf("%s", fn);
            // duping the relapath up so it caters for the files that are passed directly to the function from trove.c
            strcpy(buf, realpath(fn, NULL));
            strcat(buf, ":");
            strcat(buf, wordbuf);

            hashtable_add(hash, buf);
        }
    }

    fclose(fp);
}

void usage(void)
{
    printf("./trove [-f trovefile] [-b | -r | -u] [-l length] [filelist | word]\n");
}

void search(char *fn, int indent, int searchSize, HASHTABLE *hash)
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
                    regex(realpath(path, buf), searchSize, hash);
                }
                else if (S_ISDIR(info.st_mode))
                {
                    search(path, indent + 1, searchSize, hash);
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
            // fprintf(stdout, "%s\n\n", filelocation);
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
            strcat(fulldir, ".txt");

            FILE *fp = fopen(fulldir, "w");
            if (fp == NULL)
            {
                perror("error");
            }
        }
    }
}

void dump(HASHTABLE *hash, char *f)
{

    // move the hashtable data into the "trove" file.
    char buf[PATH_MAX + 1];
    strcpy(buf, "/tmp/");
    strcat(buf, f);
    strcat(buf, ".txt");
    // fprintf(stdout, "%s\n", buf);
    if (access(buf, R_OK) != 0)
    {
        perror("Error");
    }
    else
    {
        int fp = open(buf, O_WRONLY);

        if (fp == -1)
            perror("Error");

        // we are going to implement a pipe to send all the stdout to the trove-file for storage
        dup2(fp, STDOUT_FILENO);
        hashtable_print(hash);
        close(fp);
        // reopen STDOUT pipe
        // if theres a better way to do this pls put it in report
        int fd = open("/dev/tty", O_WRONLY);
        stdout = fdopen(fd, "w");
    }
}

void zip(char *trove)
{

    char *cmd = "/usr/bin/gzip";
    char *cmd2 = "gzip";
    char *args = "-qf";

    char path[PATH_MAX];
    strcpy(path, "/tmp/");
    strcat(path, trove);
    strcat(path, ".txt");
    // fprintf(stdout, "%s", path);
    pid_t childpid = fork();
    if (childpid == 0)
    {
        if (execl(cmd, cmd2, args, path, (char *)NULL) == -1)
            perror("Error:");
    }
}

void unZip(char *file)
{
    // HASHTABLE *hash = hashtable_new();
    char path[PATH_MAX];
    strcpy(path, "/tmp/");
    strcat(path, file);
    char *f = realpath(path, NULL);
    char *cmd = "/usr/bin/zcat";
    char *cmd2 = "zcat";
    char *flags = "-c";
    pid_t pid = fork();
    if (pid == 0)
    {
        fprintf(stdout, "hit\n");
        // if (execlp("whoami", "whoami", (char *)NULL) == -1)
        //     fprintf(stdout, "hit2\n");

        if (execl(cmd, cmd2, flags, f, (char *)NULL) == -1)
            perror("Error:");
    }
    // printf("hit2\n");
}
