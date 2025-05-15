// az író folyamat

#include "myinclude.h"
#include "shm.h"

// két szemafort használunk
// kezdőérték s0=1, s1=0
// az író folyamat down: -1->s0
//                 up:   +1->s1

int main(int argc, char *argv[])
{
    int semid, shmid;
    char *mem; // ide csatoljuk

    // TODO: szemafor műveletek definiálása

    struct sembuf down = {0, -1, 0}; // down művelet
    struct sembuf up = {1, +1, 0};   // up művelet
    short int init[] = {1, 0};       // kezdeti értékek
    // TODO: szemafor tömb létrehozás

    if ((semid = semget(KEY, 2, IPC_CREAT | 0660)) == -1)
    {
        syserr("semget");
    }

    // TODO: szemafor inicializálás
    if (semctl(semid, 0, SETALL, init) == -1)
    {
        syserr("semctl");
    }

    // TODO: osztott memória id lekérése

    if ((shmid = shmget(KEY, SIZE, IPC_CREAT | 0660)) == -1)
    {
        syserr("shmget");
    }

    // TODO: osztott memória felcsatolása

    if ((mem = (char *)shmat(shmid, NULL, 0)) == (char *)-1)
    {
        syserr("shmat");
    }

    while (1)
    {

        // TODO:
        //  1. down művelet
        if (semop(semid, &down, 1) == -1)
        {
            syserr("semop down");
        }
        //  2. 1 sor olvasása stdin->mem, max 1024 byte!
        if (fgets(mem, SIZE, stdin) == NULL)
        {
            strcpy(mem, "exit\n");
            // printf("End of file\n");

            // ha kiugrunk akkor ezt nem csinaljuk meg
            if (semop(semid, &up, 1) == -1)
            {
                syserr("semop up");
            }
            break;
        }
        //  3. up művelet
        if (semop(semid, &up, 1) == -1)
        {
            syserr("semop up");
        }
        // ha  a sor "exit" akkor break
    }
    // osztott memória lecsatolás
    if (shmdt(mem) == -1)
    {
        syserr("shmdt");
    }

    exit(EXIT_SUCCESS);
}