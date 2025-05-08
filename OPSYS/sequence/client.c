#include "sequence.h"

static int qid;
// kliens program
// argv[1] : hányadik feladat
int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);

    // itt megoldani a kliens feladatot
    if (argc != 2)
    {
        fprintf(stderr, "Hiba: %s <pid>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((qid = msgget(SVRKEY, 0)) == -1)
    {
        syserr("cl: msgget");
    }
    pid_t pid = getpid();
    client_msg cmsg;
    server_msg smsg;

    char buff[1024];
    FILE *fp;
    if ((fp = fopen(argv[1], "r")) == NULL)
    {
        syserr("cl: fopen");
    }

    while (fgets(buff, sizeof(buff), fp) != NULL)
    {
        if (buff[0] == '\n')
            continue;
        int seq = atoi(buff);
        cmsg.mtype = 1L;
        cmsg.pid = (long)pid;
        if (msgsnd(qid, &cmsg, CSIZE, 0) < 0)
        {
            syserr("cl: msgsnd");
        }
        if (msgrcv(qid, &smsg, SSIZE, pid, 0) < 0)
        {
            syserr("cl: msgrcv");
        }

        if (smsg.seq == -1)
        {
            fprintf(stderr, "cl: %s: hiba\n", argv[1]);
            break;
        }

        printf("seq szam: %d   munka: %d", pid, smsg.seq);
        usleep(100);
    }

    fclose(fp);
    exit(EXIT_SUCCESS);
}
