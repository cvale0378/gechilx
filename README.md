			GECHILX VERSIONE 23.10

Architettura : 64bit

Font di sistema per una visualizzazione corretta : DejaVu Sans Book 10

- Titoli schermate usato carattere Monospace Bold di 45 pixel e del colore nero , coordinate sinistra-alto della scritta 130x30.

- Schermata resoconto mese usato carattere Sans di 11 pixel.

- Colori :

			R		G		B

verde		0.067	0.514   	0.024
rosa			0.859	0.224	0.808
arancio		0.957	0.286	0.106
giallo		0.957	0.969	0.106
blu			0.106	0.145	0.969
rosso		1		0		0	
viola		0.369	0.133	0.988
marrone 		0.439	0.239	0.008	
azzurro		0.020	0.588	0.580
grigio		0.420	0.439	0.439



INSTALLAZIONE :
- Nella directory $HOME creare la cartella 'gechilx_client'.

- Al suo interno copiare l'eseguibile gechilx.

- Sempre nella cartella $HOME/gechilx_client copiare il file 'system.gec'.
 
- Sempre nella cartella $HOME/gechilx_client copiare lo script 'aggiorna_db_locale_gechilx.sh'.

- Sempre nella cartella $HOME/gechilx_client creare le cartelle 'database' e 'database.local'.

- Sempre nella cartella $HOME/gechilx_client creare una cartella 'gui' e copiare al suo interno 
  il file 'gechilx.glade' e tutti i file di immagini contenuti nella cartella 'gui' del progetto.

- Installare il client NFS 'sudo apt install nfs-common'

- Installare il terminale XTerm 'sudo apt install xterm'

- Crearsi una copia di backup del file '/etc/fstab'.

