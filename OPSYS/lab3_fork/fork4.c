/*
 * Hibás kód!!!
 * hány folyamat van a példában ?
 * hány c változó van?
 * mennyi a c változók értéke a folyamatok kilépése előtt?
 */

#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char** argv) {
    int c;
    int child = fork(); // mind a 2 ben lefut
    c = 5;

    if (child == 0) { // masodik folyamat
        c += 5;
    } else {
        child = fork();
        c += 10;
        if (child)
            c += 5;
    }
    exit(EXIT_SUCCESS);
}
