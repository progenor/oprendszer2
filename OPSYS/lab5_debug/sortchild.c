/*
 * 10.6 feladat sablon 
 * */

#include "myinclude.h"

/* a puffer hossza */
#define BUFF 80

int main(int argc, char *argv[])
{
    pid_t pid;
    int pfd[2];
    int fd, n, status;
    char buff[BUFF];

    if (argc != 3)
    {
        printf("használat: %s be.txt ki.txt\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    setbuf(stdout, NULL);

    //TODO: csovezetek letrehozas pfd-ben

    //TODO: fiu letreozasa pid az azonosito

    //fiu kodja, sort fut benne
    if (pid == 0)
    {
        //TODO: nem hasznalt cso azonosito zarasa

        //TODO: kimeneti fajl megnyitasa irasra

        //TODO: STDOUT_FILENO atiranyitasa

        //TODO: STDIN_FILENO atiranyitasa

        //TODO: execlp

        exit(EXIT_SUCCESS);
    }
    //apa
    else
    {
        //TODO: nem hasznalt cso azonosito zarasa

        //TODO: bemeneti fajl megnyitasa olvasasra

        //TODO: fajl atkuldese

        //bezarni a pipe veget, fajlt

        wait(&status);
        exit(EXIT_SUCCESS);
    }
}