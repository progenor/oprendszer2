#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/wait.h>

extern int errno; /* az errno.h által visszaadott hibakód */

// rendszer hiba kiíró makró
// egy sztringet kap bemenetként, kiírja a sztringet és
// hozzáfűzi a rendszer hiba üzenetét:
//    msg : rendszerhiba
//    alakban

#define syserr(msg)                                                       \
    do {                                                                  \
        fprintf(stderr, "hiba: fájl: %s sor: %d.\n", __FILE__, __LINE__); \
        perror(msg);                                                      \
        exit(EXIT_FAILURE);                                               \
    } while (0)
