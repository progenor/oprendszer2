/*
 * az stdin másolása az stdout-ra első
 * szintű read és write-al
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 128  // puffer méret

int main(int argc, char* argv[]) {
    char buff[N];
    int n;

    while ((n = read(STDIN_FILENO, buff, N)) > 0) {  // a STDIN_FILENO értéke 0
        write(STDOUT_FILENO, buff, n);               // a STDOUT_FILENO értéke 1
    }
    exit(0);
}
