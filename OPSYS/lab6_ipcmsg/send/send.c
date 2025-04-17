// elkuld egy uzenetet az uzenetsorba
#include "send.h"

#include "myinclude.h"

int main() {
    int msqid;  // az uzenetsor azonositoja
    key_t key;
    message_buf sbuf;
    size_t buf_length;

    key = ftok(FTFILE, FTPRJ);  // kulcs

    if ((msqid = msgget(key, IPC_CREAT | 0660)) == -1) {
        syserr("msgget");
    }

    sbuf.mtype = 1;  // az uzenet tipusanak beallitasa

    strcpy(sbuf.mtext, "Az üzenet tartalma");

    // figyelem: mindig az uzenet pontos hosszat kiszamolni
    // a mtype nelkul
    buf_length = strlen(sbuf.mtext) + 1;

    // kuldes
    if (msgsnd(msqid, &sbuf, buf_length, 0) == -1) {
        printf("%d, %ld, %s, %ld\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
        syserr("msgsnd");
    }

    printf("Üzenet: \"%s\" elküldve\n", sbuf.mtext);
    exit(EXIT_SUCCESS);
}
