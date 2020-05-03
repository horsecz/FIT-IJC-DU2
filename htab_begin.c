/**
 *	@file	htab_begin.c
 *	@author Dominik Horky, FIT
 *	@date	20.04.2020
 *	@brief  Modul knihovny 'libhtab' pro funkci stejneho nazvu - vraci prvni existujici zaznam tabulky.
 *	@note	Reseni IJC-DU2, priklad b)
 *	@details Prelozeno na gcc 9.3.0 (Manjaro Linux)
 */

#include "htab_private.h"

htab_iterator_t htab_begin(const htab_t * t) {
    htab_iterator_t begin;

    if (t == NULL) {
        return htab_end(t);
    }

    begin.ptr = t->ptr[0]; // "prvni" seznam v tabulce
    begin.t = t;
    begin.idx = 0;

    if (!t->size) // 0 zaznamu => zadny begin :(
        return begin;

    if (begin.ptr != NULL) // pokud prvni seznam v tabulce obsahuje zaznam
        return begin;

    begin = htab_iterator_next(begin); // najde prvni existujici zaznam

    return begin;
}