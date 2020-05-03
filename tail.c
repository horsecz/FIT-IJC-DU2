/**
 *	@file	tail.c
 *	@author Dominik Horky, FIT
 *	@date	24.04.2020
 *	@brief  Implementace POSIX tail v C
 *	@note	Reseni IJC-DU2, priklad a)
 *	@details Prelozeno na gcc 9.3.0 (Manjaro Linux)
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "tail.h"

/**maximalni hodnota poctu znaku na jednom radku*/
#define DEFAULT_ROW_CHARS 1023
/**standardni hodnota vystupu (tolika = DEFAULT_PRINT_LINES) poslednich radku od konce vstupu*/
#define DEFAULT_PRINT_LINES 10
/**syntax pouziti programu*/
#define SYNTAX_STRING "./tail [-n] [POCET_RADKU] [SOUBOR]"
/**pokud je 1, vypise varovnou zpravu pri pouziti prilis velke hodnoty (=> overflow) u argumentu programu [CISLO]**/
#define CISLO_OVERFLOW_OUTPUT 0

int nArgCheck(char* n_argument) {
    if (!strcmp("-n", n_argument))
        return 0;
    else
        return -1;
}

int hasNumberPlusSign(char* argv) {
    if (argv[0] == '+')
        return 1;
    else
        return 0;
}

long int editPrintRows(int argc, char** argv) {
    long int print_rows;
    int offset;
    if (argc == 4) // 4 argc => bude tam filename => format: ./tail -n CISLO soubor.c
        offset = 2;
    else           // 3 argc => stdin => format: ./tail -n CISLO
        offset = 1;

    char *recyle_bin;
    print_rows = strtol(argv[argc-offset], &recyle_bin, 0);
    if (recyle_bin[0] != '\0') {
        fprintf(stderr, "CHYBA: Spatny argument [POCET_RADKU] ! (detekovany nelegalni znaky - povolena jsou pouze cisla)\n");
        return -2;
    }

    if (print_rows == LONG_MIN)
        return LONG_MAX;

    if (print_rows < 0)
        return -print_rows;

    return print_rows;
}

void printFile(FILE *f) {
    int c = 1;
    while ((c = fgetc(f)) != EOF) {
        if (isprint(c) || c == '\n')
            putchar(c);
    }
}

void file_tail(long int print_rows, FILE *f, int tail_start) {
    if (tail_start && print_rows <= 1) {
        printFile(f);
        return;
    }

    long int line_count = 0;
    int c;
    // +[CISLO] -> tisknu od 'print_rows' radku
    if (tail_start) {
        line_count = 1;
        while (((c = fgetc(f)) != EOF)) {
            if (c == '\n')
                line_count++;
            if (line_count >= print_rows)
                if (line_count != print_rows || c != '\n') { // prevence tisknuti \n na zacatku
                    putchar(c);
                }
        }
        return;
    }

    int seek_state = fseek(f, 0, SEEK_END);
    if (seek_state && f != stdin) {
        perror("CHYBA: Nelze najit konec souboru (fseek error) "); // vypise "text ": [errno prevedeny na string]
        return;
    }
    else if (seek_state && f == stdin)
        return;

    long int position = 0;
    line_count = 0;
    int ungotten = 0;

    while (!(seek_state = fseek(f, --position, SEEK_END))) {
        c = fgetc(f);
        if (c == EOF)
            break;
        if (c == '\n' && position != -1) { // jako POSIX tail > nepocitat \n jako line pokud je na konci file
            line_count++;
        }
        if ((line_count == print_rows && !tail_start)) {
            c = ungetc(1, f);
            ungotten++;
            break;
        }
    }

    // pokud jsem neskoncil breakem, znamena to ze cyklus koncil neplatnou podminkou => zacatek souboru
    if (!ungotten) {
        rewind(f);
        printFile(f);
    }

    // [CISLO] -> tisku 'print_rows'-ty radek odspodu
    printFile(f);
    return;
} // file_tail();

void stdin_tail_printStart(char **input, long int print_rows) {
    for (long int i = 1;;i++) {
        input[0] = fgets(input[0], DEFAULT_ROW_CHARS, stdin);

        if (input[0] == NULL) {
            break;
        }

        if (i >= print_rows - 1) {
            fprintf(stdout, "%s", input[0]);
        }
    }
    return;
}


int stdin_tail_rowResize(char** input, long int maxsize, long int print_rows) {
    void* reall;
    int error = 0;
    for (long int i = 0; i < print_rows; i++) {
        reall = realloc(input[i], maxsize+1);
        if (reall == NULL) {
            fprintf(stderr, "CHYBA: Realokace pameti se nezdarila!!\n");
            error++;
            break;
        }
        input[i] = reall;
    }

    return error;
}

