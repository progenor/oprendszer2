// az író folyamat

#include "myinclude.h"
#include "shm.h"

//két szemafort használunk
//kezdőérték s0=1, s1=0
//az író folyamat down: -1->s0
//                up:   +1->s1

int main( int argc, char * argv[])
{
    int semid, shmid;
    char * mem;    //ide csatoljuk

    //TODO: szemafor műveletek definiálása
                  
    //TODO: szemafor tömb létrehozás

    //TODO: szemafor inicializálás

    //TODO: osztott memória id lekérése

    //TODO: osztott memória felcsatolása

    while(1){

    //TODO:
    // 1. down művelet
    // 2. 1 sor olvasása stdin->mem, max 1024 byte!
    // 3. up művelet
    //ha  a sor "exit" akkor break
    }
    //osztott memória lecsatolás


    exit(EXIT_SUCCESS);
}