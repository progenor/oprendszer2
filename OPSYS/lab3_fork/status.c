/*
 *  a wait által kapott status tesztelése
 */

#include "myinclude.h"

int main() {
    setbuf(stdout, NULL);

    pid_t pid;
    pid = fork();  // új folyamat indul

    if (pid < 0) {  // ha hiba történt
        printf("\n hibás fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {  // fiú folyamat
        printf("Fiú: %d folyamat vagyok.\n", getpid());
        sleep(2);
        exit(2);
    } else {  // apa folyamat
        int status;
        wait(&status);
        if (WIFEXITED(status)) {  // ha exit()-el lépett ki
            printf("A fiú exit kódja: %d\n", WEXITSTATUS(status));
        }
        exit(EXIT_SUCCESS);
    }
}