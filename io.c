/**
 *	@file	io.c
 *	@author Dominik Horky, FIT
 *	@date	20.04.2020
 *	@brief  Modul obsahujici funkci get_word (nepatri do knihovny).
 *	@note	Reseni IJC-DU2, priklad b)
 *	@details Prelozeno na gcc 9.3.0 (Manjaro Linux)
 */

#include "io.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>

/* poznamky:
 * - predpokladam a pocitam s tim, ze pole je velke n+1 znaku, ktere se maji nacist pro jedno slovo (pro \0)
 * - vypracovani bez impl. limitu by bylo slozite (v zadani je stejne povolen limit) - bylo by potreba, aby funkce vracela pole ...
 * - mozne reseni problemu: nepouzivat get_word jako funkci, ale cely kod mit v mainu (nehezke reseni, nicmene funkcni)
 */
int get_word(char *s, int max, FILE *f) {
    if (s == NULL || f == NULL)
        return EOF;

    memset(s, 0, max); // vynuluje pole (poprve pri callocu to treba neni, ale po prvnim zapisu uz ano)
    int character = 0;
    int i = 0;
    int overflow = 0;

    // je treba preskocit mezery (=nejsou slovo), jinak vratim prazdny jen \0
    while((character = fgetc(f)) != EOF && isspace(character));
    if (character == EOF)
        return EOF;

    // jelikoz jsem uz jeden znak nacetl, vlozim ho do pole (i=0)
    s[i] = character;

    for (i = 1; ((character = fgetc(f)), !isspace(character)) && character != EOF; i++) {
        if (i >= max) { // musi byt misto pro '\0'
            character = fgetc(f);
            overflow++;
        }
        else
            s[i] = character;
    }
    s[i] = '\0';

    if (character == EOF)
        return EOF;

    if (overflow)
        return max+1;

    return strlen(s);
}