/*
 * leteszteli, hogy hany uzenetet lehet egy uzenetsorba
 * bekuldeni
 * addig kuld, amig eléri a rendszer hatarat
 *
 */

#include "myinclude.h"

#define SIZE 128  // az uzenet hossza

typedef struct {
    long mtype;
    char mtext[SIZE];
} message_buf;

int main() {
    int msqid;                      // az uzenetsor azonositoja
    int msgflg = IPC_CREAT | 0660;  // az uzenetsor keres allapotjelzoje

    key_t key;
    message_buf sbuf;
    size_t buf_length;
    int counter = 0;

    key = ftok(".", 'l');

    if ((msqid = msgget(key, msgflg)) == -1) {
        syserr("msgget");
    }
    printf("msgget: msgget sikerült: id = %d\n", msqid);

    sbuf.mtype = 1;  // az uzenet tipusanak beallitasa

    strcpy(sbuf.mtext, "Az üzenet tartalma.");
    buf_length = strlen(sbuf.mtext) + 1;

    while (++counter) {
        // kuldes
        // ha IPC_NOWAIT akkor hibara visszajon
        if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) == -1) {
            counter--;  // ez mar nem ment el
            // mi a hiba:
            perror("msgsnd");
            // ha hiba akkor megtelt
            printf("sor megtelt: %d darab %d byte hosszú üzenet lett elküldve\n", counter, (int)buf_length);
            printf("helyfoglalás: %d byte\n", (int)(counter * buf_length));
            // torli a sort
            if (msgctl(msqid, IPC_RMID, NULL) == -1) {
                syserr("msgctl");
            }
            printf("sor törölve\n");
            break;
        }
        printf("Üzenet: \"%s\" elküldve.\n", sbuf.mtext);
    }
    exit(EXIT_SUCCESS);
}
