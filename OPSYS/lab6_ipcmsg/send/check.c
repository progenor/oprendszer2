// kiveszi az uzenetsor leiro strukturat
// kiirja, hany uzenet van a sorban
#include "myinclude.h"
#include "send.h"

int main() {
    int msqid;  // az uzenetsor azonositoja
    key_t key;  // a generalo kulcs

    struct msqid_ds ds;

    key = ftok(FTFILE, FTPRJ);  // kulcs

    printf("msgget hívás, kulcs = %#lx\n", (long)key);

    if ((msqid = msgget(key, 0666)) == -1) {
        printf("nincs üzenetsor az ftok(%s,%c) kulcsra\n", FTFILE, FTPRJ);
        syserr("msgget");
    }

    // kiveszi a sor allapot leirasat
    if (msgctl(msqid, IPC_STAT, &ds) == -1) {
        syserr("msgctl");
    }

    printf("A sorban %ld üzenet található\n", ds.msg_qnum);

    exit(EXIT_SUCCESS);
}
