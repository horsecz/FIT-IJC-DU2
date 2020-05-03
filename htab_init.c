/**
 *	@file	htab_init.c
 *	@author Dominik Horky, FIT
 *	@date	20.04.2020
 *	@brief  Modul knihovny 'libhtab' pro funkci stejneho nazvu - vytvori a inicializuje hashovaci tabulku.
 *	@note	Reseni IJC-DU2, priklad b)
 *	@details Prelozeno na gcc 9.3.0 (Manjaro Linux)
 */

#include "htab_private.h"
#include <stdlib.h>
#include <stdio.h>

htab_t *htab_init(size_t n) {
    htab_t *new_tab = (htab_t*) calloc(sizeof(htab_t)+sizeof(struct htab_item)*n,1);
    if (new_tab == NULL) {
        fprintf(stderr, "CHYBA: Nepodarilo se alokovat pamet pro hashovaci tabulku!\n");
        return NULL;
    }

    // inicializace hodnot
    new_tab->size = 0;
    new_tab->arr_size = n;

    // pro pripad exotickych platforem s 0 != NULL
    for (size_t i = 0; i < n; i++) {
        new_tab->ptr[n] = NULL;
    }

    return new_tab;
}