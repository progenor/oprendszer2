/*
  feladat 1 sablon f1.c
*/

#include "myinclude.h"
#include "f12.h"

int main(int argc, char *argv[])
{
    int semid;     //szemafor tömb
    int fid;       //fájl
    char a = 'a';  //ezt írjuk a fájlba
    int count = N; //számláló

    //az f1 hozza létre és inicializálja  a szemaforokat
    
    //TODO:az init a kezdő értékeket tartalmazza
    //short init[2] = ... 

    //TODO: a két művelet f1 számára
    //struct sembuf ... 
    //struct sembuf ...

    //TODO: szemafor tömb lekérés, hozza létre ha nem létezik

    //TODO: szemafor tömb inicializálás

    //TODO: fájl megnyitása írásra, O_WRONLY | O_TRUNC | O_CREAT | O_APPEND

    //íro ciklus
    while (count--)
    {
        //TODO: kritikus szakaszba lép

        //TODO: lseek

        //TODO: ír

        //kilép a kritikus szakaszból
    }

    //zárja a fájlt
    close(fid);

    exit(0);
}