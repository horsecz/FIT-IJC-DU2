/**
 *	@file	htab_clear.c
 *	@author Dominik Horky, FIT
 *	@date	20.04.2020
 *	@brief  Modul knihovny 'libhtab' pro funkci stejneho nazvu - smaze vsechny zaznamy z tabulky.
 *	@note	Reseni IJC-DU2, priklad b)
 *	@details Prelozeno na gcc 9.3.0 (Manjaro Linux)
 */

#include "htab_private.h"
#include <stdlib.h>

void htab_clear(htab_t * t) {
    if (t == NULL)
        return;

    htab_iterator_t removal = htab_begin(t);
    struct htab_item* rm = removal.ptr;
    struct htab_item* next;

    for (int i = 0; i < t->arr_size; i++) {
        next = t->ptr[i];
        while (next != NULL) {
            rm = next;
            next = rm->next;
            free((void*)rm->key);
            free(rm);
        }
    }
}