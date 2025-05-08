/*
  feladat 1 sablon f2.c
*/
#include "myinclude.h"
#include "f12.h"

int main( int argc, char * argv[])
{
    int semid; //szemafor tömb 

    //TODO:a két szemafor művelet f2 számára
    //struct sembuf ... 
    //struct sembuf ...

    //TODO: a szemafor tömb lekérése, adjon hibát ha nem létezik
    //az f1 által létrehozva

    //TODO: fájl nyitása írásra, adjon hibát ha nem létezik
    //ne csonkolja
    int fid;  //fájl

    //íro ciklus
    int count = N; //számláló
    while (count--)
    {
        //TODO: kritikus szakaszba lép

        //TODO: lseek ha nincs O_APPEND nyitáskor

        //TODO: ír
        char c = 'b';  //ezt írjuk a fájlba

        //TODO:kilép a kritikus szakaszból

    }

    //TODO: törli a szemafor tömböt
    //ez a folyamat használja utoljára

    //zárja a fájlt
    close(fid);
    exit(EXIT_SUCCESS);

} 