/**
 *	@file	htab_iterator_get_value.c
 *	@author Dominik Horky, FIT
 *	@date	20.04.2020
 *	@brief  Modul knihovny 'libhtab' pro funkci stejneho nazvu - vrati hodnotu (data) ze zaznamu, na ktery ukazuje iterator.
 *	@note	Reseni IJC-DU2, priklad b)
 *	@details Prelozeno na gcc 9.3.0 (Manjaro Linux)
 */

#include "htab_private.h"

htab_value_t htab_iterator_get_value(htab_iterator_t it) {
    if (!htab_iterator_valid(it))
        return 0;

    return it.ptr->data;
}