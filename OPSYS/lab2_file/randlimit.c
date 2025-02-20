#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

// egy pseudo random egész számot generál, amely a [0,max)
// intervallumban van

int randlimit(int max) {
    int l, d, x, r;

    // kiválasztunk egy határt a random függvény eredmény-nek:
    // ez a max-nak az a legnagyobb többszöröse, amely
    // kisebb mint RAND_MAX

    l = RAND_MAX - (RAND_MAX % max);
    // l a max legnagyobb többszöröse, < RAND_MAX
    d = l / max;

    // behívunk egy random számot
    // addig hívjuk a függvényt amíg az eredmény a [0,l) intervallumban
    // lesz. Ekkor ezt d - vél osztva, egy [0,max) intervallumbéli
    // egyenletesen eloszló random számot kapunk

    do {
        x = rand();

    } while (x >= l);

    r = (int)(x / d);

    // printf("rand call: %d from pid: %d\n",r,getpid());

    return r;
}
