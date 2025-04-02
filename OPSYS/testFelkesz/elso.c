#include "myinclude.h"

#define MAXBUFF 80
void child1(int pfd1)
{
    int fd;
    FILE *file;
    char line[MAXBUFF];
    int num;

    if ((fd = open("d2.txt", O_RDONLY, 0644)) < 0)
        syserr("file1");

    file = fdopen(fd, "r");
    if (file == NULL)
        syserr("fdopen");

    // Read number by number and write to pipe
    while (fgets(line, MAXBUFF, file) != NULL)
    {
        // Parse the number
        if (sscanf(line, "%d", &num) != 1)
            continue; // Skip if not a valid integer

        // Write the number to the pipe
        if (write(pfd1, &num, sizeof(int)) < 0)
            syserr("write error to pipe");
    }

    fclose(file); // This also closes fd

    if (close(pfd1) < 0)
        syserr("pfd1 close");
}

void parent(int pfd0)
{
    FILE *file;
    int num1, num2;
    int sum;
    int out_fd;
    char line[MAXBUFF];

    file = fopen("d1.txt", "r");
    if (file == NULL)
        syserr("open file");

    if ((out_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
        syserr("open output file");

    // Read alternatively from d1.txt and the pipe
    while (1)
    {
        // Read from d1.txt - one line per number
        if (fgets(line, MAXBUFF, file) == NULL)
            break; // End of file

        // Parse number from line
        if (sscanf(line, "%d", &num1) != 1)
            continue; // Skip if not a valid integer

        // Read from pipe
        if (read(pfd0, &num2, sizeof(int)) <= 0)
            break; // End of pipe

        // Calculate sum
        sum = num1 + num2;

        // Write to output file
        char output[100];
        sprintf(output, "%d\n", sum);
        if (write(out_fd, output, strlen(output)) < 0)
            syserr("write to output file");

        //  Later implementation with named pipe
        // char fifo_name[] = "fifo1";
        // int fifo_fd;
        // if ((fifo_fd = open(fifo_name, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
        //     syserr("open fifo");

        // if (write(fifo_fd, &sum, sizeof(int)) < 0)
        //     syserr("write to fifo");
        // close(fifo_fd);
    }

    fclose(file);
    if (close(out_fd) < 0)
        syserr("close output file");
    if (close(pfd0) < 0)
        syserr("close pipe");
}

int main()
{
    int pfd[2];
    pid_t pid;

    // Create pipe
    if (pipe(pfd) < 0)
        syserr("pipe");

    // Create child process
    if ((pid = fork()) < 0)
        syserr("fork");

    if (pid == 0)
    {
        // Child process
        close(pfd[0]);  // Close read end
        child1(pfd[1]); // Call child1 with write end
        exit(EXIT_SUCCESS);
    }
    else
    {
        // Parent process
        close(pfd[1]);  // Close write end
        parent(pfd[0]); // Call parent with read end

        // Wait for child to finish
        if (wait(NULL) < 0)
            syserr("wait");
    }

    return 0;
}