/**
 *	@file	wordcount.c
 *	@author Dominik Horky, FIT
 *	@date	20.04.2020
 *	@brief  Modul wordcount spocita pocet vyskytu vsech slov ze vstupu (stdin).
 *	@note	Reseni IJC-DU2, priklad b)
 *	@details Prelozeno na gcc 9.3.0 (Manjaro Linux)
 */

#include "htab.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>

/**konstanta velikosti hashovaci tabulky - idealni je 1.3 nasobek poctu klicu, ktere budou v tabulce + prvocislo
 *
 * zadana velikost zde tam je z duvodu, ze toto je program na pocitani slov a nejcastejsi vyuziti programu
 * tedy (pravdepodobne) bude pocitani slov v nejakem rozsahlem textu - kniha, clanek, ..,
 * prumernych vyskytu (klicu) je cca 20 000 (u ruznych vstupu), 1.3 nasobek je 26 000 a nejblizsi prvocislo je dany define
 * (optimalni velikost hashovaci tabulky je totiz 1.3 * pocet_klicu && prvocislo)
 **/
#define DEF_HTAB_SIZE 26003
/**konstanta maximalniho poctu znaku jednoho slova*/
#define WORD_SIZE_LIMIT 127

/* Definice probihaji pri prekladu -> urcuji, ktera z hashovacich funkci se pouzije (knihovni, nejaka odtud)
 * Pri definovani HASHTEST, DJB2 nebo LOSELOSE se nahradi knihovni funkce danou hashovaci funkci.
 * Standartne pouziva knihovna algoritmus SDBM.
 */
#ifdef HASHTEST
#include <stdint.h>
// "nahradni" vlastni hash funkce (inspirace z algoritmu sdbm, djb2, lose lose)
size_t htab_hash_fun(htab_key_t str) {
    uint64_t hash = 32771;
    unsigned char c = 1;
    for (int i = 0; c != '\0'; i++) {
        c = str[i];
        hash = hash*13 + (c^2);
        if (hash > 32771)
            hash ^= c*13;
    }
    return hash;
}

#elif DJB2
// algoritmus djb2 (prevzato z http://www.cse.yorku.ca/~oz/hash.html)
size_t htab_hash_fun(htab_key_t str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

#elif LOSELOSE
// algoritmus lose lose uveden v K&R, neefektivni, spatny hashovaci algoritmus, pouzity pouze jako vzor (prevzato z http://www.cse.yorku.ca/~oz/hash.html)
size_t htab_hash_fun(htab_key_t str)
{
	unsigned int hash = 0;
	int c;

	while ((c = *str++))
	    hash += c;

	return hash;
}
#endif

int main(void) {
    // "unordered map"
    htab_t *m = htab_init(DEF_HTAB_SIZE);
    if (m == NULL)
        return 1;

    htab_iterator_t iterator;

    // alokace pameti pro string, resp slovo
    char *word = (char*) calloc(sizeof(char)*(WORD_SIZE_LIMIT+1), 1);
    int word_size = 1;
    int overflow_msg = 0;

    while (word_size != EOF) {
        // nacte 1 slovo
        word_size = get_word(word, WORD_SIZE_LIMIT, stdin);

        if (word_size > WORD_SIZE_LIMIT)
            overflow_msg++;

        if (word_size > 0 && word_size < WORD_SIZE_LIMIT+1) {
            // m[word]++
            iterator = htab_find(m, word);
            if (htab_iterator_valid(iterator)) {
                htab_iterator_set_value(iterator, htab_iterator_get_value(iterator) + 1);
            } else {
                iterator = htab_lookup_add(m, word);
                if (!htab_iterator_valid(iterator)) {
                    htab_free(m);
                    break;
                }
            }
        }
    }

    // precist vsechny prvky kontejneru a vypsat ve formatu 'klic hondnota'
    for(htab_iterator_t temp_it = htab_begin(m); !htab_iterator_equal(htab_end(m), temp_it); temp_it = htab_iterator_next(temp_it)) {
        printf("%s\t%d\n", htab_iterator_get_key(temp_it), htab_iterator_get_value(temp_it));
    }

    // vypise hlasku pokud se najde delsi slovo nez 127 znaku a zkrati se (az po vystupu klicu/vyskytu, kdyby to bylo pred vystupem, hlaska se lehce ztrati)
    if (overflow_msg)
        fprintf(stderr, "VAROVANI: Ve vstupu se vyskytly slova s vice jak %d znaky - tato slova byla zkracena (na 127 znaku)\n", WORD_SIZE_LIMIT);

    htab_free(m);
    free(word);
    return 0;
}