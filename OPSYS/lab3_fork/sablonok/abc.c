/*
 *  Labor 3, Feladat 1 sablon
 */

#include "myinclude.h"

// n darab c karaktert ír a name nevű fájlba
int makefile(char *name, char c, int n) {
    
    int fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        syserr("open");
    }

    for (int i = 0; i < n; i++) {
        if (write(fd, &c, 1) != 1) {
            syserr("write");
        }
    }

    // fájl zárás
    close(fd);
    return 0;
}

int main(int argc, char *argv[]) {
    pid_t pid1, pid2;      // folyamat azonosítók
    int status1, status2;  // kilépési állapotok átvételéhez

    // első fiú indítása
    if ((pid1 = fork()) < 0) {
        syserr("f1");
    }
    if (pid1 == 0) {
        // első fiú kódja
        makefile("b.txt", 'b', 1000);
        exit(EXIT_SUCCESS);  // a fiú itt mindenképp kilép, így
                                 // nem futhat rá a következő kódokra
    }

 
    if ((pid2 = fork()) < 0) {
        syserr("f2");
    }
    if (pid2 == 0) {
        // második fiú kódja
        makefile("c.txt", 'c', 1000);
        exit(EXIT_SUCCESS);
    }
   
    makefile("a.txt", 'a', 1000);



    // apa megvárja mindkét fiát
    wait(&status1);
    if(WIFEXITED(status1)){
        printf("eloso kilepes kodja %d\n", WEXITSTATUS(status1));
    }
    wait(&status2);
    if(WIFEXITED(status2)){
        printf("masodik kilepes kodja %d\n", WEXITSTATUS(status2));
    }

    exit(EXIT_SUCCESS);
}
