/*
 * FELADAT: a write() és open() függvények tanulmányozása
 * a programnak két paramétere van, egy fájlnév és egy sztring
 *
 * futtatás:
 *    ./writefile a.txt alma
 *
 * különböző módokat használva a feladat megoldása során
 * beírja az "alma" sztringet az a.txt fájlba   
 * 
*/

#include "myinclude.h"
#include <ctype.h>
#include <string.h>

int main(int argc, char * argv[])
{

    char * buff; //ezt a sztringre tesszük
    int fd; //fájl azonosító
    int n;  //a write() által visszatérített érték
    int len;

    //TODO: mennyi az argc?
    if (argc!= ... ) {
        printf("használat: %s fájlnév sztring\n", argv[0]); 
        exit(EXIT_FAILURE);
    }

    buff = argv[2]; //áttesszük a sztring címét
    //TODO: c) pont, '\0' cseréje

    //TODO: nyissuk meg a fájlt a feladat által kért módban 
    fd = ...

    //ellenőrizzük, hogy sikerült-e
    if (fd<0){
        syserr("open");
    }

    //TODO: írjuk be a sztringet a fájlba
    n = ...

    //TODO: ellenőrizzük, hogy sikerült-e, ha nem, akkor hiba és kilépés 
    if (n<0){
        syserr("write");
    }

    if (close(fd)<0){
        syserr("close");
    }

    exit(EXIT_SUCCESS);
}

