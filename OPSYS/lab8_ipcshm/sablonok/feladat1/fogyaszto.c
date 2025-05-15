//olvasó folyamat

#include "myinclude.h"
#include "shm.h"

//két szemafort használunk
//kezdőérték s0=1, s1=0
//az olvasó folyamat down: -1->s1
//                   up:   +1->s0

int main( int argc, char * argv[])
{
    int semid, shmid;
    char * mem;    //ide csatoljuk

    //TODO: szemafor műveletek definiálása
                  
    //TODO: szemafor tömb lekérése

    //TODO: osztott memória id lekérése


    //TODO: osztott memória felcsatolása

    while(1) {

    //TODO:
    // 1. down művelet
    // 2. 1 sor olvasása és kiírása: mem->stdout 
    printf("%s",mem);
    // 3. up művelet
    //ha  a sor "exit" akkor break

    }    

    //TODO: osztott memória lecsatolás

    //TODO: szemafor tömb és osztott memória id törlése
    if(semctl(semid,0,IPC_RMID)<0) syserr("ctl1"); 
    if(shmctl(shmid,IPC_RMID,0)<0) syserr("ctl2"); 



    exit(EXIT_SUCCESS);
}