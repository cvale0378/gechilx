#!/bin/sh

rsync -v -r -t  ~/gechilx_client/database/* ~/gechilx_client/database.local/

# se l'aggiornamento del database locale è riuscito visualizza il messaggio e va in pausa in
# attesa che premiamo un tasto
if [ "$?" = 0 ]
then
	echo "\nDatabase di locale di gechilx (directory ...database.local) aggiornato"
	exit
fi

