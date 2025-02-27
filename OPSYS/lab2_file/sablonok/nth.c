/*
 * FELADAT: N. karakter kiírása egy fájlból
 */ 

#include "myinclude.h"

#include <ctype.h>

int main ( int argc, char * argv[])
{

    //TODO: mennyi az argc?
    if (argc != 3 ) {
        printf ("használat: %s fájlnév N\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //TODO: alakítsuk át egész számmá az argv[2]-t
    //minta a feladat szövegében
    //ellenőrizzük, hogy jó a konverzió
    long n; //n fogja tárolni a számot

    n =atol(argv[2]); 

    //a karaktereket 1-től indexeljük a fájlban
    if ( n < 1 ) {
        printf ("nem jó az N értéke, N > 0\n"); 
        exit(EXIT_FAILURE);
    }

    //TODO: nyissuk meg a fájlt olvasásra, a neve argv[1]-ben 
    //ha nem sikerül hiba és kilépés
    int fd ;// ide kerÜl az azonosító
    if ((fd = open(argv[1], O_RDONLY))<0) syserr("open");

    //?elég hosszú a fájl?
    //az offset-et a fájl végére viszi, ez a fájl hossza byte-ban
    long off = lseek ( fd, 0, SEEK_END );
    if( n < 0) {
        perror("argument less than0");
        exit(EXIT_FAILURE);
    };
    if (n > off) {
        printf ("n to big ");
        exit(EXIT_FAILURE);
    }

    //TODO: vigyük az offset mutatót az N. karakterre a fájlban
    //az offset 0-tól, mi pedig 1-től indexeljük őket 
    //ha nem sikerül, hiba és kilépés
    if (lseek(fd, n-1, SEEK_SET)<0) syserr("lseek");
    char b;
    //TODO: olvassunk egy byte-ot az &b címre
    if (read(fd, (void*)&b,1)<0) syserr("read");

    if (isprint(b)){
         printf ("%c\n", b);
    }else{
         printf ("nem nyomtatható %x\n", b & 0xff);

    }

    exit(EXIT_SUCCESS);

}
