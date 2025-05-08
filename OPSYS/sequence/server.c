#include "sequence.h"

// a sor azonosítója külső statikus változó,
// azért hogy a fájlban levő függvényből is elérjük
static int qid;

// ez a függvény törli a sort, minden exit() hívás után
// lefut
void remove_queue()
{
    printf("svr: exit, sor törölve\n");
    if (msgctl(qid, IPC_RMID, NULL) < 0)
    {
        syserr("svr: RMID");
    }
}

// szerver program
int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);

    if (argc != 2)
    {
        fprintf(stderr, "Hiba: %s <pid>\n", argv[0]);
        exit(1);
    }
    // az atexit() függvénnyel be lehet állítani egy void
    // paraméterrel rendelkező függvényt, ami lefut minden exit()
    // hívás után, kilépés előtt
    // oldjuk meg, hogy INT vagy TERM jelzésre is hívódjon meg a függvény
    if (atexit(remove_queue) != 0)
    {
        syserr("svr: atexit");
    }
    if ((qid = msgget(SVRKEY, IPC_CREAT | IPC_EXCL | 0666)) == -1)
    {
        syserr("svr: msgget");
    }

    FILE *fp;
    if ((fp = fopen(argv[1], "r")) == NULL)
    {
        syserr("svr: fopen");
    }

    int seq;
    client_msg cmsg;
    server_msg smsg;
    while (fscanf(fp, "%d", &seq) == 1)
    {
        if (msgrcv(qid, &cmsg, CSIZE, 1, 0) < 0)
        {
            syserr("svr: msgrcv");
        }
        if (cmsg.mtype != 1L)
            continue;
        smsg.mtype = cmsg.pid;
        smsg.seq = seq;
        if (msgsnd(qid, &smsg, SSIZE, 0) < 0)
        {
            syserr("svr: msgsnd");
        }
    }

    fclose(fp);
    int count = 3000;

    while (count > 0)
    {

        if (msgrcv(qid, &cmsg, CSIZE, 1, IPC_NOWAIT) < 0)
        {
            if (errno == ENOMSG)
            {
                usleep(100 * 1000);
                count -= 100;
                continue;
                ;
            }
            else
            {
                syserr("svr: msgrcv");
            }
        }
        smsg.mtype = cmsg.pid;
        smsg.seq = -1;
        if (msgsnd(qid, &smsg, SSIZE, 0) < 0)
        {
            syserr("svr: msgsnd");
        }
        usleep(100 * 1000);
        count -= 100;
    }

    // itt megoldani a szerver feladatot
}
