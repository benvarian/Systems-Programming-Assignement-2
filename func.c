#include "trove.h"

bool lenCheck(int x)
{
    if (x < 1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void regex(char *fn, int searchSize, HASHTABLE *hash)
{
    // open the files that gets passed and read it in word for word in a 1024 sized buf
    FILE *fp = fopen(fn, "r");
    char buf[PATH_MAX];
    char wordbuf[PATH_MAX];
    while (fscanf(fp, "%1023s", wordbuf) != EOF)
    {
        if (validWord(wordbuf, searchSize))
        {
            // duping the relapath up so it caters for the files that are passed directly to the function from trove.c
            strcpy(buf, realpath(fn, NULL));
            strcat(buf, ":");
            strcat(buf, wordbuf);
            // adds string of path:word to the hashtable so its easy to read later
            hashtable_add(hash, buf);
        }
    }
    fclose(fp);
}

void usage(void)
{
    fprintf(stderr, "./trove [-f trovefile] [-b | -r | -u] [-l length] [filelist | word]\n");
}

void search(char *fn, int indent, int searchSize, HASHTABLE *hash)
{
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX + 1];
    struct stat info;
    // the supplied file wasnt a folder so it couldnt be searched
    if ((dir = opendir(fn)) == NULL)
        perror("Error with a directory that was supplied:");
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
                    // is file is regular simply pass it to the regex function
                    // to get read word for word
                    regex(realpath(path, NULL), searchSize, hash);
                }
                else if (S_ISDIR(info.st_mode))
                {
                    // recursive search part
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
// the plan was to use grep with execl and pipe that stdout to the file with the realpath
// but that wasnt allowed so this had to settle
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
            // if -f (trove) is found to exist itll overwrite that file and make an empty one with the same name
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
            // if -b is specified but the file doesnt exist and the name is trove create one
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
        // if the name isnt trove
        char buf[PATH_MAX + 1];
        char filelocation[PATH_MAX];
        sprintf(filelocation, "/tmp/%s", file);

        if (access(realpath(filelocation, buf), R_OK) == 0 && b == true)
        {
            // existing file found going to overwrite with an empty file
            remove(realpath(filelocation, buf));

            FILE *fp = fopen(filelocation, "w");
            if (fp == NULL)
            {
                perror("error");
            }
        }
        else
        {
            // name wasnt trove and it doenst exist so simply make an empty txt file to zip later
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

    char buf[PATH_MAX + 1];
    strcpy(buf, "/tmp/");
    strcat(buf, f);
    strcat(buf, ".txt");
    // make sure we can read in the trove file
    if (access(buf, R_OK) != 0)
    {
        perror("Error");
        exit(EXIT_FAILURE);
    }
    // open the file and get the file descriptor from it
    // the fp should be 3 for example 
    int fp = open(buf, O_WRONLY);
    // if it failed to open throw error 
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

void zip(char *trove)
{
    // args for gzip command 
    char *cmd = "/usr/bin/gzip";
    char *cmd2 = "gzip";
    char *args = "-qf";
    char path[PATH_MAX];
    // make the path the form of the .txt file that was created in the dump function above
    strcpy(path, "/tmp/");
    strcat(path, trove);
    strcat(path, ".txt");
    // make a child process
    pid_t childpid = fork();
    // if it the parent process make a child that does the gzip command 
    if (childpid == 0)
    {
        if (execl(cmd, cmd2, args, path, (char *)NULL) == -1)
            perror("Error:");
    }
}

void unZip(char *file)
{
    // this is where the program stops as i couldnt get zcat to work it threw the error 
    // that said zcat: standard input is a terminal -- ignoring
    // but the idea was to crate a pipe that reads in the stdout and move it into a hashtable 
    // then compare with the required function and then dump back into the gziped file using another stdout pipe


    // HASHTABLE *hash = hashtable_new();
    // char path[PATH_MAX];
    // strcpy(path, "/tmp/");
    // strcat(path, file);
    // args 
    // char *f = realpath(path, NULL);
    // char *cmd = "/usr/bin/zcat";
    // char *cmd2 = "zcat";
    // char *flags = "-c";
    
    // pid_t pid = fork();
    // if (pid == 0)
    // {
        // if you uncommment this youll find the problem 
        // if (execl(cmd, cmd2, flags, f, (char *)NULL) == -1)
        //     perror("Error:");
    // }
}
