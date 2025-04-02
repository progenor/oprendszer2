
#define MYINCLUDE

#include <sys/stat.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stddef.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

extern int errno; /* az errno.h által visszaadott hibakód */

// rendszer hiba kiíró makró
#define syserr(msg)                                             \
  do                                                            \
  {                                                             \
    fprintf(stderr, "hiba: %s %d. sor:\n", __FILE__, __LINE__); \
    perror(msg);                                                \
    exit(EXIT_FAILURE);                                         \
  } while (0)
