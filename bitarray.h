#ifndef BITARRAY_H
#define BITARRAY_H
    
    #include <stdlib.h> //calloc a free
    #include <stdbool.h> //true a false
    #include <limits.h> //CHAR_BIT
    #include "error.h" //error_exit

    /* --- DEFINICE BITARRAY --- */

    //datovy typ bitoveho pole (unsigned long pro erasthothenes)
    #define bitarray_element unsigned long

    //typ bitoveho pole
    typedef bitarray_element* bitarray_t;

    //Typ indexu bitoveho pole
    typedef unsigned long bitarray_index_t;


    /* --- MAKRA PRO VELIKOSTI --- */

    //makro pro velikost jednoho bitarray_elementu v bitech
    #define bit_size ((unsigned long)sizeof(bitarray_element) * CHAR_BIT)

    //makro pro pocet bitarr_elementu v poli o velikost velikost
    #define bitarr_elem_size(velikost)          \
            ((unsigned long)                    \
            ((velikost) % bit_size == 0 ?       \
            ((velikost) / bit_size) + 1         \
            : ((velikost) / bit_size) + 2))


    /* --- MAKRA ZE ZADANI --- */

    //vytvori pole jmeno_pole s velikosti v prvnim elementu, nulovano
    #define bitarray_create(jmeno_pole, velikost)                                                       \
        bitarray_element jmeno_pole[bitarr_elem_size(velikost)] = {[0] = (bitarray_element)(velikost)}

    //alokuje jmeno_pole s velikosti v prvnim elementu, nulovano
    // ! nebude fungovat uvnitr zkraceneho IFu
    #define bitarray_alloc(jmeno_pole, velikost)                                                \
        bitarray_t jmeno_pole = calloc(bitarr_elem_size(velikost), sizeof(bitarray_element));   \
        if (!jmeno_pole)                                                                        \
        {                                                                                       \
            fprintf(stderr, "bitarray_alloc: Chyba alokace paměti\n");                          \
            exit(1);                                                                            \
        }                                                                                       \
        jmeno_pole[0] = (bitarray_element)(velikost);                                           \

    #ifdef USE_INLINE
        static inline void bitarray_free(bitarray_t jmeno_pole)
        {
            //jestlize je NULL, neuvolnovat
            if (jmeno_pole)
                free(jmeno_pole);
        }

        //vrati velikost bitarray v bitech
        static inline unsigned long bitarray_size(bitarray_t jmeno_pole)
        {
            return jmeno_pole[0];
        }

        //vyplni bitarray 0 (false), nebo 1 (true)
        static inline void bitarray_fill(bitarray_t jmeno_pole, bool bool_výraz)
        {
            if (!bool_výraz)
            {
                for (unsigned long i = 1; i < bitarr_elem_size(jmeno_pole[0]); i++)
                {
                    jmeno_pole[i] = 0;
                }
            }
            else
            {
                for (unsigned long i = 1; i < bitarr_elem_size(jmeno_pole[0]); i++)
                {
                    jmeno_pole[i] = ~((bitarray_element)0);
                }
            }
        }

        //nastavi bit na indexu na 0(false) nebo 1 (true)
        static inline void bitarray_setbit(bitarray_t jmeno_pole, unsigned long index, bool bool_výraz)
        {
            #ifndef NO_CHECK
            if (index >= jmeno_pole[0])
                error_exit("bitarray_setbit: Index %lu mimo rozsah 0..%lu", index, jmeno_pole[0] - 1);
            #endif
            
            //index elementu v poli, ktery se zmeni
            unsigned long el_idx = index / bit_size + 1; //+1 kvuli velikosti na nultem indexu

            //index bitu (default pro little endian)
            unsigned long b_idx = (bit_size - 1) - index % bit_size;

            if (bool_výraz)
                jmeno_pole[el_idx] |= ((bitarray_element)1) << b_idx;
            else
                jmeno_pole[el_idx] &= ~(((bitarray_element)1) << b_idx);
        }

        //vrati hodnotu bitu na danem indexu
        static inline int bitarray_getbit(bitarray_t jmeno_pole, unsigned long index)
        {
            #ifndef NO_CHECK
            if (index >= jmeno_pole[0])
                error_exit("bitarray_getbit: Index %lu mimo rozsah 0..%lu", index, jmeno_pole[0] - 1);
            #endif
            
            unsigned long el_idx = index / bit_size + 1;
            unsigned long b_idx = (bit_size - 1) - index % bit_size;
            

            //posun jednicky na bitovy index elementu
            bitarray_element changer = ((bitarray_element)1) << b_idx;

            //vysledek bude 0 pokud bit == 0, jinak bude 2^b_idx
            if (jmeno_pole[el_idx] & changer)
                return 1;
            return 0;
        }
    #else

        //uvolni pole z heapu
        #define bitarray_free(jmeno_pole)   \
        do {                                \
            if (jmeno_pole)                 \
                free(jmeno_pole);           \
        } while(0);

        //velikost pole v bitech
        #define bitarray_size(jmeno_pole)   \
            jmeno_pole[0]

        //naplni pole nulami (false), nebo jednickami (true)
        #define bitarray_fill(jmeno_pole, bool_výraz)                               \
        do {                                                                        \
            if (!(bool_výraz))                                                      \
            {                                                                       \
                for (unsigned long i = 1; i < bitarr_elem_size(jmeno_pole[0]); i++) \
                {                                                                   \
                    jmeno_pole[i] = 0;                                              \
                }                                                                   \
            }                                                                       \
            else                                                                    \
            {                                                                       \
                for (unsigned long i = 1; i < bitarr_elem_size(jmeno_pole[0]); i++) \
                {                                                                   \
                    jmeno_pole[i] = ~((bitarray_element)0);                         \
                }                                                                   \
            }                                                                       \
        } while(0);

        /** POZNAMKY PRO SETBIT
         * logika:
         * pro 1: element = element | (00..001 << bit_index)
         * napr. pro bit_index = 2: 1000 | (0001 << 2) -> 1000 | 0100 -> 1100
         * pro 2: element = element & neg(00..001 << bit_index)
         * napr pro bit_index = 2: 1110 & neg(0001 << 2) -> 1110 & neg(0100) -> 1110 & 1011 -> 1010
         * bit index upraven pro little_endian
         */
        #ifdef NO_CHECK
            #define bitarray_setbit(jmeno_pole, index, bool_výraz)                          \
            do {                                                                            \
                unsigned long el_idx = ((unsigned long)(index)) / bit_size + 1;             \
                unsigned long b_idx = (bit_size - 1) - ((unsigned long)(index)) % bit_size; \
                if (bool_výraz)                                                             \
                    jmeno_pole[el_idx] |= ((bitarray_element)1) << b_idx;                   \
                else                                                                        \
                    jmeno_pole[el_idx] &= ~(((bitarray_element)1) << b_idx);                \
            } while(0);
        #else
            #define bitarray_setbit(jmeno_pole, index, bool_výraz)                          \
            do {                                                                            \
                if ((unsigned long)(index) >= jmeno_pole[0])                                \
                    error_exit("bitarray_setbit: Index %lu mimo rozsah 0..%lu",             \
                        (unsigned long)(index), (unsigned long)jmeno_pole[0] - 1);          \
                unsigned long el_idx = ((unsigned long)(index)) / bit_size + 1;             \
                unsigned long b_idx = (bit_size - 1) - ((unsigned long)(index)) % bit_size; \
                if (bool_výraz)                                                             \
                    jmeno_pole[el_idx] |= ((bitarray_element)1) << b_idx;                   \
                else                                                                        \
                    jmeno_pole[el_idx] &= ~(((bitarray_element)1) << b_idx);                \
            } while(0);
        #endif

        /** POZNAMKY PRO GETBIT
         * v pripade bez NO_CHECK ternarni operator
         * logika:
         * (element & (00..001 << bit_index)) >> bit_index
         * napr.: (1100 & (0001 << 2)) >> 2 -> (1100 & 0100) >> 2 -> 0100 >> 2 -> 0001
         * bit_index = pocet_bitu_v_elementu - kolikaty_bit_v_elementu
         * bit_index upraven pro little endian
         */
        #ifdef NO_CHECK
            #define bitarray_getbit(jmeno_pole, index)                                          \
                ((jmeno_pole[(unsigned long)(index) / bit_size + 1]                             \
                    & (((bitarray_element)1)                                                    \
                        << ((bit_size - 1) - (unsigned long)(index) % bit_size))                \
                )                                                                               \
                >> ((bit_size - 1) - (unsigned long)(index) % bit_size))
        #else
            #define bitarray_getbit(jmeno_pole, index)                                          \
                ((unsigned long)(index) >= jmeno_pole[0] ?                                      \
                (error_exit("bitarray_getbit: Index %lu mimo rozsah 0..%lu",                    \
                        (unsigned long)(index), (unsigned long)jmeno_pole[0] - 1), 0)           \
                : (jmeno_pole[(unsigned long)(index) / bit_size + 1]                            \
                    & (((bitarray_element)1)                                                    \
                        << ((bit_size - 1) - (unsigned long)(index) % bit_size))                \
                )                                                                               \
                >> ((bit_size - 1) - ((unsigned long)(index) % bit_size)))
        #endif

    #endif

#endif