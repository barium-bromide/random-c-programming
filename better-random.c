// ensure pseudo randomness to nanoseconds

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int btr_rand()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    srand(ts.tv_nsec);
    return rand();
}

int main()
{
    int number1 = btr_rand(); // 0-32767
    printf("%d\n", number1);
    return 0;
}