/**
 *	@file	htab_find.c
 *	@author Dominik Horky, FIT
 *	@date	20.04.2020
 *	@brief  Modul knihovny 'libhtab' pro funkci stejneho nazvu - najde zaznam se zadanym klicem.
 *	@note	Reseni IJC-DU2, priklad b)
 *	@details Prelozeno na gcc 9.3.0 (Manjaro Linux)
 */

#include "htab_private.h"
#include <string.h>

htab_iterator_t htab_find(htab_t * t, htab_key_t key) {
    if (t == NULL)
        return htab_end(t);

    size_t index = (htab_hash_fun(key) % t->arr_size); // zjistim index kam vlozit dle hash. funkce

    // pokusi se najit zaznam na zadanem indexu
    htab_iterator_t item_iterator;
    item_iterator.t = t;
    item_iterator.idx = index;
    item_iterator.ptr = t->ptr[index];

    // v danem seznamu dojdu az na jeho konec
    while (item_iterator.ptr != NULL) {
        if (!strcmp(item_iterator.ptr->key, key)) {
            return item_iterator;
        }
        if (item_iterator.ptr->next != NULL)
            item_iterator.ptr = item_iterator.ptr->next;
        else
            break;
    }

    return htab_end(t);
}