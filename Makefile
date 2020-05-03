#
#	Makefile
#
#	Reseni:			IJC-DU2
#	Datum:			21. 04. 2020
#	Autor:			Dominik Horky, FIT
#	Vystup gcc -v:	gcc version 9.3.0 (Arch Linux 9.3.0-1)
# 	Poznamka:		knihovny jsou sestavovany a pouzivany z aktualniho adresare

#	Pouziti:
#			make / make all	-> zkompiluje a vytvori spustitelne soubory
#			make clean		-> smaze .o a spustitelne soubory

#
# PREKLADAC, PARAMETRY PREKLADU
#

CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -O2
DYNAMIC=-fPIC
HALG=SDBM

#
# DODATECNE PROMENNE
#

# vsechny objektove soubory hashovaci tabulky
OBJ=htab.o htab_begin.o htab_bucket_count.o htab_clear.o htab_end.o htab_erase.o htab_find.o htab_free.o htab_hash_fun.o htab_init.o htab_iterator_get_key.o htab_iterator_get_value.o htab_iterator_next.o htab_iterator_set_value.o htab_lookup_add.o htab_size.o

#
# MAKE
#

all: tail wordcount wordcount-dynamic

#
# KONTROLA VALIDITY HALG
#

#
# VYTVORENI OBJEKTOVYCH SOUBORU
#

# TAIL
tail.o: tail.c tail.h
	$(CC) $(CFLAGS) -c $<

# WORDCOUNT
# hash table
%.o: %.c htab_private.h
	$(CC) $(CFLAGSS) $(DYNAMIC) -c $<

# io modul pro wordcount
io.o: io.c io.h
	$(CC) $(CFLAGS) -c $<

# samotny wordcount
wordcount.o: wordcount.c htab.h io.h libhtab.a
	$(CC) $(CFLAGS) -D$(HALG) -c wordcount.c htab.h io.h

wordcount-dyn.o: wordcount.c htab.h io.h libhtab.so
	$(CC) $(CFLAGS) -D$(HALG) $(DYNAMIC) -c wordcount.c -o $@

# KNIHOVNY
# staticka
libhtab.a: $(OBJ)
	LD_LIBRARY_PATH="."
	ar crsv libhtab.a $(OBJ)
	ranlib $@

# dynamicka
libhtab.so: $(OBJ)
	LD_LIBRARY_PATH="."
	$(CC) -shared $(DYNAMIC) $(OBJ) -o $@

#
# VYTVORENI SPUSTITELNYCH SOUBORU
#

tail: tail.o
	$(CC) $(CFLAGS) $? -o $@
wordcount: wordcount.o io.o libhtab.a
	LD_LIBRARY_PATH="."
	$(CC) $(CFLAGS) -static wordcount.o io.o -L. -lhtab -Wl,-rpath="$(PWD)" -o $@
wordcount-dynamic: wordcount-dyn.o io.o libhtab.so
	$(CC) $(CFLAGS) wordcount-dyn.o io.o -L. -lhtab -Wl,-rpath="$(PWD)" -o $@

#
# DODATECNA FUNKCIONALITA
#

# provede 'uklid'
clean:
	rm -rf *.o *.a *.so *.zip tail wordcount wordcount-dynamic

# zazipuje, aby bylo mozne odevzdat
zip:
	zip xhorky32.zip *.c *.h Makefile

# vytvori pouze knihovny
libs: libhtab.a libhtab.so
	rm -rf *.o

# vypise help
help:
	@printf "Makefile pro IJC-DU2.\n"
	@printf "Pouziti:\n"
	@printf 	"\tSESTAVENI\n"
	@printf 	"\tmake [all]		zkompiluje a vytvori vsechny spustitelne soubory a knihovny\n"
	@printf 	"\tmake tail		vytvori 'tail'\n"
	@printf		"\tmake wordcount		vytvori 'wordcount' vyuzivajici statickou knihovnu libhtab\n"
	@printf 	"\tmake wordcount-dynamic	vytvori 'wordcount-dynamic' pouzivajici dynamickou knihovnu libhtab\n"
	@printf		"\tmake libs		vytvori pouze knihovnu libhtab (statickou i dynamickou)\n"
	@printf		"\n\tMOZNOSTI\n"
	@printf 	"\tmake HALG=HASHTEST	totez co 'make [all]', pro hashovani se zvoli muj vlastni algoritmus\n"
	@printf 	"\tmake HALG=SDBM		implicitni hashovaci algoritmus programu, stejne jakoe 'make' bez dalsich parametru\n"
	@printf 	"\tmake HALG=DJB2		algoritmus djb2 pouzivajici magickou 33\n"
	@printf 	"\tmake HALG=LOSELOSE	hashovaci funkce uvedena v K&R (velmi neefektivni)\n"
	@printf		"\n\tUTILITY\n"
	@printf		"\tmake clean		smaze vysledky a vystupy make (provede uklid)\n"
	@printf		"\tmake zip		zazipuje vsechny zdrojove soubory a Makefile\n"