void stdin_tail_printEnd(char **input, long int print_rows) {
    int c = 0;
    long int tmp = 0;
    long int maxsize = DEFAULT_ROW_CHARS;

    // pracuji dokud nenarazim na EOF, vzdy s poslednim polem
    for (long int i = print_rows-1, j = 0; c != EOF;) {
        // presune vsechny radky o pole vys => prvni se smaze, v poslednim bude misto pro nova data
        for (j = 0; j < i; j++) {
            if (strlen(input[j+1]))
                strcpy(input[j], input[j + 1]);
        }

        // nahraji se nova data do posledniho radku
        for (j = 0; ((c = fgetc(stdin)) != EOF); j++) {
            tmp = j;
            // je nutne zvysit velikost radku
            if (j > maxsize-2) {
                maxsize = strlen(input[i])*2;
                if (stdin_tail_rowResize(input, maxsize, print_rows))
                    break;
            }
            input[i][j] = c;
                // vlozeni znaku do radku, pokud je \n skoncim + vlozim \0 za \n. => jako fgets, ale "bez" limitu
            if (c == '\n') {
                input[i][j+1] = '\0';
                break;
            }
        }

        if (c == EOF) {
            input[i][tmp+1] = '\0'; // skoncil mi zanoreny for, je treba \0 doplnit
            // pokud cely soubor konci \n, tak tento \n chci ignorovat (jako POSIX tail)
            if (input[i][tmp] == '\n') {
                // posunu vsechny radky o pole niz => vyhodim \n a vytiskne se to co ma
                for (j = i; j > 0; j--) {
                    strcpy(input[j], input[j - 1]);
                }
            }
        }
    } // konec nacitani radku (forend)
} // stdin_tail_printEnd();

void stdin_tail_free(char** input, long int alloc_rows) {
    for (long int i = 0; i < alloc_rows; i++) {
        free(input[i]);
    }
}

void stdin_tail(long int print_rows, int tail_start) {
    // inkrementace pro vystup vsech radku (1 by chybel)
    print_rows++;

    long int alloc_rows = 1;
    if (!tail_start)
        alloc_rows = print_rows;

    // alokace pameti pro 'alloc_rows' radku
    char *input[alloc_rows]; /**<< @todo: neefektivni pri velkem poctu radku (optimalizovat?)**/
    for (long int i = 0; i < alloc_rows; i++) {
        input[i] = (char *) calloc((DEFAULT_ROW_CHARS) * sizeof(char), 1);
        if (input[i] == NULL) {
            fprintf(stderr, "CHYBA: Alokace pameti se nezdarila!\n");
            alloc_rows = -1;
            break;
        }
    }

    // chyba alokace
    if (alloc_rows < 0)
        return;

    // pokud +[CISLO] => chci tisknout od zacatku
    if (tail_start) {
        stdin_tail_printStart(input, print_rows);
        return;
    }
    // tisk od konce
    else
        stdin_tail_printEnd(input, print_rows);

    for (int i = 1; i < print_rows; i++) {
        printf("%s", input[i]);
    }

    stdin_tail_free(input, alloc_rows);
    return;
}


int main(int argc, char** argv) {
    if (argc > 4) {
        fprintf(stderr, "CHYBA: Spatny pocet argumentu!\n");
        return 1;
    }

    // long kvuli strtol (v editPrintRows();) a rozsahu (co kdyz chci vytisknout vazne hodne radku..)
    long int print_rows = DEFAULT_PRINT_LINES;
    FILE *f = NULL;

    // pri 3/4 argumentech je sance, ze je zde prepinac '-n' -> check + zmena prinintt_rows
    if (argc == 3 || argc == 4) {
        if (nArgCheck(argv[1]) < 0) {
            fprintf(stderr, "CHYBA: Spatny argument! (Nechtel jsi pouzit '-n' ?)\nSyntax: %s\n", SYNTAX_STRING);
            return 1;
        }
        print_rows = editPrintRows(argc, argv);
        if (print_rows < 0) {
            return 1;
        }
#if CISLO_OVERFLOW_OUTPUT
        if (print_rows == LONG_MAX) {
            fprintf(stderr, "VAROVANI: Bylo pouzito prilis velke cislo v argumentu [POCET_RADKU]. (Bude pouzita hodnota LONG_MAX)\n");
        }
#endif
    }

    // kontrola zda budu zacinat od konce nebo od zacatku
    int tail_start = 0;
    if ((argc == 3 && hasNumberPlusSign(argv[argc-1])) || (argc == 4 && hasNumberPlusSign(argv[argc-2])))
        tail_start = 1;

    // chci vytisknout 0 radku od konce
    if (!tail_start && !print_rows)
        return 0;

    // 2/4 argumenty => vstupni soubor - nebrat ze stdin
    if (argc == 2 || argc == 4) {
        // z posledniho argumentu vezme nazev souboru a ten otevre (nebo se pokusi)
        f = fopen(argv[argc-1], "r");
        if (f == NULL) {
            perror("Chyba v otevreni souboru ");
            return 1;
        }
    }
    // 1/3 argumenty => stdin
    else if (argc == 1 || argc == 3){
        stdin_tail(print_rows, tail_start);
        return 0;
    }
    else {
        fprintf(stderr, "CHYBA: Spatne argumenty !\nSyntax: %s\n", SYNTAX_STRING);
        return 1;
    }
    // provedeni tailu

    file_tail(print_rows, f, tail_start);
    //putchar('\n');
    fclose(f);
    return 0;
}