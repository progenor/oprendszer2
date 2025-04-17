// sablon az 1.b feladathoz
#include "myinclude.h"

#define MAXLEN 1024 // egy sor max hossza

int main()
{
    int msglen; // üzenet hossza

    struct msgbuf
    {
        long mtype;
        char buff[MAXLEN];
    } message; // az üzenetpuffer

    int qid; // a sor azonosítója
    // üzenetsort lekérni
    if ((qid = msgget(IPC_PRIVATE, IPC_CREAT | 0666)) == -1)
        syserr("msgget");

    pid_t pid1;
    if ((pid1 = fork()) == -1)
        syserr("fork1");
    if (pid1 == 0)
    {
        // első fiú, olvas a terminálról és ír a sorba
        while (fgets(message.buff, MAXLEN, stdin) != NULL)
        {
            message.mtype = 1;
            msglen = strlen(message.buff) + 1;
            if (msgsnd(qid, &message, msglen, 0) == -1)
            {
                syserr("msgsnd");
            }
            if (strcmp(message.buff, "exit\n") == 0)
            {
                break; // kilépés
            }
        }
        strcpy(message.buff, "exit\n");
        message.mtype = 1;
        msglen = strlen(message.buff) + 1;
        if (msgsnd(qid, &message, msglen, 0) == -1)
        {
            syserr("msgsnd");
        }

        exit(EXIT_SUCCESS);
    }

    pid_t pid2;
    if ((pid2 = fork()) == -1)
        syserr("fork2");
    if (pid2 == 0)
    {
        // FILE *fp; // második fiú fp
        // // második fiú, olvas sorból és ír a fájlba

        while (1)
        {
            msglen = msgrcv(qid, &message, MAXLEN, 1, 0);
            if (msglen == -1)
            {
                syserr("msgrcv");
            }
            if (strcmp(message.buff, "exit\n") == 0)
            {
                break;
            }
            printf("%s", message.buff);
            fflush(stdout);
        }
        exit(EXIT_SUCCESS);
    }

    // megvarjuk a folyamatokat
    wait(NULL);
    wait(NULL);

    if (msgctl(qid, IPC_RMID, NULL) == -1)
        syserr("msgctl");
    exit(EXIT_SUCCESS);
}