#include <sys/unistd.h>  
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>  
#include <errno.h>
 
extern int errno ; /* az errno.h által visszaadott hibakód */

//makróval ugyanaz:
#define syserr(msg)   \
  do { \
       fprintf(stderr, "hiba: %s %d. sor:\n", __FILE__, __LINE__); \
       perror ( msg ); \
       exit (EXIT_FAILURE); \
     } while(0) 

