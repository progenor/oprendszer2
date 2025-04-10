#include "myinclude.h"

#define MAXBUFF 80

//#define TEXTFILE

#ifdef TEXTFILE
//szoveges fájlokra fdopen_el
//
void fiu_feladat (int pfd0, char * nev)
{
    FILE * fpbe, * fpki ; char buff[80]; 
    if ((fpbe=fdopen(pfd0,"r"))==NULL){
        syserr("fdopen2");
    }
    if ((fpki=fopen(nev,"w"))==NULL){
        syserr(nev);
    }
    while (fgets(buff, MAXBUFF,fpbe)!=NULL){
        if(fputs(buff,fpki)==EOF){
            syserr("fputs2");
        }
    } 
    if(ferror(fpbe)){
        syserr("ferr");
    }
    fclose(fpbe);fclose(fpki);
}

void apa_feladat (int pfd1, char * nev)
{
    FILE * fpbe, *fpki ; char buff[80]; 
    if ( (fpki=fdopen(pfd1,"w"))==NULL){
        syserr("fdopen1");
    }
    if ( (fpbe=fopen(nev,"r"))==NULL){
        syserr(nev);
    }
    while (fgets(buff,MAXBUFF,fpbe)!=NULL){
        if (fputs(buff,fpki)==EOF){
            syserr("fputs1");
        }
    }    
    if(ferror(fpbe)){
        syserr(nev);   
    }
    fclose(fpbe);fclose(fpki);
}

#else
//binaris fájlokra
void fiu_feladat (int pfd0, char * nev)
{
    int fd,n; char buff[80]; int count;
    if ( (fd=open(nev,O_WRONLY|O_TRUNC|O_CREAT,0644))<0){
        syserr(nev);
    }
    count=0;
    while ( (n=read(pfd0, buff,MAXBUFF))>0){
        if(write(fd,buff,n)<0){
            syserr("write2");
        }
        count+=n;
    } 
    if(n<0) syserr("read2");
    close(pfd0); close(fd);
    printf("masolás: %d\n",count);
}

void apa_feladat (int pfd1, char * nev)
{
    int fd,n; char buff[80]; 
    if ( (fd=open(nev,O_RDONLY))<0){
        syserr("open1");
    }

    while ( (n=read(fd,buff,MAXBUFF))>0){
        if(write(pfd1,buff,n)<0){
            syserr("write1");
        }
    }
    if(n<0){
        syserr("read1");
    }

    close(pfd1); close(fd);
}

#endif

int main(int argc, char * argv[])
{
    pid_t pid;  
    int pfd[2];
    int status;

    if (argc!=3){
        printf("2 fájlnév!\n"); exit(1);
    }
    if (!strcmp(argv[1],argv[2])){
        printf("a két név ugyanaz!\n"); exit(1);
    }


    if (pipe(pfd)<0) syserr("pipe");

    if ( (pid=fork()) < 0) syserr("fork");

    if (pid==0){
        close(pfd[1]);
        fiu_feladat (pfd[0],argv[2]); 
        exit(0);
    }
    //apa
    close(pfd[0]);
    apa_feladat (pfd[1],argv[1]); 
    wait(&status);

    if (WIFEXITED(status) && WEXITSTATUS(status)==0){
        printf("exit fiú ok\n");
    
        
        if ( (pid=fork()) < 0) syserr("fork");

        if (pid==0){
            execlp("cmp","cmp",argv[1],argv[2],NULL);
           syserr("execlp");
        }     
        wait(&status);

         if (WIFEXITED(status) && WEXITSTATUS(status)==0){
            printf("a két fájl egyforma\n"); 
        }else
        {
            printf("összehasonlítás nem sikerült\n"); 
        }
        
    
    }

    exit(0);

}
