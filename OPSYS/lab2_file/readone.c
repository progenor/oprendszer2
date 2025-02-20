/*
 * az stdin másolása az stdout-ra első
 * szintű read és write-al
 * karakterenként
 */

#include "myinclude.h"

int main(int argc, char* argv[]) {
    char buff;
    int n;

    if ((n = read(STDIN_FILENO, &buff, 1)) < 0) {
        syserr("read");
    }
    exit(EXIT_SUCCESS);
}
