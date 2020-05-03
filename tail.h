#ifndef __TAIL_H__
#define __TAIL_H__

/**
 *	@file	tail.h
 *	@author Dominik Horky, FIT
 *	@date	24.04.2020
 *	@brief  Hlavickovy soubor k 'tail.c'
 *	@note	Reseni IJC-DU2, priklad a)
 *	@details Prelozeno na gcc 9.3.0 (Manjaro Linux)
 */

/**
 * @brief Kontrola vyskytu prepinace '-m' argumentu programu.
 * @param n_argument argv[1] z main(), bud -n nebo cokoli jineho
 * @return Pokud je n_argument "-n", vraci 0, jinak vrati -1
 */
int nArgCheck(char* n_argument);

/**
 * @brief Zjisti, jestli cislo ve stringu ma znamenko + (pro zjisteni [CISLO] nebo +[CISLO])
 * @param argv predano z main()
 * @return Vraci 1 pokud tam + je nebo 0 pokud neni
 */
int hasNumberPlusSign(char* argv);

/**
 * @brief Pri zjisteni '-n' se zavola tato funkce a upravi pocet radku, ktere se vytisknou na vystup.
 * @param argc predano z main(), pocet argumentu programu
 * @param argv predano z main(), argumenty programu
 * @return Vraci pocet radku, ktere se vytisknou na stdout. Pokud je vstupem zaporne cislo, vrati jeho absoultni hodnotu.
 *         Pokud je na vstupu cislo prilis velke (> LONG_MAX), vraci LONG_MAX. Dale vraci -1 pokud se detekuje +CISLO
 *         (vypis 'shora') a -2 vraci pri chybe.
 */
long int editPrintRows(int argc, char** argv);

/**
 * @brief Funkce vypise cely obsah souboru.
 * @details Uzito v priprade, kdy je zadano v argumentech '-n +1' nebo '-n +0', kdy se tiskne od pocatecniho radku
 *          => od zacatku souboru (=> cely soubor)
 * @param f soubor, ktery bude vytisknut na stdout
 */
void printFile(FILE *f);

/**
 * @brief Funkce zajistujici vypis poslednich 'print_rows' radku ze souboru
 * @param print_rows pocet radku, kolik jich bude vypsano na stdout
 * @param f soubor
 * @param tail_start kde tail zacne s vypisem (+[CISLO] nebo [CISLO]) - 0 -> od konce, 1 -> od n-radku v cisle -n [CISLO]
 */
void file_tail(long int print_rows, FILE *f, int tail_start);

/**
 * @brief Samotny algoritmus pro vypis vsech radku od 'print_rows'-teho radku, vcetne vypisu.
 * @param input jednotlive radky ve 2D poli
 * @param print_rows n-ty radek od zacatku vstupu, od ktereho zacne vystup na stdout
 */
void stdin_tail_printStart(char **input, long int print_rows);

/**
 * @brief Zmena maximalni velikosti radku, resp. maximalniho poctu znaku vsech radku.
 * @details Program ocekava, ze pokud existuje delsi radek nez 'DEFAULT_ROW_CHARS', potom takovych radku ve vstupu bude vice.
 * @param input jednotlive radky ve 2D poli
 * @param maxsize nova maximalni velikost radku
 * @param print_rows pocet radku, ktere se udrzuji v pameti
 * @return
 */
int stdin_tail_rowResize(char** input, long int maxsize, long int print_rows);

/**
 * @brief Samotny algoritmus zpracovani kazdeho radku pro stdin - pro vypis poslednich 'print_rows' radku.
 * @param input jednotlive radky ve 2D poli (pole ukazatelu na pole)
 * @param print_rows pocet radku, ktere se budou udrzovat v pameti, take pocet radku, co se vypise na stdout
 * @return Vraci 0 nebo EOF, pokud tail narazi na konec souboru.
 */
void stdin_tail_printEnd(char **input, long int print_rows);

/**
 * @brief Uvolni dynamicky alokovanou pamet (radky)
 * @param input pole radku, ktere se vytiskly
 * @param alloc_rows pocet radku
 */
void stdin_tail_free(char** input, long int alloc_rows);

/**
 * @brief Hlavni funkce pro tail ze stdin - alokuje si 'print_rows' radku nebo pouze jeden, pokud je 'tail_start == 1'
 * @param print_rows pocet radku, ktere se vytisknou (od konce, od zacatku)
 * @param tail_start urcuje, zda se bude tisknout od konce (= 0) nebo od zacatku (= 1)
 */
void stdin_tail(long int print_rows, int tail_start);

#endif