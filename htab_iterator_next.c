/**
 *	@file	htab_iterator_next.c
 *	@author Dominik Horky, FIT
 *	@date	20.04.2020
 *	@brief  Modul knihovny 'libhtab' pro funkci stejneho nazvu - posune se na dalsi existujici zaznam (iterator++)
 *	@note	Reseni IJC-DU2, priklad b)
 *	@details Prelozeno na gcc 9.3.0 (Manjaro Linux)
 */

#include "htab_private.h"

htab_iterator_t htab_iterator_next(htab_iterator_t it) {
    if (!it.t->size) { // v tabulce nejsou zaznamy
        return htab_end(it.t);
    }

    htab_iterator_t new_iterator;
    new_iterator.idx = it.idx;
    new_iterator.ptr = it.ptr;
    new_iterator.t = it.t;

    // aktualni seznam ma alespon 1 polozku a ta ma naslednika (2+ polozek v seznamu)
    if (it.ptr != NULL && it.ptr->next != NULL) {
        new_iterator.ptr = it.ptr->next;
        return new_iterator;
    }

    if (new_iterator.ptr == NULL || new_iterator.ptr->next == NULL) { // aktualni seznam zaznamy nema nebo jsem na jeho konci -> hledam 1 polozku jineho seznamu
        while (new_iterator.idx++ < new_iterator.t->arr_size) { // !! inkrementace idx
            new_iterator.ptr = it.t->ptr[new_iterator.idx];
            if (new_iterator.ptr != NULL) {
                break;
            }
        }
    }

    if (new_iterator.ptr == NULL)
        return htab_end(new_iterator.t);

    return new_iterator;
}