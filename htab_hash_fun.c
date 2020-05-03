/**
 *	@file	htab_hash_fun.c
 *	@author Dominik Horky, FIT
 *	@date	20.04.2020
 *	@brief  Modul knihovny 'libhtab' pro funkci stejneho nazvu - hashovaci funkce a algoritmy.
 *	@note	Reseni IJC-DU2, priklad b)
 *	@details Prelozeno na gcc 9.3.0 (Manjaro Linux)
 */

#include "htab_private.h"
#include <stdint.h>

// algoritmus sdbm (zadani, pouziva se implicitne, prevzato z http://www.cse.yorku.ca/~oz/hash.html)
size_t htab_hash_fun(htab_key_t str) {
    uint32_t h = 0;     // musí mít 32 bitů
    const unsigned char *p;
    for(p = (const unsigned char*) str; *p != '\0'; p++)
        h = 65599*h + *p;
    return h;
}