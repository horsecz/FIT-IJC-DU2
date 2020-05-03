/**
 *	@file	io.h
 *	@author Dominik Horky, FIT
 *	@date	20.04.2020
 *	@brief  Hlavickovy soubor modulu io.c
 *	@note	Reseni IJC-DU2, priklad b)
 *	@details Prelozeno na gcc 9.3.0 (Manjaro Linux)
 */

#ifndef __IO_H__
#define __IO_H__

#include <stdio.h>

/**
 * @brief Nacte 1 slovo do zadaneho pole *s.
 * @details Hodnota parametru 'max' se muze dynamicky zvetsit - na zaklade delsiho (vetsiho poctu znaku) slova.
 * @param s pole, do ktereho se nacte slovo
 * @param max (zakladni) maximalni pocet znaku slova
 * @param f stream, soubor, z ktereho se precte slovo.
 * @return Vraci delku slova, resp. pocet znaku znaku nebo EOF v pripade ze je nacteno EOF (slovo je poslednim v souboru)
 */
int get_word(char *s, int max, FILE *f);

#endif
