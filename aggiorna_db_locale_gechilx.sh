#!/bin/sh

rsync -v -r -t -p --progress --stats ~/gechilx_client/database/ ~/gechilx_client/database.local/

# se l'aggiornamento del database locale è riuscito visualizza il messaggio e va in pausa in
# attesa che premiamo un tasto
if [ "$?" = 0 ]
then
	echo "\nDatabase di locale di gechilx (directory ...database.local) aggiornato"
	echo "Premere 'invio' per continuare\n"
	read tasto
	exit
fi

# se ivece il comando rsync ha restituito errore, ce lo visualizza e va in pausa in attesa che
# premiamo un tasto
echo "\nPremere 'invio' per continuare"
read tasto
exit
