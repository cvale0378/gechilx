#!/bin/sh

# Compila il file gechilx.c .Il comamdo "pkg-config --cflags gtk+-3.0" scritto tra apici inversi viene sostituito dal suo output che sappiamo
# essere l'elenco delle directory con i file include per le librerie gtk+-3.0. Se va a buon fine genera il file gechilx.o
 
# Opzioni :
# -Wall		genere tutti gli avvisi di warning durante la compilazione
# -O0		nessuna ottimizzazione del codice. Serve per poter fare il debug. Se invece si vuole ottimizzare al meglio usare -O3
# -g		inserisce nel file oggetto i simboli per il debug
# -c		compila solo senza linkare
gcc -Wall -O0 -g -c gechilx.c `pkg-config --cflags gtk+-3.0`