- Copiare il seguente testo (escluse le virgolette iniziali e finali nel file '/etc/fstab' (le righe che
  iniziano con '#' possono essere omesse, ma servono da spiegazione) : 
 "# fa in modo che si possa montare la directory col database di gechilx dal server 10.10.19.200
  # spiegazione opzioni di montaggio :
  # -rw	classica read write
  #
  # -user		permette il montaggio anche agli utenti non superuser , NB il comando da usare è
  # 			'mount /home/crv/gechilx_client/database' altrimenti da errore
  #
  # -noauto	non monta automaticamente il filesystem all'avvio, ma appunto bisogna usare il
  # 			comando sopra indicato
  10.10.19.200:/home/crv/gechilx_db/database	/home/crv/gechilx_client/database	nfs	rw,user,noauto	0	0"

- Sostituire '10.10.19.200:/home/crv' con il giusto indirizzo ip del server e il giusto percorso dove si trova
  la directory 'gechilx_db/database' che appunto contiene i file del database. 
  NB: è l'eseguibile gechilx che si occupa di montare automaticamente il database dal server all'avvio e di
  smontarlo all'uscita e di segnalare se ci sono errori.

- Configurare NFS server sul pc che fa da SERVER :
    1 - Installare nfs-kernel-server con 'sudo apt install nfs-kernel-server'.
    2 - Verificare che il server sia in esecuzione 'sudo systemctl status nfs-kernel-server'.
    3 - Modificare il file '/etc/exports' per indicare quale cartella vogliamo esportare e con quali attributi.
        Esempio se inseriamo la seguente riga : 
        /home/crv/gechilx_db/database      crv-pc(rw,sync,no_subtree_check) tabletmint(rw,sync,no_subtree_check)
        Esporta la cartella '/home/crv/gechilx_db/database' presente sul server per essere condivisa solo con crv-pc 
        e tabletmint in lettura e scrittura (rw) e altre opzioni.
    4 - Applicare i cambiamenti alla configurazione con 'sudo exportfs -a'.
    5 - Abilitare NFS server all'avvio automatico dopo un riavvio con 'sudo systemctl enable nfs-kernel-server'.

 

CARATTERISTICHE ATTIVE :
- Si lavora su di un database salvato sul pc che fa da server e ogni volta che
  usciamo dal programma ci chiede se vogliamo salvare una copia del database sul
  pc locale.

- Password di accesso. Modificato il meccanismo di ricerca della password.

- Registra Chiamata.

- Modifica Chiamata.

- Carica Mese.

- Lista Chiamate Mese.

- Resoconto Mese.

- Statistiche Annuali. Per la visualizzazione usa il file 'grafico4.png'.

- Database Clienti e Apparecchiature.Attivata la possibilità di aggiungere ed
  eliminare clienti e apparecchiature.

- Cambio Password.

- Modifica compensi attività.


MODIFICHE INTRODOTTE NELLA VERSIONE 23.10 : 

- RISCRITTO IL CODICE PER LA SCHERMATA LISTA CHIAMATE MESE IN MODO CHE UTILIZZI L'APPOSITO WIDGET (GtkTreeView)
  PER LA VISUALIZZAZIONE DELLA LISTA SENZA PIÙ RICORRERE AD UNA MOLTITUDINE DI GtkEntry



MODIFICHE INTRODOTTE NELLA VERSIONE 22.01 : 

- RISCRITTE LE FUNZIONI "conta_attivita" e "crea_dialog_box_due_pulsanti_con_entry" PER CORREGGERE ALCUNI BUG.

- MODIFICATO LO SCRIPT "aggiorna_db_locale_gechilx.sh" METTENDO IL COMANDO rsync AL POSTO DEL COMANDO cp.

- CORRETTO IL BUG CHE FA DUPLICARE LE LESTE CLIENTI E APPARECCHIARTURE NEI RELATIVI COMBO_BOX_TEXT DELLE
  SCHERMATE 'Registra Chiamata' E 'Modifica Chiamata' OGNI VOLTA CHE USCIAMO E RIENTRIAMO IN ESSE.



MODIFICHE INTRODOTTE NELLA VERSIONE 21.01 : 

- RISCRITTE LE FUNZIONI "pressed_button_salva_chiamata" E "pressed_button_salva_chiamata1" PERCHÈ NON SALVAVANO
  CORRETTAMENTE I CARATTERI UNICODE UTF16 E QUINDI I CARATTERI SPECIALI O LE LETTERE ACCENTATE DOPO IL 
  SALVATAGGIO NON VENIVANO VISUALIZZATE CORRETTAMENTE. ORA FUNZIONANO BENE.

- AGGIUNTA LA FINESTRA INFORMAZIONI, RICHIAMABILE DAL MENU' "AIUTO -> INFORMAZIONI".



MODIFICHE INTRODOTTE NELLA VERSIONE 20.03 : 

- LA DIRECTORY HOME DEL PROGRAMMA È $HOME/gechilx_client.

- RITORNO AD UN UNICO FILE GLADE PER LA GUI, $HOME/gechilx_client/gui/gechilx.glade.

- RIDISEGNATA TUTTA L' INTERFACCIA GRAFICA (GUI) PER FAR IN MODO CHE SI POSSA ADATTARE A MONITOR DI DIVERSE
  DIMESIONI E RISOLUZIONI.



MODIFICHE INTRODOTTE NELLA VERSIONE 18.11 : 
  
- RITORNO ALLA COLORAZIONE VERDE.

- ELIMINATO IL FILE gechilx_gui.h E AL SUO POSTO CREATI 9 FILE .h OGNUNO CONTENENTE SOLO LA
  STRINGA DELLA SCHERMATA CHE RAPPRESENTA.

- LA DIRECTORY DEL PROGRAMMA DIVENTA '$HOME/gechilx_db' PER NON DOVERLA PIÙ MODIFICARE OGNI VOLTA CHE CAMBIAMO
  VERSIONE. ALL'INTERNO DELLA DIRECTORY DEL PROGRAMMA DOBBIAMO COPIARE LA CARTELLA 'gui' CHE CONTIENE I FILE
  DELL'INTERFACCIA GRAFICA (NON SONO PIÙ IN /usr/share/gechilx...) , POI DOBBIAMO CREARE LA CARTELLA 'database'
  NELLA QUALE VERRÀ MONTATO IL DATABASE DIRETTAMENTE DAL SERVER ALL' AVVIO DEL PROGRAMMA , 
  POI DOBBIAMO CREARE LA CARTELLA 'database.local' NELLA QUALE VERRANNO COPIATI TUTTI I FILE DEL DATABASE
  CHE È SUL SERVER (E CHE ABBIAMO MONTATO IN PRECEDENZA IN '$HOME/gechilx_db/database') OGNI VOLTA CHE FACCIAMO 
  L'AGGIORNAMENTO DEL DATABASE LOCALE. LO SCRIPT 'aggiorna_db_locale_gechilx.sh'
  VA ANCHE LUI COPIATO NELLA DIRECTORY DEL PROGRAMMA ASSIEME ALL'ESEGUIBILE 'gechilx'.
  PREMENDO LA X NELL'ANGOLO DELLA FINESTRA PER USCIRE DAL PROGRAMMA APPARE UN DIALOG CHE CHIEDE SE SI VUOLE
  AGGIORNARE IL DATABASE LOCALE, IN COSA DI RISPOSTA AFFERMATIVA ESEGUE LO SCRIPT 'aggiorna_db_locale_gechilx.sh'.

- AGGIUNTA UNA SCHERMATA DI RIPRISTINO CHE APPARE ALL'AVVIO DEL PROGRAMMA DOPO CHE LA VOLTA PRECEDENTE SI ERA 
  LAVORATO SU UNA COPIA DEL DATABASE LOCALE, E CHE CI AVVISA COME FARE PER SISTEMARE LE COSE SUL SERVER.



MODIFICHE INTRODOTTE NELLA VERSIONE 16.07 : 
  
- AGGIUNTO PULSANTE "MODIFICA COMPENSI ATTIVITÀ" PER FARE IN MODO CHE SI POSSA VARIARE 
  IL COMPENSO RETRIBUITO PER OGNI ATTIVITÀ STANDARD E ANCHE L'IVA.
  I VALORI DEI COMPENSI E DELL'IVA VERRANNO SALVATI DI VOLTA IN VOLTA NEL FILE DELLE 
  CHIAMATE RELATIVO AL MESE A CUI SI RIFERISCONO. 
  PER FARE QUESTO VIENE CREATO UN NUOVO FILE compensi.gec SEMPRE FACENTE PARTE DELLA 
  DIR DATABASE, IN QUESTO FILE SONO SONO SALVATI I COMPENSI PER OGNI ATTIVITÀ STANDARD E 
  L'IVA, OGNUNO COMPOSTO DA 2 CIFRE PER I DECIMALI E 2 CIFRE PER I CENTESIMI SEPARATI DA 
  UN PUNTO. OGNI COMPENSO TERMINA CON CARATTERE NULLO QUINDI SI PUÒ TRATTARE COME UNA STRINGA.
  OGNI VOLTA CHE SI ACCEDE AL MENÙ PRINCIPALE DOPO AVER MESSO LA PASSWORD IL CONTENUTO DEL 
  FILE compensi.gec VIENE COPIATO NEL FILE CHIAMATE MESE A PARTIRE DAL BYTE 192 IN MODO DA 
  NON SOVRASCRIVERE LA PRIMA CHIAMATA CHE COMINCIA PIÙ AVANTI. IN QUESTO MODO PER OGNI 
  MESE POTREMMO AVERE COMPENSI DIVERSI CHE IL PROGRAMMA SI ANDRÀ AD ESAMINARE OGNI VOLTA. 
  OGNI VOLTA CHE MODIFICHIAMO I COMPENSI DALL'APPOSITA SCHERMATA, VERRANNO SALVATI NEL 
  FILE compensi.gec.OVVIAMENTE PER OGNI MESE FARANNO FEDE I COMPENSI DELL'ULTIMA MODIFICA. 



MODIFICHE INTRODOTTE NELLA VERSIONE 16.00 :
 
- IMPLEMENTATA LA POSSIBILITÀ DI SALVARE UNA CHIAMATA A COMPENSO NON STANDARD, CHE DECIDIAMO 
  NOI SUL MOMENTO.



MODIFICHE INTRODOTTE NELLA VERSIONE 15.10 :
 
- L' INTERFACCIA GRAFICA NON VIENE PIÙ CARICATA DAI VARI FILE GLADE MA VIENE 
  INTEGRATA DIRETTAMENTE NEL CODICE ESEGUIBILE TRAMITE STRINGHE CHE REPLICANO IL CONTENUTO 
  DEI VARI FILE GLADE E CHE SONO DICHIARATE NEL FILE HEADER
  gechilx_gui.h . IN QUESTO MODO NON C'È PIÙ BISOGNO DI TENERE UNA COPIA DEI FILE .GLADE
  NELLA $HOME/gechilx$VERSION/gui MA BASTA TENERLI NELLA DIRECTORY DEL PROGETTO PER LE 
  MODIFICHE FUTURE.
 
- LA DIRECTORY 'gui' CHE CONTIENE I FILE DELL'INTERFACCIA GRAFICA DEL PROGRAMMA VA COPIATA 
  IN /usr/share/gechilx. INFATTI D'ORA IN AVANTI LE IMMAGINI SARANNO DEFINITE IN 
  gechilx_gui.h COME APPARTENENTI ALLA DIRECTORY /usr/share/gechilx/gui.



MODIFICHE INTRODOTTE NELLA VERSIONE 15.05 :
  
- IMPLEMENTATA FUNZIONE DEL CAMBIO PASSWORD DI ACCESSO.



MODIFICHE INTRODOTTE NELLA VERSIONE 14.09 :
  
- TUTTE LE FINESTRE TRANNE QUELLA DEL MENÙ PRINCIPALE VENGONO DISTRUTTE E NON PIÙ SOLO 
  NASCOSTE  QUANDO PREMIAMO IL PULSANTE INDIETRO. OVVIAMENTE VERRANNO RICREATE OGNI VOLTA 
  CHE LE SELEZIONIAMO.

- INOLTRE TUTTI I FILE DELLE IMMAGINI E DELL'INTERFACCIA GRAFICA ANDRANNO NELLA DIRECTORY 
  $HOME/gechilx$VERSION/gui E NON CI SARÀ PIÙ UN UNICO FILE .GLADE PER L'INTERFACCIA GRAFICA, 
  MA OGNI WINDOW AVRA' IL SUO FILE .GLADE (la window_accesso e la window_menu_princ sono in 
  un solo file .glade chiamato menu.glade).
  
- LA DIRECTORY PRINCIPALE NON SARÀ PIÙ $HOME/gechilx MA SARÀ %HOME/gechilx$VERSION 
  CHE CORRISPONDE ALLA VERSIONE DEL PROGRAMMA.
 
