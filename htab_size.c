/**
 *	@file	htab_size.c
 *	@author Dominik Horky, FIT
 *	@date	20.04.2020
 *	@brief  Modul knihovny 'libhtab' pro funkci stejneho nazvu - vrati pocet zaznamu tabulky.
 *	@note	Reseni IJC-DU2, priklad b)
 *	@details Prelozeno na gcc 9.3.0 (Manjaro Linux)
 */

#include "htab_private.h"

size_t htab_size(const htab_t * t) {
    if (t == NULL)
        return 0;

    return t->arr_size;
}             // počet záznamů v tabulce