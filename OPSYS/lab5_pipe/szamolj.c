#include "myinclude.h"

int main(){
    double num;
    int pfd[2];
    if(pipe(pfd) < 0){
        syserr("pipe");
    }
    pid_t pid;
    if((pid = fork()) < 0){
        syserr("fork");
    }
    if(pid == 0){
        close(pfd[0]);
        if(dup2(pfd[1], STDIN_FILENO) == -1){
            syserr("dup2");
        }
        if(pfd[1] = write(pfd[1], (void *)&num, sizeof(num)) < 0){
            syserr("write");
        }

    }else{
        close(pfd[1]);
        while(read(pfd[0], (void *)&num, sizeof(num)) > 0){
            printf("%f\n", num);
        }

        wait(NULL);
    }
    exit(EXIT_SUCCESS);


}
