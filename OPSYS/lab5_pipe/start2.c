/* fork feladatok
 * a 4. feladat megoldása , 5_pipe_feladatok.pdf
 * forditas:
 *       gcc -Wall start2.c -o start2
 * indítás:
 *
 *      ./start2  prog1 prog2
 *
 *   stdin -> PROG1 -> pipe -> PROG2
 *
 *      például:
 *      ./start2  cat sort
 *      vagy:
 *      ./start2 tac cat
 *
 *  egy apa folyamat felügyeli a két fiút
 *
 *  ebben a példa indításban: ./start2  cat sort
 *  az egyik fiúban  a cat fut, a másik fiúban pedig a sort
 *  amit a cat ír, azt a sort fogja olvasni
 *
 *  a cat a standard bementről olvas, be kell írni sorokat
 *  a sort a standard kimenetre ír, rendezve kiírja a sorokat
 *
 *  mindkét folyamatban duplikálni kell a fájl azonosítókat
 */

#include "myinclude.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("használat: %s prog1 prog2\n", argv[0]);
        exit(1);
    }

    int fd[2];
    if (pipe(fd) < 0) {
        syserr("pipe");
    }

    pid_t pid;
    if ((pid = fork()) < 0) {
        syserr("fork hiba");
    }

    // elso fiu:
    if (pid == 0) {
        // éz argv[1] -et kell indítsa, a csőbe ír, olvasható véget lezárja
        close(fd[0]);
        // hozzárendelem a pipe írható felét az stdout-hoz
        if (dup2(fd[1], STDOUT_FILENO) < 0) {
            syserr("dup2 hiba");
        }
        // elindítom az első argumentumnak megfelelő programot
        execlp(argv[1], argv[1], NULL);
        syserr("exec fiu1-ben nem indul");
        // fiú ide nem ér el
    }

    if ((pid = fork()) < 0) {
        syserr("fork hiba");
    }
    // második fiú:
    if (pid == 0) {
        // cső írható fele lezárva
        close(fd[1]);
        // a pipe olvasható fele a stdin leíró számát kapja
        // innen fog olvasni a sort
        if (dup2(fd[0], STDIN_FILENO) < 0)
            syserr("dup2 hiba");

        execlp(argv[2], argv[2], NULL);
        syserr("exec a fiu2-ben nem indul");
    }
    //  !!! figyelem:
    // az apa lezárja a csővezetek mindkét véget, mert ezek az ő folyamatában
    // nyitva vannak - bár nem fogja használni őket
    // ez a művelet fontos - ha nyitva hagyja, a fiai azt hiszík, hogy
    // még valaki használja a csövet, és
    // arra várnak, hogy írjon belé
    // így blokkolódni fognak, és ha az apa elkezdi a wait-ét a zárás
    // nélkül akkor ő is blokkolódik

    close(fd[0]);
    close(fd[1]);

    wait(NULL), wait(NULL);

    exit(EXIT_SUCCESS);
}
