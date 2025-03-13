/*
 * pelda az execl() hasznalatara
 */

#include "myinclude.h"

int main(int argc, char* argv[]) {
    pid_t pid;   // folyamat azonosító
    int status;  // kilépési állapot tárolására
    pid = fork();  // új folyamat
    if (pid < 0) {
        syserr("fork");
    }

    if (pid == 0) {
        // fiú folyamat, új programot indítunk
        //  az ls -l parancsot indítjuk
        // a /bin/ls azért szerepel kétszer, mert az első sztring
        // az indítandó programkód helye, a második a parancssor első argumentuma
        execlp("ls", "ls", "-l", (char*)NULL);
        // ide már nem tér vissza
        // ha mégis, akkor hiba
        syserr("execl");
    } else {
        // apa folyamat megvárja a fiút
        wait(&status);
    }
    exit(EXIT_SUCCESS);
}
