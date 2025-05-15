// olvasó folyamat

#include "myinclude.h"
#include "shm.h"

// két szemafort használunk
// kezdőérték s0=1, s1=0
// az olvasó folyamat down: -1->s1
//                    up:   +1->s0

int main(int argc, char *argv[])
{
    int semid, shmid;
    char *mem; // ide csatoljuk

    // TODO: szemafor műveletek definiálása
    struct sembuf downf = {1, -1, 0}; // down művelet
    struct sembuf upf = {0, +1, 0};   // up művelet

    // TODO: szemafor tömb létrehozás

    if ((semid = semget(KEY, 2, 0)) == -1)
    {
        syserr("semget");
    }

    // TODO: osztott memória id lekérése

    if ((shmid = shmget(KEY, SIZE, 0)) == -1)
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
        if (semop(semid, &downf, 1) == -1)
            syserr("semop down");

        //  2. 1 sor olvasása és kiírása: mem->stdout
        printf("%s", mem);
        // 3. up művelet
        if (semop(semid, &upf, 1) == -1)
            syserr("semop up");
        // ha  a sor "exit" akkor break
        if (strcmp(mem, "exit\n") == 0)
            break;
    }

    // TODO: osztott memória lecsatolás
    if (shmdt((void *)mem) < 0)
        syserr("shmdt");

    // TODO: szemafor tömb és osztott memória id törlése
    if (semctl(semid, 0, IPC_RMID) < 0)
        syserr("ctl1");
    if (shmctl(shmid, IPC_RMID, 0) < 0)
        syserr("ctl2");

    exit(EXIT_SUCCESS);
}