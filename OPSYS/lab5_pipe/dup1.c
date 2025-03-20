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

        pid_t pid1;
        if ((pid1 = fork()) < 0) {
            syserr("fork");
        }
        if(pid1==0){
            int fd1;
            if ((fd1 = open("../teszt.txt",   O_RDONLY, 0644)) < 0) {
                syserr("open");
            }
            if (dup2(fd1, STDIN_FILENO) == -1) {
                syserr("dup2");
            }
            if(dup2(fd[1], STDOUT_FILENO) == -1){
                syserr("dup2");
            }

            execlp("cat", "cat", (char *)NULL); syserr("execlp");
        }
        
        if(close (fd[1]) < 0){
            syserr("close");
        }
        wait(NULL);  // megvárom amíg a fiú végez

    }

    exit(EXIT_SUCCESS);
}
