/**
 *	@file	htab_lookup_add.c
 *	@author Dominik Horky, FIT
 *	@date	20.04.2020
 *	@brief  Modul knihovny 'libhtab' pro funkci stejneho nazvu - najde zaznam se zadanym klicem, pokud neexistuje, tak ho vytvori.
 *	@note	Reseni IJC-DU2, priklad b)
 *	@details Prelozeno na gcc 9.3.0 (Manjaro Linux)
 */

#include "htab_private.h"
#include <stdlib.h>

htab_iterator_t htab_lookup_add(htab_t * t, htab_key_t key) {
    if (t == NULL || key == NULL)
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

    struct htab_item *new_item = (struct htab_item*) malloc(sizeof(struct htab_item));

    // kontrola alokace -> pokud nevyjde, vracim nevalidni iterator (nutny test: iterator_valid(it);)
    if (new_item == NULL) {
        item_iterator.ptr = NULL;
        return item_iterator;
    }

    new_item->key = (char*) calloc(strlen(key)+1, 1);
    if (new_item->key == NULL) {
        item_iterator.ptr = NULL;
        free(new_item);
        return item_iterator;
    }
    strcpy(new_item->key, (char*) key);
    new_item->data = 1; // data = pocet vyskytu => novy prvek => 1 vyskyt
    new_item->next = NULL; // bude posledni na seznamu => NULL


    // pripojim item do seznamu
    // item je prvnim zaznamem seznamu
    if (item_iterator.ptr == NULL) {
        t->ptr[index] = new_item;
    }
    else if (item_iterator.ptr->next == NULL) { // item bude na konci seznamu
        if (t->ptr[index] == item_iterator.ptr)
            t->ptr[index]->next = new_item;
        else
            item_iterator.ptr->next = new_item;
    }

    item_iterator.ptr = new_item;
    t->size++;

    return item_iterator;
}