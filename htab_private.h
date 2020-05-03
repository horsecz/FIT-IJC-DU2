/**
 *	@file	htab_private.h
 *	@author Dominik Horky, FIT
 *	@date	20.04.2020
 *	@brief  Privatni hlavickovy soubor obsahujici definice struktur.
 *	@note	Reseni IJC-DU2, priklad b)
 *	@details Prelozeno na gcc 9.3.0 (Manjaro Linux)
 */

#ifndef __HTAB_PRIVATE_H__
#define __HTAB_PRIVATE_H__

#include "htab.h"

struct htab {
    htab_value_t size;
    htab_value_t arr_size;
    struct htab_item *ptr[];
};

struct htab_item {
    char* key;
    htab_value_t data;
    struct htab_item *next;
};

#endif