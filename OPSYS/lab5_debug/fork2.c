/* 
 *  a fork fuggveny kezelese - apa es fiu folyamat
 *  kulon kod reszeken futnak
 */

#include "myinclude.h"

int main() {
    printf ("\n");
    pid_t fiu_pid ; //az apa es fiu pid szamat itt taroljuk
    pid_t apa_pid ;

    /* a kimenet puffereleset megszuntetjuk, igy a printf
       fuggveny azonnal kiir miutan meghivodik
       */
    setbuf (stdout,NULL);

    apa_pid = getpid(); //apa pid szama      

    fiu_pid = fork();           //folyamat inditas

    if ( fiu_pid < 0 ) {                  //hibakezeles
        syserr("fork");
    } 

    if ( fiu_pid == 0) {
        // fiu kod resze 
        printf ("Fiú: apa pid %d, fiú pid %d\n", (int) apa_pid, (int) getpid());

        printf("fiú vár\n"); 
        sleep(4);                          // fiu var 4 masodpercet

        printf("Fiú: kilép\n");
        exit(0);
    } else {    
        /* apa kod resze */     
        //     sleep(2);
        printf("Apa: apa pid %d, fiú pid %d\n", (int) apa_pid, (int) fiu_pid);

        wait(NULL);      //megvarja a fiut, ha nem kell a kilepesi ertek
        //akkor NULL a parameter 

        printf("Apa: kilép\n");
        exit(0);
    }    

    printf("ide nem érnek el soha!\n");
    return 0;
}

