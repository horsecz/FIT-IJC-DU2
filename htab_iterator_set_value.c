/**
 *	@file	htab_iterator_set_value.c
 *	@author Dominik Horky, FIT
 *	@date	20.04.2020
 *	@brief  Modul knihovny 'libhtab' pro funkci stejneho nazvu - nastavi hodnotu (data) v zaznamu, na ktery ukazuje iterator
 *	@note	Reseni IJC-DU2, priklad b)
 *	@details Prelozeno na gcc 9.3.0 (Manjaro Linux)
 */

#include "htab_private.h"

htab_value_t htab_iterator_set_value(htab_iterator_t it, htab_value_t val) {
    if (!htab_iterator_valid(it))
        return 0;
    return it.ptr->data = val;
}