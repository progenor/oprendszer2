/*
 * ugyanaz a program pingpongozik onmagaval: 2 X kell elinditani
 * egy sztringet kuld oda-vissza uzenetsoron
 * a felek minden atkuldesnel megesznek egy karaktert
 * a parbeszed leall ha a sztring elfogy
 *
 */

#include "myinclude.h"

#define SIZE 128  // az uzenet hossza

typedef struct {
    long mtype;
    char mtext[SIZE];
} message_buf;

int main(int argc, char* argv[]) {
    int msqid;                      // az uzenetsor id-je
    int msgflg = IPC_CREAT | 0660;  // az uzenetsor keres allapotjelzoje
    key_t key;
    message_buf sbuf;
    size_t buf_length;
    /* segedtipusk es valtozok az allapotok
     * kovetesere */

    typedef enum { elso = 0,
                   masodik } whoami;  // ki vagyok elso vagy masodik
    typedef enum { kuldo = 0,
                   fogado } program_status;  // kuldo vagyok vagy fogado

    whoami who = elso;            // hanyadiknak indulok
    program_status pass = kuldo;  // eppen milyen allapotban vagyok

    key = ftok(".", 'b');

    if ((msqid = msgget(key, msgflg)) == -1) {
        syserr("msgget");
    }
    printf("a sor azonosítója: %d\n", msqid);
    // megallapitom, ki vagyok? az elso vagy masodik program
    // probalok uzenetet olvasni, de nem varok: IPC_NOWAIT
    // a hossz 0, tehat nem olvas, de ha van uzenet akkor E2BIG hibat ad
    if (msgrcv(msqid, &sbuf, 0, 0, IPC_NOWAIT) == -1) {
        switch (errno) {
            // ha nincs meg uzenet a sorban, elso vagyok
            case ENOMSG:
                who = elso;
                pass = kuldo;
                break;
                // ha mar van, a masodik
            case E2BIG:
                who = masodik;
                pass = fogado;
                break;
            default:
                syserr("msgrcv: nem tud dönteni");
        }
        printf("%d %s vagyok (errno=%d)\n", getpid(), (who ? "második" : "első"), errno);
    }

    sbuf.mtype = who;  // az uzenet tipusanak beallitasa
    // mindig a kuldo alairasat viseli
    strcpy(sbuf.mtext, "aaaaaaaaaaaa\0");

    // itt a vegtelen while

    while (1) {
        printf("következik:%s, tevékenység:%s, a string:%s\n",
               (who ? "második" : "első"),
               (pass ? "fogad" : "kuld"),
               sbuf.mtext);

        if (pass == kuldo) {
            // kuldes
            sbuf.mtext[strlen(sbuf.mtext) - 1] = '\0';  // hamm
            sbuf.mtype = who + 1;

            buf_length = strlen(sbuf.mtext) + 1;
            if (buf_length <= 1) {
                break;  // vege
            }
            if (msgsnd(msqid, &sbuf, buf_length, 0) == -1) {
                syserr("msgsnd");
            }
        } else {
            // fogadas
            if (msgrcv(msqid, &sbuf, SIZE, !who + 1, 0) == -1) {
                if (errno == EIDRM) {
                    exit(0);  //  a masik torolte a sort
                }
                syserr("msgrcv");
            }
            printf("lefutott:%s, tevékenység:%s, a string:%s\n",
                   (who ? "első" : "második"),
                   (pass ? "fogad" : "kuld"),
                   sbuf.mtext);
        }

        // allapotvaltas
        if (pass == kuldo) {
            pass = fogado;
        } else {
            pass = kuldo;
        }
    }

    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        syserr("msgctl");
    }

    exit(EXIT_SUCCESS);
}
