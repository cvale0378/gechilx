#!/bin/sh

# Esegue il link tra il file oggetto gechilx.o e le librerie gtk+-3.0 che vengono specificate dall'output del
# comando "pkg-config --libs gtk+-3.0" che viene sostituito nella riga di comando, essendo scritto tra apici inversi

# Opzioni :
# -o		nome del file eseguibile che vogliamo ottenere
g++ -o gechilx gechilx.o `pkg-config --libs gtk+-3.0`

