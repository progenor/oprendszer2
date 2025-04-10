/* 
   5. feladat sablon
   2 fiú szinkronizálása pipe-al

   TODO: a szinkronizálás meg van írva, írjuk meg a feladat
   alábbi követelményét:
  - apa létrehoz egy fájlt amíg a fiúk várnak, benne "Helló" szöveg
  - fiúk kiírják a terminálra a fájlt miután újraindulnak

  -tesztelés:
     $ gcc -Wall fel5.c -o fel5
     $ ./fel5

*/

#include "myinclude.h"

#define BUFSIZE 80

//fiúk olvasása
void read_hello(char * name)
{
    int fd, n;
    char * buff[BUFSIZE]; //olvasáshoz a fájlból
    if ((fd = open(name, O_RDONLY))<0) syserr("open");
    if ((n=read(fd, buff, BUFSIZE))<0) syserr("read");
    //kiírjuk

    if (write(STDOUT_FILENO, buff, n)<0) syserr("stdout");
    close(fd);
}

int main(int argc, char *argv[])
{
  int pfd[2]; //pipe azonosítók
  pid_t p1, p2;
  char b; //ezt használjuk puffernek a pipe esetén

  char name[] = "hello.txt"; // afájl neve
  char msg[] = "Helló!\n"; //az átírt üzenet

  setbuf(stdout, NULL); //pufferelés tiltása

  //cső
  if (pipe(pfd) < 0) syserr("pipe");

  //első fiú
  if ((p1 = fork()) < 0) syserr("p1:");

  //első fiú folyamat
  if (p1 == 0)
  {
    //olvasni fog a csőből
    if (close(pfd[1])<0) syserr("close");

    printf("fiú 1 vár\n");
    //próbál egy byte-ot olvasni, blokkolódik
    if (read(pfd[0], &b, 1) < 0) syserr("read2");

    printf("fiú 1 fut\n");
    //fájl kiolvasása  
    read_hello(name);

    close(pfd[0]);
    exit(EXIT_SUCCESS);
  }

  //apa folytatja, 2. fiú létrehozása
  if ((p2 = fork()) < 0)
    syserr("p2:");

  //fiú 2
  if (p2 == 0)
  {
    //olvasni fog
    if (close(pfd[1])<0) syserr("close");
    
    printf("fiú 2 vár\n");
    //olvasni próbál, blokkolódik
    if (read(pfd[0], &b, 1) < 0) syserr("read1");
    
    printf("fiú 2 fut\n");
    //fájl kiolvasása  
    read_hello(name);
    
    close(pfd[0]);
    exit(0);
  }

  //apa dolgozik

  //írni fog a csőbe
  if (close(pfd[0])<0) syserr("close");

  //apa
  printf("apa dolgozik\n");

  //fájl létrehozása
  int fd;
  if ((fd=open(name, O_WRONLY|O_CREAT|O_TRUNC, 0664))<0) syserr("open");
  if (write(fd, msg, strlen(msg))<0) syserr("msg");
  if (close(fd)<0) syserr("close");

  sleep(3);
  printf("apa befejezte\n");

  //ha lezárja a csövet, a két read visszatér fájl vége 0-val
  //és a két fiú folyamat újra indul
  //ha írtunk volna 2 byte-ot és utána zárjuk a csővet ugynaz történik
  close(pfd[1]);

  //megvárja a fiait
  wait(NULL);
  wait(NULL);
  exit(EXIT_SUCCESS);
}
