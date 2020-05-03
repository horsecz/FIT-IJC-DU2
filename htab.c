/**
 *	@file	htab.c
 *	@author Dominik Horky, FIT
 *	@date	20.04.2020
 *	@brief  Externi deklarace inline funkci
 *	@note	Reseni IJC-DU2, priklad b)
 *	@details Prelozeno na gcc 9.3.0 (Manjaro Linux)
 */

#include "htab_private.h"

extern inline bool htab_iterator_valid(htab_iterator_t it);
extern inline bool htab_iterator_equal(htab_iterator_t it1, htab_iterator_t it2);