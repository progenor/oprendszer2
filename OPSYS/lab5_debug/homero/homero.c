
#include "myinclude.h"

// B és C feladata
// mindketten ugyanazt végzik
// pipew: a saját cső írható fele, name: a beolvasott fájl
void fiu_feladat(int pipew, char *name) {
    FILE *fp1 = fopen(name, "r");
    if (fp1 == NULL) syserr("fp1");

    double x;
    while (fscanf(fp1, "%lf", &x) == 1) {
        if (write(pipew, (void *)&x, sizeof(double)) == -1) syserr("write");
    }
    if (ferror(fp1)) syserr("fp1err");
    fclose(fp1);
    close(pipew);
}

int main(int argc, char const *argv[]) {
    pid_t p1;  // B folyamat pid és pipe
    int pfd1[2];

    // B pipe, fork
    if (pipe(pfd1) == -1) syserr("pipe1");
    p1 = fork();
    if (p1 == -1) syserr("fork1");
    // B folyamat
    if (p1 == 0) {
        if (close(pfd1[0]) == -1) syserr("close");
        fiu_feladat(pfd1[1], "b.txt");
        exit(EXIT_SUCCESS);
    } else {
        // A folyamat tovább
        //  erre nincs többet szükség
        if (close(pfd1[1]) == -1) syserr("close");

        pid_t p2;  // C folyamat pid és pipe
        int pfd2[2];
        // pipe 2, C fork
        if (pipe(pfd2) < 0) syserr("pipe1");
        p2 = fork();
        if (p2 == -1) syserr("fork1");

        // C folyamat
        if (p2 == 0) {
            if (close(pfd2[0]) == -1) syserr("close");
            if (close(pfd1[0]) == -1) syserr("close");
            fiu_feladat(pfd2[1], "c.txt");
            exit(EXIT_SUCCESS);
        } else {
            // A folyamat tovább
            if (close(pfd2[1]) == -1) syserr("close");

            // A folyamat feladata
            FILE *fifo;
            if ((fifo = fopen("myfifo", "w")) == NULL) syserr("fifo");

            double x1, x2;
            int n1, n2;
            while (1) {
                if ((n1 = read(pfd1[0], (void *)&x1, sizeof(double))) == -1) syserr("read");
                if ((n2 = read(pfd2[0], (void *)&x2, sizeof(double))) == -1) syserr("read");
                if (n1 == 0 || n2 == 0) break;
                if (fprintf(fifo, "%lf\n", x1 - x2) < 0) syserr("fprintf");
            }
            fclose(fifo);
            wait(NULL);
            wait(NULL);

            return EXIT_SUCCESS;
        }
    }
}