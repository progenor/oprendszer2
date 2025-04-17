// fejlecek es közös define
#include "myinclude_kuld.h"

/*
 *  az apa folyamat uzenetsoron keresztul egesz szamokat tartalmazo
 * sztringeket olvas egy masik,
 * folyamattol üzenetsoron keresztul
 * majd csovezeteken keresztül atkuldi a fianak
 * fiu osszeadja oket es kiirja az osszeget
 *
 */

static int qid;  // sor azonosito

// torli a sort
void clean_ipc() {
    if (msgctl(qid, IPC_RMID, 0) == -1) {
        syserr("msgctl");
    }
    printf("Sor törölve\n");
}
// jelzés kilépéskor
void sig_handler(int sig) {
    clean_ipc();
    _exit(EXIT_FAILURE);  // így nem hívódik meg az exit_handler()
}
// exit-kor:
void exit_handler() {
    clean_ipc();
}

int main() {
    setbuf(stdout, NULL);

    pid_t pid;
    int fd[2];  // pipe

    // uzenetsor
    if ((qid = msgget(KEY1, 0666 | IPC_CREAT)) == -1)
        syserr("msgget");

    if (pipe(fd) == -1) syserr("pipe");

    if ((pid = fork()) == -1) syserr("fork");

    if (pid)
    // apa
    // szamokat olvas, atkuldi a fianak
    {
        if (close(fd[0]) == -1) syserr("close");
        // beállítja a jelzéskezelőt
        // megjegyzés: van ennél modernebb módszer, lásd sigaction()
        signal(SIGINT, sig_handler);   // Ctrl+C
        signal(SIGTERM, sig_handler);  // TERM
        signal(SIGSEGV, sig_handler);  // segmentation fault
        signal(SIGPIPE, sig_handler);  // broken pipe

        // beállítja az exit-kor lefutó függvényt
        atexit(exit_handler);

        while (1) {
            // sztringeket olvas ciklusban
            if (msgrcv(qid, &mbuff, BUFSIZE, 1, 0) == -1) {
                syserr("msgrcv");
            }
            mbuff.b[strlen(mbuff.b) - 1] = '\0';

            int n;
            n = atoi(mbuff.b);  // egesz szamma alakitja
            // atirja  a szamot
            if (write(fd[1], &n, sizeof(n)) == -1) {
                syserr("write");
            }

            // printf("apa: üzenet: %s , konvertálva számmá: %d\n", mbuff.b, n);
            if (!strcmp(mbuff.b, "0")) {
                break;
            }
        }

        strcpy(mbuff.b, "ok\n");

        mbuff.mtype = 2;

        // vissza kuld egy "ok"-t
        if (msgsnd(qid, &mbuff, 4, 0) == -1) {
            syserr("msgsnd");
        }

        if (close(fd[1]) == -1) syserr("close");

        // megvarja a fiat
        wait(NULL);
        exit(EXIT_SUCCESS);

    } else {  // fiu
        // összeadja a számokat
        if (close(fd[1]) == -1) syserr("close2");

        int n, s = 0;
        while (read(fd[0], &n, sizeof(int)) == sizeof(int)) {
            if (n == 0) {
                break;
            }
            s += n;
        }
        if (n < 0) syserr("read");

        close(fd[0]);

        printf(" fiú: az olvasott összeg: %d\n", s);

        fflush(stdout);

        _exit(EXIT_SUCCESS);  // mivel a fiu orokli a beallitast, így az nem fut le
    }
}
