#include <stdio.h>
#include "bitarray.h"
#include <time.h>

void Eratosthenes(bitarray_t pole);

int main(void)
{
    //zacatek behu programu
    clock_t start = clock();

    bitarray_create(pole, 444000000);
    Eratosthenes(pole);

    //ulozi posledni 4 prvocisla
    unsigned long last_primes[10];
    unsigned long last_idx = bitarray_size(pole) - 1;
    unsigned long primes_count = 0;
    for (; primes_count < 10; last_idx--)
    {
        if (last_idx == 0)
            return 1;
        if (bitarray_getbit(pole, last_idx) == 1)
            last_primes[primes_count++] = last_idx;
    }

    for (; primes_count > 0; primes_count--)
        printf("%lu\n", last_primes[primes_count-1]);

    fprintf(stderr, "Time=%.3g\n", (double)(clock()-start)/CLOCKS_PER_SEC);
}