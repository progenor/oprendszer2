// sablon az 1.b feladathoz
#include "myinclude.h"

#define MAXLEN 1024  // egy sor max hossza

int main() {
    int msglen;  // üzenet hossza

    struct msgbuf {
        long mtype;
        char buff[MAXLEN];
    } message;  // az üzenetpuffer

    int qid;  // a sor azonosítója
    // TODO: üzenetsort lekérni

    pid_t pid1;
    if ((pid1 = fork()) == -1)
        syserr("fork1");
    if (pid1 == 0) {
        // TODO: első fiú, olvas a terminálról és ír a sorba

        exit(EXIT_SUCCESS);
    }

    pid_t pid2;
    if ((pid2 = fork()) == -1)
        syserr("fork2");
    if (pid2 == 0) {
        FILE* fp;  // második fiú fp
        // TODO: második fiú, olvas sorból és ír a fájlba

        exit(EXIT_SUCCESS);
    }

    // TODO: apa megvárja a fiait, törli a sort

    exit(EXIT_SUCCESS);
}