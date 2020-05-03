/**
 *	@file	htab_end.c
 *	@author Dominik Horky, FIT
 *	@date	20.04.2020
 *	@brief  Modul knihovny 'libhtab' pro funkci stejneho nazvu - vraci "zaznam" za poslednim existujicim zaznamem (=zarazka)
 *	@note	Reseni IJC-DU2, priklad b)
 *	@details Prelozeno na gcc 9.3.0 (Manjaro Linux)
 */

#include "htab_private.h"

htab_iterator_t htab_end(const htab_t * t) {
    htab_iterator_t end;
    end.ptr = NULL;

    if (t == NULL) {
        end.t = NULL;
        end.idx = 0;
        return end;
    }

    end.t = t;
    end.idx = t->arr_size;
    return end;
}