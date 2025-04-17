/*
 * uzenetsor feladatok, 2-es feladat
 *
 * sajat magaval beszelgeto program
 *
 * 2X kell eilinditani, elso alkalommal a parancssor:
 * ./xtalk 1
 * a masodik alkalommal:
 * ./xtalk 2
 *
 * az argumentum eszerint lesz az elso vagy masodik peldany
 * ebbol szamolja  ki, hogy o milyen uzenetet kuld vagy fogad
 *
 * mindket peldany ket folyamatot hoz letre, egyet az irasra es
 * egyet az olvasasra
 *
 * kulon terminalon kell elinditani oket
 *
 * fiu folyamat terminalt olvas, sorba ir
 * apa a sort olvassa, terminalra ir
 *
 * kilepes:
 *
 * fiu "end" sztringet olvas -> atkuldi -> masik apa "end"-et olvas ->
 *    atkuldi elso apanak -> mindket apa TERM-et kuld a fianak utana
 *    megvarj es kilep
 *    a send_no = 1 apa torli a sort
 *
 */

#include "myinclude.h"

#define MAXLEN 256

int main(int argc, char** argv) {
    int qid;              // sor azonosito
    int send_no, rec_no;  // send_no: kuldeskor hasznalt uzenet tipus
                          // rec_no: fogadaskor hasznalt tipus

    struct {  // sor puffer
        long mtype;
        char mtext[MAXLEN];
    } buff;

    pid_t pid;  // pid szam

    if (argc != 2) {
        printf("\ninditas: %s n (n=1 vagy 2)\n", argv[0]);
        exit(1);
    }

    // itt szamolja ki az uzenettipusokat, pl. ha argv[1] = 1 akkor
    // send_no = 1 es rec_no = 2
    // argv[1]=2 eseten forditva
    send_no = *argv[1] - '0';  // a kuldott uzenet tipusa lesz ez

    rec_no = send_no == 1 ? 2 : 1;  // a fogadott uzenet a komplementer szam

    if (send_no > 2 || send_no < 1) {
        printf("\n n=1 vagy 2\n");
        exit(1);
    }

    // nem puffereljuk az stdout-ot
    setbuf(stdout, NULL);

    // uzenetsor lekeres
    if ((qid = msgget(ftok(".", 1), 0660 | IPC_CREAT)) == -1) {
        syserr("msgget");
    }

    if ((pid = fork()) == -1) {
        syserr("fork");
    }

    if (pid == 0) {
        // a fiu  a folyamat az uzenetsorba ir
        while (1) {
            printf("ok:");  // prompt, egy sor olvasasa:
            if (fgets(buff.mtext, MAXLEN, stdin) == NULL) {
                if (ferror(stdin)) {  // ha stdin hiba
                    syserr("stdin");
                }
                // ha fajl vege - Ctrl-D
                // akkor mi irjuk be az end szot
                strcpy(buff.mtext, "end\n");
            }
            //
            // az atkuldott uzenet tipusa
            buff.mtype = send_no;
            // printf("\nuzenetet kuld: %s",buff.mtext);

            if (msgsnd(qid, &buff, strlen(buff.mtext) + 1, 0)) {
                syserr("msgsnd");
            }
            // vege ha a sztring end
            if (!strcmp(buff.mtext, "end\n")) {
                printf("fiú end bemenetet kapott\n");

                // exit(0);  // ki lehetne lepni itt, de a peldaban
                //  mas megoldast alkalmazunk: lasd a kill() hasznalatat az
                //  apaban
            }
        }  // while
        printf("ide nem kell eljutni\n");
        exit(1);
    }

    // apa folyamat, fiu nem érhet ide
    while (1) {
        // ha az uzenet hosszabb mint MAXLEN csonkitjuk
        if (msgrcv(qid, &buff, MAXLEN, rec_no, MSG_NOERROR) == -1) {
            syserr("msgrcv");
        }
        // ha end, vege
        if (!strcmp(buff.mtext, "end\n")) {
            // mielott kilep atkuldi a masik  apanak
            printf("apa exit\n");
            // a tipus a send_no
            buff.mtype = send_no;
            if (msgsnd(qid, &buff, strlen(buff.mtext) + 1, 0)) {
                syserr("msgsnd");
            }
            // utana lezarja a fiat
            printf("apa TERM jelzést kuld\n");
            if (kill(pid, SIGTERM) == -1) {
                syserr("kill");
            }
            break;
        }
        // fogadott uzenetet a terminalra irja
        printf("\n%sok:", buff.mtext);
    }  // while
    // megvarja a fiat
    wait(NULL);
    // amelyiknel a  send_no=1 torli a sort, miutan var egy masodpercet
    if (send_no == 1) {
        sleep(1);
        printf("üzenetsor törlés itt\n");
        if (msgctl(qid, IPC_RMID, NULL) == -1) {
            syserr("IPC_RMID hiba");
        }
    }
    exit(0);
}