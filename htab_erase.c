/**
 *	@file	htab_erase.c
 *	@author Dominik Horky, FIT
 *	@date	20.04.2020
 *	@brief  Modul knihovny 'libhtab' pro funkci stejneho nazvu - smaze 1 existujici zaznam.
 *	@note	Reseni IJC-DU2, priklad b)
 *	@details Prelozeno na gcc 9.3.0 (Manjaro Linux)
 */

#include "htab_private.h"
#include <stdlib.h>

// predpokladam ze cil existuje => iterator it je validni !
void htab_erase(htab_t * t, htab_iterator_t it) {
    if (t == NULL)
        return;

    htab_iterator_t removal = htab_begin(t); // removed iterator
    htab_iterator_t b_removal = removal; // iterator to item before
    htab_iterator_t n_removal = removal; // iterator to item after

    while (!htab_iterator_equal(removal, it)) {
        b_removal = removal;
        removal = htab_iterator_next(removal);
    }
    n_removal = htab_iterator_next(removal);

    struct htab_item* b_item = b_removal.ptr;
    struct htab_item* rm_item = removal.ptr;
    struct htab_item* n_item = n_removal.ptr;

    if (t->size == 1) { // v cele tabulce je jen muj mazany zaznam
        t->ptr[removal.idx] = NULL;
    }
    else if (b_item->next == rm_item) { // naslednik predchoziho je mazany => mazu ve STEJNEM seznamu
        if (rm_item->next == NULL) { // muj zaznam nema naslednika -> je poslednim na seznamu
            b_item->next = NULL;
        }
        else { // muj zaznam naslednika ma -> je nekde uprostred seznamu
            if (rm_item->next == n_item) // naslednik meho zaznamu je n_item
                b_item->next = n_item;
            else // n_item neni ve stejnem seznamu jako mazany prvek => seznam mym zaznamem konci
                b_item->next = NULL;
        }
    }
    else if (rm_item->next == n_item) { // naslednik mazaneho prvku je n_item => mazu ve STEJNEM seznamu
        if (b_item->next != rm_item) { // predchozi zaznam je v jinem seznamu => muj zaznam je na zacatku seznamu
            if (n_item != NULL)
                t->ptr[removal.idx] = n_item; // zacatek seznamu je ted n_item
            else
                t->ptr[removal.idx] = NULL;
        }
    }
    else { // b_item ani n_item nejsou pred/za mym zaznamem (v jednom seznamu) => nejsou ve stejnem seznamu
        t->ptr[removal.idx] = NULL;
    }

    free((void*)rm_item->key);
    free(rm_item);
    t->size--;
    return;
}