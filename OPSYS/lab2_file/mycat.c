/* 
 * egyszerű cat program
 *
 * használat:
 *    mycat [ fájl ] ...
 * ha nem adunk még fájlnevet, akkor a standard bemenetét másolja
 *
 * a két eset van: 1. van fájlnév  2. nincs fájlnév 
 * ez egy if két ágában van megoldva

 * feladat:
 * írjuk át a programot úgy, hogy a másolást a két esetben ugyanaz a függvény végezze!
 */

#include "myinclude.h"
#define LEN 1024  //olvasási puffer hossz

int main (int argc, char * argv[])
{
    char buff[LEN];//puffer az olvasáshoz
    int n;
    int i;
    int fd; //fájl azonosító

    if (argc==1) {      // ha nincs paraméter a stdin-t másoljuk 

        //a read a terminálról való olvasásnál akkor jön vissza
        //ha Enter-t kap
        //ha a terminálon több karaktert ütünk be mint LEN , akkor
        //egymás után több read fut le, 
        //és a több karakter a terminál driver-ben pufferelődik 
        //egy reád maximum LÉN karaktert ad vissza
        while ( (n = read (STDIN_FILENO, buff, LEN))>0 ){
            if (write ( STDOUT_FILENO , buff, n)<0){
                perror("write stdout hiba");
                exit(EXIT_FAILURE);
            }
        }
        if (n<0) { //ha n<0 hiba történt
            perror("olvasási hiba");
            exit(EXIT_FAILURE);
        }
    }else
    {   /* ha vannak paraméterek a fájlokat másoljuk */
        /* az argc pont a maximális indexet jelenti a paraméter listában */
        /* i=1 indexen van az első fájlnév, az argv[i] helyen  */ 	
        /* mert az argv[0] a program nevet tartalmazza */
        for (i=1; i<argc; i++) { /* amíg van még paraméter */
            /* nyitás csak olvasásra */
            if ( ( fd = open (argv[i], O_RDONLY )) < 0) { 
                syserr("open");
            }
            /* másolás */
            while ( (n = read (fd, buff, LEN))>0 ) {
                if (write ( STDOUT_FILENO , buff, n)<0){ //n karakter írása az stdout-ra
                    perror("write");
                    exit(EXIT_FAILURE);
                }
            }
            if (n<0) {
                perror("olvasasi hiba");
                exit(EXIT_FAILURE);
            }
            if (close(fd)<0){ //zárjuk 
                perror("close");
                exit(EXIT_FAILURE);
            }
        }
    }					
    exit(EXIT_SUCCESS);					
}
