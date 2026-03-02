#ifndef BITARRAY_H
#define BITARRAY_H
    
    #include <stdlib.h> //calloc a free
    #include <stdbool.h> //true a false
    #include <limits.h> //CHAR_BIT
    #include "error.h" //error_exit

    #define bitarray_element unsigned long

    #define bit_size (unsigned long)sizeof(bitarray_element) * CHAR_BIT

    //typ bitoveho pole
    typedef bitarray_element* bitarray_t;

    //Typ indexu bitoveho pole
    typedef unsigned long bitarray_index_t;

    //vytvori bitove pole dane velikosti
    //(real_size reprezentuje pocet bitarray_elementu v poli)
    #define bitarray_create(jmeno_pole, velikost)                                           \
    do {                                                                                    \
        unsigned long real_size = velikost / (bit_size) + 1;                                \
        if (velikost % (bit_size))                                                          \
            real_size++;                                                                    \
        bitarray_element jmeno_pole[real_size] = {0};                                       \
        jmeno_pole[0] = velikost;                                                           \
    } while (0);

    //alokuje a nuluje pole dane velikosti
    //(real_size reprezentuje pocet bitarray_elementu v poli)
    #define bitarray_alloc(jmeno_pole, velikost)                                            \
    do {                                                                                    \
        unsigned long real_size = velikost / (bit_size) + 1;                                \
        if (velikost % (sizeof(bitarray_elment) * CHAR_BIT))                                \
            real_size++;                                                                    \
        bitarray_element *jmeno_pole = calloc(real_size * sizeof(bitarray_element));        \
        if (!jmeno_pole)                                                                    \
        {                                                                                   \
            fprintf(stderr, "bitarray_alloc: Chyba alokace paměti\n");                      \
            exit(1);                                                                        \
        }                                                                                   \
        jmeno_pole[0] = velikost;                                                           \
    } while (0);

    #ifdef USE_INLINE
        inline void bitarray_free(bitarray_t jmeno_pole);
        
        inline unsigned long bitarray_size(bitarray_t jmeno_pole);

        inline void bitarray_fill(bitarray_t jmeno_pole, bool bool_výraz);

        inline void bitarray_setbit(bitarray_t jmeno_pole, unsigned long index, bool bool_výraz);

        inline int bitarray_getbit(bitarray_t jmeno_pole, unsigned long index);
    #else

        #define bitarray_free(jmeno_pole)   \
        do {                                \
            if (jmeno_pole)                 \
                free(jmeno_pole);           \
        } while(0);

        #define bitarray_size(jmeno_pole)   \
            jmeno_pole[0]

        #define bitarray_fill(jmeno_pole, bool_výraz)                       \
        do {                                                                \
            if (bool_výraz)                                                 \
            {                                                               \
                for (unsigned long i = 1; i * bit_size < jmeno_pole[0]; i++)\
                {                                                           \
                    jmeno_pole[i] = 0;                                      \
                }                                                           \
            }                                                               \
            else                                                            \
            {                                                               \
                for (unsigned long i = 1; i * bit_size < jmeno_pole[0]; i++)\
                {                                                           \
                    jmeno_pole[i] = ~(bitarray_element)0;                   \
                }                                                           \
            }                                                               \
        } while(0);

        #ifdef NO_CHECK
            #define bitarray_setbit(jmeno_pole, index, bool_výraz)
            do {
                unsigned long el_idx = (unsigned long)index / bit_size + 1;            \
                unsigned long b_idx = (bit_size - 1) - (unsigned long)index % bit_size;\
                if (bool_výraz)                                                        \
                    jmeno_pole[el_idx] |= ((bitarray_element)1 << b_idx);              \
                else                                                                   \
                    jmeno_pole[el_idx] &= (~((bitarray_element)1 << b_idx));           \
            } while(0);
        #else
            #define bitarray_setbit(jmeno_pole, index, bool_výraz)                     \
            do {                                                                       \
                if ((unsigned long)index >= jmeno_pole[0])                             \
                    error_exit("bitarray_setbit: Index %lu mimo rozsah 0..%lu",        \
                        (unsigned long)index, (unsigned long)jmeno_pole[0] - 1);       \
                unsigned long el_idx = (unsigned long)index / bit_size + 1;            \
                unsigned long b_idx = (bit_size - 1) - (unsigned long)index % bit_size;\
                if (bool_výraz)                                                        \
                    jmeno_pole[el_idx] |= ((bitarray_element)1 << b_idx);              \
                else                                                                   \
                    jmeno_pole[el_idx] &= (~((bitarray_element)1 << b_idx));           \
            } while(0);
        #endif

        /** POZNAMKY PRO GETBIT
         * v pripade bez NO_CHECK ternarni operator
         * logika:
         * (element & (00..001 << bit_index)) >> bit_index
         * napr.: (1100 & (0001 << 2)) >> 2 -> (1100 & 0100) >> 2 -> 0100 >> 2 -> 0001
         * bit_index = pocet_bitu_v_elementu - kolikaty_bit_v_elementu
         */
        #ifdef NO_CHECK
            #define bitarray_getbit(jmeno_pole, index)                                          \
                (bitarray[(unsigned long)index / (unsigned long)(bit_size) + 1]                 \
                    & (((bitarray_element)1)                                                    \
                        << ((bit_size - 1) - (unsigned long)index % (unsigned long)(bit_size))) \
                )                                                                               \
                >> ((bit_size - 1) - (unsigned long)index % ((unsigned long)(bit_size)))
        #else
            #define bitarray_getbit(jmeno_pole, index)                                          \
                (unsigned long)index >= jmeno_pole[0] ?                                         \
                error_exit("bitarray_setbit: Index %lu mimo rozsah 0..%lu",                     \
                        (unsigned long)index, (unsigned long)jmeno_pole[0] - 1)                 \
                : (bitarray[(unsigned long)index / (unsigned long)(bit_size) + 1]               \
                    & (((bitarray_element)1)                                                    \
                        << ((bit_size - 1) - (unsigned long)index % (unsigned long)(bit_size))) \
                )                                                                               \
                >> ((bit_size - 1) - (unsigned long)index % ((unsigned long)(bit_size)))
        #endif

    #endif

#endif