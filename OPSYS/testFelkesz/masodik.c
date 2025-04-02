#include "myinclude.h"

#define MAXBUFF 80

void readFromFIFO()
{
    char fifo_name[] = "output.txt";

    int fd;
    if ((fd = open(fifo_name, O_RDONLY)) < 0) // Fixed parentheses
        syserr("open fifo");

    FILE *file;
    if ((file = fdopen(fd, "r")) == NULL)
        syserr("fdopen fifo");

    int fd2;
    if ((fd2 = open("kiugro.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) // Fixed parentheses
        syserr("open output file");

    int num;
    char line[MAXBUFF];
    FILE *outFile;
    if ((outFile = fdopen(fd2, "w")) == NULL)
        syserr("fdopen output file");

    while (1)
    {
        if (fgets(line, MAXBUFF, file) == NULL)
            break; // End of file
        if (sscanf(line, "%d", &num) != 1)
            continue;  // Skip if not a valid integer
        if (num < 0.7) // Note: comparing int with float, might want to fix
            continue;
        fprintf(outFile, "%d\n", num);
    }

    fclose(outFile); // Close output file
    fclose(file);    // Close input file
}

int main()
{
    int pfd[2];
    pid_t pid1, pid2;

    // First read from FIFO/file
    readFromFIFO();

    // Create pipe for communication between child processes
    if (pipe(pfd) < 0)
        syserr("pipe");

    // Create first child (sorter)
    if ((pid1 = fork()) < 0) // Fixed parentheses
        syserr("fork");

    if (pid1 == 0)
    {
        // Child 1 process - sorts kiugro.txt and sends to pipe
        close(pfd[0]); // Close read end

        // Redirect stdout to pipe
        if (dup2(pfd[1], STDOUT_FILENO) < 0)
            syserr("dup2");

        close(pfd[1]); // Close original after dup2

        // Execute sort command
        execlp("sort", "sort", "-n", "kiugro.txt", NULL);
        syserr("execlp sort");
    }

    // Create second child (display with head)
    if ((pid2 = fork()) < 0) // Fixed parentheses
        syserr("fork");

    if (pid2 == 0)
    {
        // Child 2 process - reads from pipe and displays with head
        close(pfd[1]); // Close write end

        // Redirect stdin from pipe
        if (dup2(pfd[0], STDIN_FILENO) < 0)
            syserr("dup2");

        close(pfd[0]); // Close original after dup2

        // Execute head command (shows first 10 lines by default)
        execlp("head", "head", NULL);
        syserr("execlp head");
    }

    // Parent process
    close(pfd[0]); // Close read end
    close(pfd[1]); // Close write end

    // Wait for both children to finish
    if (waitpid(pid1, NULL, 0) < 0)
        syserr("waitpid");
    if (waitpid(pid2, NULL, 0) < 0)
        syserr("waitpid");

    return 0;
}