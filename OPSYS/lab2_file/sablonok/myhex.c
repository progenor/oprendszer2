/*
 * fájl nyomtatása hex formátumban
 */

#include "myinclude.h"
#include <ctype.h>

#define BUFFSIZE 8

int main(int argc, char * argv[])
{

    char buff[BUFFSIZE]; //puffer, egy ciklusban 8 karaktert olvasunk
    int fd;             //fájl azonosító
    int n;              //a read() visszatérített értékének tárolása
    int k;              //puffer index
    int count =0 ;

    //ha nincs egy argumentum
    if (argc<2) {
        fprintf(stderr,"használat: %s fájlnév\n", argv[0]); 
        exit(EXIT_FAILURE);
    }

    //TODO : nyissuk meg csak olvasásra a fájlt
    //megkapjuk az fd-t, 
    fd = 
    
    
    //TODO: teszteljük, hogy nincs hiba, ha van kilépünk


    //TODO: olvassunk, amíg a read() által visszatérített érték > 0
    while ( ){

        //kiírjuk a sor első karakterének sorszámát
        printf("%06x ",count);

        //TODO: kiírjuk a hexa kódokat, egy k indexel járjuk végig a puffert
        // ezt a printf-et használjuk:    printf ("%02x ", buff[k] & 0xff);
        // a %x 16 bites előjel nélküli egészet vár, 
        // ha a karakter felső bitje 1-es, negatív számokat fog kiírni


        //TODO: kiírjuk a nyomtatható karaktereket
        //    ha: isprint(buff[k])) igaz kiírjuk, ha nem egy . karaktert írunk
        // lásd:  man isprint

        printf("\n");
        count+=BUFFSIZE; //következő sor
    }

    if (close(fd)<0) 
        syserr("close"); //zárjuk a fájlt

    exit(EXIT_SUCCESS);

}

