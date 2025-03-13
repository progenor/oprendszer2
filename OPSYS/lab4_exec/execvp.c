/* példa az execvp használatára */

#include "myinclude.h"

// a parancssort feldolgozó és execvp()-vel elindító függvény
// bemenet: parancssor sztring, pl:
//    "ls -l /home"
// kimenet: hibakód, ha nem hajtódik végre a parancs
// az execvp() nem tér vissza, ha sikerült 
int parancssor(const char *parancs) {
    const char *elvalaszto = " ";
    int argc = 0;
    char **argv = NULL; /*ebben lesz az argv*/
    char *masolat = NULL;
    char *s;

    /* mivel a strtok() tönkreteszi a sztringet amin dolgozik,
     * egy másolatot kell készíteni róla */
    masolat = (char *)malloc(strlen(parancs) + 1);
    if (masolat == NULL)
        return -1;
    strcpy(masolat, parancs);
    argc = 0;
    s = strtok(masolat, elvalaszto);
    while (s != NULL) {
        /* van még egy paraméter, megnövelem az argv[]-t */
        argv = (char **)realloc(argv, (size_t)(argc + 1) * (sizeof(*argv)));
        /* helyet foglalok a következő stringnek */
        argv[argc] = (char *)malloc((size_t)strlen(s) + 1);

        strcpy(argv[argc], s);
        argc++;
        s = strtok(NULL, elvalaszto);
        /* a második hívásnál NULL az első paraméter */
    }
    /* amikor befejezzük, az utolsó mutatónál vagyunk, még be kell írni egyet */
    argv = (char **)realloc(argv, (size_t)(argc + 1) * (sizeof(*argv)));
    argv[argc] = NULL;
    /* itt megvan az argv */
    execvp(argv[0], argv);
    /* ide akkor jutunk ha nem sikerült */
    /* felszabditjuk a memóriat */
    while (--argc > 0)
        free(argv[argc]);
    free(masolat);
    return EXIT_FAILURE;
}

#define MAX 4096  // egy parancssor hossza

int main(int argc, char *argv[]) {
    pid_t pid;
    int status;
    char parancs[MAX];

    setbuf(stdout, NULL);

    /* parancssor beolvasása: */
    printf("írjunk be egy parancsot(például: ls -l /home ):");
    if (fgets(parancs, MAX, stdin) == NULL) { /*olvasás stdin-ről*/
        printf("olvasás hiba\n");
        exit(1);
    }

    // az fgets() által beolvasott sztring végén
    // van egy újsor, azt felülírjuk
    parancs[strlen(parancs) - 1] = '\0';

    pid = fork();
    if (pid < 0) {
        syserr("fork");
    }

    if (pid == 0) {
        /* fiú folyamat, új programot indítunk */
        parancssor(parancs);

        /* ide már nem tér vissza */
        syserr("execv hiba\n");
    }

    /*apa folyamat megvarja a fiut */
    wait(&status);
    exit(EXIT_SUCCESS);
}
