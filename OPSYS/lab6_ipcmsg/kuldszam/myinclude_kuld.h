
#define MYINCLUDE

#define _XOPEN_SOURCE 501
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/wait.h>
#include <time.h>

extern int errno; /* az errno.h által visszadott hibakód */

// rendszer hiba kiíró függvény (makró)
// egy sztringet kap bemenetként, kiírja a sztringet és
// hozzáfűzi a rendszer hiba üzenetét:
//    msg : rendszerhiba
//    alakban

#define syserr(msg)                                                 \
    do {                                                            \
        fprintf(stderr, "hiba: %s %d. sor:\n", __FILE__, __LINE__); \
        perror(msg);                                                \
        exit(EXIT_FAILURE);                                         \
    } while (0)

// define a ket programnak
#define KEY1 11
#define BUFSIZE 80

struct mb {  // uzenetsor puffer
    long mtype;
    char b[BUFSIZE];
} mbuff;