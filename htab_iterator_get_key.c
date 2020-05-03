/**
 *	@file	htab_iterator_get_key.c
 *	@author Dominik Horky, FIT
 *	@date	20.04.2020
 *	@brief  Modul knihovny 'libhtab' pro funkci stejneho nazvu - vrati klic zaznamu z iteratoru, ktery na nej ukazuje.
 *	@note	Reseni IJC-DU2, priklad b)
 *	@details Prelozeno na gcc 9.3.0 (Manjaro Linux)
 */

#include "htab_private.h"

htab_key_t htab_iterator_get_key(htab_iterator_t it) {
    if (!htab_iterator_valid(it))
        return NULL;

    return it.ptr->key;
}