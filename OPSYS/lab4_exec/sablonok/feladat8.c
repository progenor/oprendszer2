/* 
 * sablon az 1. feladathoz
 *
 */

#include "../myinclude.h"
#include "../parancssor.h"

int main (int argc, char * argv[])
{
    pid_t pid;   //folyamat azonosito
    int status;  //kilepesi allapot tarolasara

    pid = fork();  //uj folyamat
    if ( pid < 0){
        syserr("fork");
    }

    if ( pid == 0){

        //fiu folyamat
        execlp("sort", "sort", "-nr", "../szam.txt", "-o", "./ki.txt", (char *)NULL);
    
        syserr("execlp");
    } else
    {  
        //apa folyamat megvarja a fiut 
        wait (&status);
        if(WIFEXITED(status) == 0){
            // syserr("WIFEXITED");
            printf("WIFEXITED sad\n");
            exit(1);
        }
        else{
            execlp("tail", "tail", "-1", "./ki.txt", (char *)NULL);
            syserr("execlp");
        }
   }
    exit(EXIT_SUCCESS);
}
