#include "myinclude.h"

 #define MAX_ARGS 128 //max 128 argumentummal mukodik

//a parancssort feldolgozó függvény
//bemenet: parancssor sztring, pl:
//   ls -l /home
//kimenet: ha minden rendben, lefut az execvp a parancssorral
//         és nem tér vissza
//         -1 hibákód, ha nem hajtódik végre a parancs   
int parancssor ( char * parancs)
{
    printf("%s\n", parancs);    
    char * argv[MAX_ARGS] ; //ebben lesz az argv
    char * s;
    int argc = 0; 
    char ** ptr = argv;   //mutató az argv-re

    s = strtok(parancs, " "); //elso sztring

    while ( s != NULL ){
        *ptr++ = s;            //mutato bekerul az argv-be
                               //s sztring marad a helyen
        s = strtok(NULL, " "); //kovetkezo sztringek
        
        //ellenorzes, hogy ne lepjem tul a MAX_ARGS-ot
        if (++argc >= MAX_ARGS){
            fprintf(stderr, "túl sok argumentum\n");
            return 1;      
        }
    }
    *ptr = NULL ; //argv tömb lezárása
    // itt megvan az argv 
    execvp (argv[0], argv);
    // ide akkor jutunk ha nem sikerul 
    return EXIT_FAILURE;
} 
