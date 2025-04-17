// kivesz egy uzenetet a sorbol
#include "myinclude.h"
#include "send.h"

int main() {
    int msqid;         // az uzenetsor azonositoja
    key_t key;         // a generalo kulcs
    message_buf rbuf;  // az uzenet bufefere

    key = ftok(FTFILE, FTPRJ);  // kulcs

    // ha nincs sor kilép
    if ((msqid = msgget(key, 0660)) == -1) {
        syserr("msgget");
    }

    // itt fogadom  az uzenetet
    if (msgrcv(msqid, &rbuf, SIZE, 1, IPC_NOWAIT) == -1) {
        if (errno == ENOMSG) {
            printf("Nincs üzenet a sorban\n");
            exit(EXIT_SUCCESS);
        }
        syserr("msgrcv");
    }

    printf("Új üzenet: %s\n", rbuf.mtext);
    exit(EXIT_SUCCESS);
}