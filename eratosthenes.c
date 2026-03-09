// eratosthenes.c
// Řešení příkladu IJC-DU1, příklad a), 7.3.2026
// Autor: Antonín Hrnčíř, FIT
// Přeloženo: gcc 15.2.1
/* Implementace eratosthenova síta pro nalezení prvočísel
    - Nuluje vsechny neprvociselne indexy v bitovem poli */

#include "bitarray.h"
#include <math.h>

//nastavi prvociselne indexy bitoveho pole na 1 (in-situ)
void Eratosthenes(bitarray_t pole)
{
    //nastavit vse na 1, pote prvni dva prvky na 0
    bitarray_fill(pole, true);
    if (bitarray_size(pole) > 0)
        bitarray_setbit(pole, 0, false);
    if (bitarray_size(pole) > 1)
        bitarray_setbit(pole, 1, false);
    
    unsigned long idx = 2UL;
    unsigned long limit = (unsigned long)(sqrt((double)bitarray_size(pole)));
    for (; idx <= limit; idx++)
    {
        if (bitarray_getbit(pole, idx) == 0)
            continue;
        
        //nuluje vsechny nasobky idx, ktery ma hodnotu bitu 1
        unsigned long next_idx = idx;
        for (; next_idx < bitarray_size(pole); next_idx += idx)
            bitarray_setbit(pole, next_idx, false);
    }
}