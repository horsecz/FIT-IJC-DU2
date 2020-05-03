/**
 *	@file	htab_free.c
 *	@author Dominik Horky, FIT
 *	@date	20.04.2020
 *	@brief  Modul knihovny 'libhtab' pro funkci stejneho nazvu - uvolni pamet tabulky vc. zaznamu = smaze tabulku
 *	@note	Reseni IJC-DU2, priklad b)
 *	@details Prelozeno na gcc 9.3.0 (Manjaro Linux)
 */

#include "htab_private.h"
#include <stdlib.h>

void htab_free(htab_t * t) {
    if (t == NULL)
        return;

    htab_clear(t);
    free(t);
    return;
}