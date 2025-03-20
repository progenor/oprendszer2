/* példa a dup2() használatára
 * az apa folyamat átír néhány sort egy pipe-on keresztül
 * a pipe a másik végén a sort várja és elrendezi a sorokat
 *
 *  int dup2(int oldfd, int newfd);
 *
 *  másolatot készít az oldfd fájl azonosítóról,
 *  és azt a newfd -bé írja be
 *  ha newfd előtte nyitva volt egy más fájlra, lezárja
 *
 * tesztelés:
 *   gcc -Wall dup.c -o dup
 *   ./dup
 */

#include "myinclude.h"

int main() {
    setbuf(stdout, NULL);

    int fd[2];           // pipe
    if (pipe(fd) < 0) {  // pipe létrehozás
        syserr("pipe");
    }

    pid_t pid;
    if ((pid = fork()) < 0) {
        syserr("fork");
    }

    // fiú folyamat:
    if (pid == 0) {
        close(fd[1]);  // bezárom a pipe írható felét

        // a pipe olvasható vége a standard input lesz
        // ha elvégzem a deszkriptor duplikálást
        if (dup2(fd[0], STDIN_FILENO) == -1) {
            syserr("dup2");
        }
        // elindítom a sort-ot
        execlp("sort", "sort", (char *)NULL);
        // ez a stdin-en várja bemenetet mert
        // nincs fájl argumentuma
        syserr("execlp");
    } else  // apa:
    {
        close(fd[0]);  // itt nincs szükség az olvasható felére

        FILE *fp;                 // standard könyvtári fájl mutató
        fp = fdopen(fd[1], "w");  // átalakítom fd-t fp-ve
                                  // így fp-t úgy használhatom mint FILE *
        if (fp == NULL) {
            syserr("fdopen");
        }

        // ezek sorok a sort számára
        fprintf(fp, "xyz\n");
        fprintf(fp, "abc\n");
        fprintf(fp, "klm\n");
        fprintf(fp, "aaa\n");

        // az fflush fontos lehet ha több folyamat ír/olvas a stdout-ra
        fflush(fp);
        fclose(fp);

        wait(NULL);  // megvárom amíg a fiú végez
    }

    exit(EXIT_SUCCESS);
}
