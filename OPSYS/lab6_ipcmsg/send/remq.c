// kitorli az uzenetsort
#include "myinclude.h"
#include "send.h"

int main() {
    int msqid;  // az uzenetsor azonositoja
    key_t key;  // a generalo kulcs

    key = ftok(FTFILE, FTPRJ);  // kulcs

    if ((msqid = msgget(key, 0666)) == -1) {
        printf("Nem létezik a keresett sor\n");
        syserr("msgget");
    }

    // torli a sort
    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        syserr("msgctl");
    }

    printf("Sor törölve\n");
    exit(EXIT_SUCCESS);
}
