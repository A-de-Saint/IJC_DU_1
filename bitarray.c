#include "bitarray.h"

#ifdef USE_INLINE
inline void bitarray_free(bitarray_t jmeno_pole)
{
    //jestlize je NULL, neuvolnovat
    if (jmeno_pole)
        free(jmeno_pole);
}

//vrati velikost bitarray v bitech
inline unsigned long bitarray_size(bitarray_t jmeno_pole)
{
    return jmeno_pole[0];
}

//vyplni bitarray 0 (false), nebo 1 (true)
inline void bitarray_fill(bitarray_t jmeno_pole, bool bool_výraz)
{
    //velikost jednoho elementu pole v bitech
    unsigned long el_size = bit_size;
    bitarray_element filler;

    if (bool_vyraz)
        filler = 0;
    else
        filler = ~(bitarray_element)0; //bitwise negace bitarray_elementove nuly

    for (int i = 1; i * el_size < jmeno_pole[0]; i++)
    {
        jmeno_pole[i] = filler;
    }
}

//nastavi bit na indexu na 0(false) nebo 1 (true)
inline void bitarray_setbit(bitarray_t jmeno_pole, unsigned long index, bool bool_výraz)
{
    #ifndef NO_CHECK
    if (index >= jmeno_pole[0])
        error_exit("bitarray_setbit: Index %lu mimo rozsah 0..%lu", index, jmeno_pole[0] - 1);
    #endif

    //velikost elementu pole v bitech
    unsigned long el_size = bit_size;
    
    //index elementu v poli, ktery se zmeni
    unsigned long el_idx = index / el_size + 1; //+1 kvuli velikosti na nultem indexu

    //index bitu (default pro little endian)
    unsigned long b_idx = (el_size - 1) - index % el_size;

    if (bool_vyraz)
        jmeno_pole[el_idx] |= ((bitarray_element)1) << b_idx;
    else
        jmeno_pole[el_idx] &= ((bitarray_element)1) << b_idx;
}

//vrati hodnotu bitu na danem indexu
inline int bitarray_getbit(bitarray_t jmeno_pole, unsigned long index)
{
    #ifndef NO_CHECK
    if (index >= jmeno_pole[0])
        error_exit("bitarray_getbit: Index %lu mimo rozsah 0..%lu", index, jmeno_pole[0] - 1);
    #endif
    
    unsigned long el_size = bit_size;
    unsigned long el_idx = index / el_size + 1;
    unsigned long b_idx = (el_size - 1) - index % el_size;

    //posun jednicky na bitovy index elementu
    bitarray_element changer = ((bitarray_element)1) << b_idx;

    //vysledek bude 0 pokud bit == 0, jinak bude 2^b_idx
    if (jmeno_pole[el_idx] & changer)
        return 1;
    return 0;
}

#endif