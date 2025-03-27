#include "myinclude.h"

void fiu1(char *input_file, int pfd)
{
    FILE *fp, *be;
    if ((be = fopen(input_file, "r")) == NULL)
        syserr("fopen");
    if ((fp = fdopen(pfd, "w")) == NULL)
        syserr("fdopen");
    double num;
    while (fscanf(be, "%lf", &num) == 1)
    {
        if (fprintf(fp, "%lf\n", num) < 0)
            syserr("fprintf");
    }
    fclose(fp);
    fclose(be);
}

void fiu2(char *output_file, int pfd)
{
    if (dup2(pfd, STDIN_FILENO) == -1)
        syserr("dup2");
    execlp("sort", "sort", "-n", "-o", output_file, NULL);
    syserr("execlp");
}

int main(int argc, char **argv)
{
    if (argc != 3)
        syserr("Usage: ./fel_sort <input_file> <output_file>");

    int pfd[2];
    if (pipe(pfd) == -1)
        syserr("pipe");
    pid_t pid1;
    if ((pid1 = fork()) == -1)
        syserr("fork1");

    if (pid1 == 0)
    {
        // Child 1
        if (close(pfd[0] == -1))
            syserr("close");
        fiu1(argv[1], pfd[1]);
        exit(EXIT_SUCCESS);
    }
    else
    {
        // parent
        pid_t pid2;
        if ((pid2 = fork()) == -1)
            syserr("fork2");
        if (pid2 == 0)
        {
            // Child 2
            if (close(pfd[1]) == -1)
                syserr("close");
            fiu2(argv[2], pfd[0]);
            exit(EXIT_SUCCESS);
        }
        else
        {
            // parent
            if (close(pfd[0]) == -1)
                syserr("close");
            if (close(pfd[1]) == -1)
                syserr("close");
            wait(NULL);
            wait(NULL);
        }
    }
}