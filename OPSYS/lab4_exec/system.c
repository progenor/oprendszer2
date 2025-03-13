/*
 * példa a system() függvény használatára
 */

#include "myinclude.h"

#define MAX 4096  // egy parancssor hossza

int system1(char *cmd){
    int status = 0;
    pid_t pid = fork();
    if (pid < 0) {
        syserr("fork");
    }
    else if (pid == 0) {
        execl("/bin/sh", "sh", "-c", cmd, (char *) NULL);
        syserr("execl");
    }
    wait(&status);
    return status;    
}

int main(int argc, char* argv[]) {

    setbuf(stdout, NULL);

    /* parancssor beolvasása: */
    printf("írjunk be egy parancsot(például: ls -l /home ):");

    char parancs[MAX];
    if (fgets(parancs, MAX, stdin) == NULL) {  // olvasás stdin-ről
        fprintf(stderr, "olvasás hiba\n");
        exit(EXIT_FAILURE);
    }
 
    // az fgets által beolvasott sztring végén
    // van egy újsor, azt felülírjuk
    parancs[strlen(parancs) - 1] = '\0';

    // a system() függvény meghívása
    //fiú folyamatot indít, lefuttatja a fiúban:
    // execl("/bin/sh", "sh", "-c", "parancs", (char *) NULL);
    //és megvárja, hogy kilépjen waitpid()-del
    //a "parancs"-ban lehetnek átirányítások, pipe-ok, stb.
    int status;
    status = system1(parancs);

    int exitcode = WEXITSTATUS(status);
    if (WIFEXITED(status) && exitcode == 0) {
        printf("A system sikeresen fejezte be a futását\n");
    } else {
        printf("A system hibával lépett ki: %d\n", exitcode);
    }

    exit(EXIT_SUCCESS);
}
