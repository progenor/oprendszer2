/*
 * fork és exec együtt = spawn
 */

#include "myinclude.h"

// ez a függvény elvégzi a fork() és execvp() hívást
// a fiú folyamatban ha sikerül nem tér vissza
// a fiú folyamatban ha NEM sikerül hibakóddal tér vissza
// az apa folyamatban visszatér a fiú PID-jével
int spawn(char* prognev, char** argumentumok) {
    pid_t fiu_pid;
    fiu_pid = fork();  // elindítja a fiú folyamatot
    if (fiu_pid != 0) {
        // az apa itt befejezi
        return fiu_pid;
    } else {
        // ez a fiú folyamat
        // az execvp() betölti az új kódot a fiú memóriájába
        execvp(prognev, argumentumok);
        // ide nem szabad eljutni, csak ha hiba történt, és execvp()
        // nem volt végrehajtható
        syserr("execvp");
    }
}

int main() {
    pid_t pid;
    int status;

    // az argumentumok sztring listával vannak megadva, tehát az
    // argv tömb elő van készítve: nem kell rajta semmit alakítani
    char* argumentumok[] = {"ls", "-l", "/home/", (char*)NULL};

    // a spawn függvény elindítja a fiút és az apa ág visszatér ugyanide
    // tehát itt az apa fut tovább a spawn után
    // a fiú folyamat vagy fut, vagy hibával kilép
    pid = spawn("ls", argumentumok);

    wait(&status);  // megvárja a fiút

    if (pid > 0) {
        printf("\nVége, a fiu PID=%d volt.\n", pid);
    } else {
        printf("fiú indítása nem sikerült\n");
    }

    exit(EXIT_SUCCESS);
}
