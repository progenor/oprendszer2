#include "myinclude.h"
/*
 * egyszerű csővezeték apa és fiú folyamat között
 * a fiú folyamat átír egy kis sztringet az apának
 * csővezetéken keresztül
 * fordítás:
 *    gcc -Wall pipe.c -o pipe
 */

#define MAX 512 /* puffer hossza az olvasáshoz */

int main()
{
    /* a pipe függvény hozza létre a csővezeték azonosítóit
     * ezt a fork() előtt le kell futtatni
     */
    int pfd[2]; /* a csővezeték azonosítói */
    if (pipe(pfd) < 0)
    {
        syserr("pipe");
    }

    pid_t pid;
    if ((pid = fork()) < 0)
    {
        syserr("fork");
    }
    // fiú folyamat:
    if (pid == 0)
    {
        /*szabály: a folyamat lezárja a csővezeték azon felet amelyet nem használ*/
        if (close(pfd[0]) < 0)
        { /* írni fog, az olvasó felét lezárja */
            syserr("close");
        }

        char a[] = "Helló!"; // ezt a kis sztringet írjuk át a csövön
        /* átírja a sztringet, megy a \0 is */
        if (write(pfd[1], a, strlen(a) + 1) < 0)
        {
            syserr("write");
        }

        /* a csövet használat után le kell zárni, ekkor tudja meg az olvasó
           folyamat, hogy vége az adat átvitelnek */
        if (close(pfd[1]) < 0)
        {
            syserr("close1");
        }
        exit(EXIT_SUCCESS);
    }
    else
    { // apa folyamat:
        if (close(pfd[1]) < 0)
        { // olvas, tehát bezárja az írható felét
            syserr("close");
        }
        /* ez a folyamat olvas: amíg a fiú nem ír a csőbe,
           addig a folyamat a read-nél blokkolva van
           a read akkor tér vissza, ha MAX karaktert olvasott
           vagy a másik fél lezárja a csővezetéket
           */
        int n;
        char b[MAX]; // puffer
        while ((n = read(pfd[0], b, MAX)) > 0)
        {
            printf("az apa kiolvasta amit fiú folyamat írt: %s\n", b);
        }
        if (n < 0)
        {
            syserr("read");
        }

        if (close(pfd[0]))
        {
            syserr("close2");
        }

        wait(NULL); /* fiúra vár */
        exit(EXIT_SUCCESS);
    }
}