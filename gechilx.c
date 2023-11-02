/*
 * File:   gechilx.c
 * Author: crv
 *
 * 
 * Utilizza librerie GTK+-3.0
 */

/* GECHILX VERSIONE 23.10 64BIT */

/* INFORMAZIONI E PROCEDURA DI INSTALLAZIONE CONTENUTE NEL FILE README.md */
 
  
#define     VERSION	"23.10"
#define     ARCH	"64"	/* architettura cpu in char */
#define     ARCH_NUM	64	/* architettura cpu in int  */

#include <gtk-3.0/gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <glib/gprintf.h>
#include "gechilx.h"

/****
***
***	VARIABILI GLOBALI
***
****/

GtkBuilder  *builder;

gchar file_config_path[MAXPATHLEN];		// percorso del file 'system.gec'
gchar file_chiamate_mese_path[MAXPATHLEN * 2];	// percorso del file chiamate aperto
gchar file_clienti_path[MAXPATHLEN];		// percorso del file clienti.gec
gchar file_appar_path[MAXPATHLEN];		// percorso del file appar.gec
gchar file_compensi_path[MAXPATHLEN * 2];	// percorso del file compensi.gec
gchar file_gui_path[MAXPATHLEN];		// percorso del file per l'interfaccia grafica(gechilx.glade)
gchar program_dir[MAXPATHLEN];			// directory del programma
gchar database_dir[MAXPATHLEN];			// directory del database
gchar gui_dir[MAXPATHLEN];			// directory della gui
gchar comando[MAXPATHLEN];			// stringa per i comandi bash

const gchar *home_dir;				// user home directory
const gchar *str_giorno_settimana_lett;
const gchar *str_mese_lett;

gboolean database_server_montato;	// diventa TRUE se riesce a montare il database dal server
gboolean database_locale;		// diventa TRUE se stiamo lavorando su una copia del database locale
gboolean modalita_ripristino;		// diventa TRUE se il programma si è aperto in modalita ripristino, quando la volta precedente si era lavorato in locale
gboolean esci_subito;			// diventa TRUE se non è possibile montare il database di rete e non vogliamo neanche usare il database locale
gboolean cancel;			// diventa TRUE se premiamo Cancel nei dialog box a due pulsanti con entry

int giorno;
int mese;
int anno;
int anno_caricato;
int mese_caricato;
int num_chiamata_selezionata;
int schermata_corrente;				// serve a sapere la schermata correntemente visualizzata

GtkWindow 	*window_principale, *window_modifica_chiamata, *window_informazioni;

GtkMenuBar	*barra_menu;

GtkWidget	*menu_aiuto_informazioni, *menu_file_esci, *menu_file_apri;

GtkLabel	*label_titolo, *label_versione, *label_descrizione_accesso;
					
GtkBox		*box_accedi, *box_menu_princ, *box_reg_chiamata, *box_modifica_chiamata, *box_lista_ch_mese,
		*box_clienti_appar, *box_resoconto_mese, *box_statistiche_annuali, *box_cambio_pw, 
		*box_mod_comp_att, *box_ripristino;

GtkDialog   *dialog_fceom, *dialog_pwerr, *dialog_icoafc, *dialog_iafc, *dialog_elimina, *dialog_copia_dati, *dialog_flag, *dialog_agg_appar, *dialog_agg_cliente,
		*dialog_vecchia_pw_err, *dialog_nuova_pw_err, *dialog_scrittura_nuova_pw_err, *dialog_conferma_cambio_pw, *dialog_comp_err, *dialog_aggiorna_db, 
		*dialog_db_server_no;

GtkViewport	*viewport;

GtkEntry	*entry_password,
		*entry_cliente, *entry_cliente1,
		*entry_appar, *entry_appar1,
		*entry_gg, *entry_gg1,
		*entry_mm, *entry_mm1,
		*entry_aaaa, *entry_aaaa1,
		*entry_ora_in_spost, *entry_ora_in_spost1,
		*entry_ora_in_lav, *entry_ora_in_lav1,
		*entry_ora_fine_lav, *entry_ora_fine_lav1,
		*entry_luogo, *entry_luogo1,
		*entry_guasto, *entry_guasto1,
		*entry_lav_eseguito, *entry_lav_eseguito1,
		*entry_ore_lav, *entry_ore_lav1,
		*entry_vecchia_pw, *entry_nuova_pw, *entry_conf_nuova_pw,
		*entry_compenso, *entry_compenso_cent,
		*entry_compenso1, *entry_compenso_cent1,
		*entry_comp_ch_t, *entry_comp_ch_t_dec,
		*entry_comp_ch_tl, *entry_comp_ch_tl_dec,
		*entry_comp_uvt, *entry_comp_uvt_dec,
		*entry_comp_ch_p, *entry_comp_ch_p_dec,
		*entry_comp_dis_p, *entry_comp_dis_p_dec,
		*entry_comp_uvp, *entry_comp_uvp_dec,
		*entry_comp_zb, *entry_comp_zb_dec,
		*entry_comp_zc, *entry_comp_zc_dec,
		*entry_comp_lav_ore, *entry_comp_lav_ore_dec,
		*entry_comp_iva, *entry_anno_scelto, *entry_dato,
		*entry_versione;

GtkWidget	*entry_agg_appar, *entry_agg_cliente;

GtkButton 	*button_accedi, *button_reg_chiamata, *button_resoc_mese, *button_carica_mese,
		*button_indietro, *button_copia_dati, *button_salva_chiamata, *button_salva_chiamata1, 
		*button_annulla1, *button_lista_ch_mese,  *button_elimina_ch, *button_modifica_ch, *button_database, 
		*button_agg_cliente, *button_agg_appar, *button_elimina_appar, *button_elimina_cliente,
		*button_stat_annuali, *button_aggiorna, *button_cambio_pw, *button_ok, *button_mod_comp_att, 
		*button_salva_comp_att, *button_esci, *button_chiudi_informazioni;

GtkCheckButton	*check_za, *check_za1,
		*check_zb, *check_zb1,
		*check_zc, *check_zc1,
		*check_cht, *check_cht1,
		*check_chtl, *check_chtl1,
		*check_uvt, *check_uvt1,
		*check_chp, *check_chp1,
		*check_dis, *check_dis1,
		*check_uvp, *check_uvp1,
		*check_compenso, *check_compenso1;

GtkComboBoxText		*combo_box_text_cliente, *combo_box_text_appar, *combo_box_text_cliente1, *combo_box_text_appar1, *combo_box_text_anno;

GtkImage	*image_resoconto_mese, *image_stat_annuali;

GtkRadioButton*	radio_button_array[MAX_NUM_CH_MESE];

GtkTextBuffer	*textbuffer_data, *textbuffer_mese;

GtkTreeView	*view_clienti, *view_appar, *view_lista_ch_mese;

GtkScrolledWindow	*scr_window_lista_ch_mese, *scr_window_clienti, *scr_window_appar;

GFile			*file_chiamate_mese, *file_appar, *file_clienti, *file_config, *file_compensi;

GFileIOStream	*file_chiamate_mese_iostream, *file_appar_iostream, *file_clienti_iostream, *file_config_iostream, *file_compensi_iostream;

goffset		file_chiamate_mese_size, file_appar_size, file_clienti_size, file_config_size, file_compensi_size;

GString		*pw_corrente;

GList		*elenco_clienti, *elenco_clienti_ordinato, *elenco_apparecchiature, *elenco_apparecchiature_ordinato;

/****
 ***
 ***    PROTOTIPI DI FUNZIONI
 ***
 ****/
GList* crea_elenco_clienti(GList *list);
GList* crea_elenco_appar(GList *list);



/****
 ***
 ***    FUNZIONI GENERALI
 ***
 ****/

/* crea un dialog box ad un pulsante, che viene chiuso quando premiamo il pulsante stesso.
 * Parametri della funzione :
 * dialog -> identificativo del dialog box
 * titolo_dialog -> testo nella barra titolo del dialog box
 * messaggio_dialog -> testo della richiesta del dialog box
 * parent_window -> finestra a cui appartiene il dialog box
 * testo_pulsante -> testo del pulsante del dialog */
void crea_dialog_box_un_pulsante(GtkDialog* dialog, const gchar* titolo_dialog, const gchar* messaggio_dialog, GtkWindow* parent_window, const gchar* testo_pulsante)
{
	GtkWidget *label, *content_area;

	/* crea un dialog che si chiude alla pressione del pulsente un esso contenuto */
	dialog = GTK_DIALOG(gtk_dialog_new_with_buttons(titolo_dialog, parent_window,
				GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
				testo_pulsante, GTK_RESPONSE_CLOSE, NULL));

	/* crea nel dialog l'area adibita a contenere il messaggio, e in essa inserisce la label del messaggio */       
	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	label = gtk_label_new(messaggio_dialog);
	gtk_container_add(GTK_CONTAINER(content_area), label);

	/* visualizza il dialog ed esegue il loop del dialog */ 
	gtk_widget_show_all((GtkWidget*)dialog);
	gtk_dialog_run((GtkDialog*)dialog);

	/* una volta premuto il pulsante del dialog, esce dal loop e distrugge il dialog */
	gtk_widget_destroy((GtkWidget*)dialog);

	return;
}


/* crea un dialog box a due pulsanti . Parametri della funzione :
 * dialog -> identificativo del dialog box
 * titolo_dialog -> testo nella barra titolo del dialog box
 * messaggio_dialog -> testo della richiesta del dialog box
 * parent_window -> finestra a cui appartiene il dialog box
 * testo_pulsante1 -> testo del primo pulsante del dialog box a partire da sinistra (solitamente riposta affermativa)
 * testo_pulsante2 -> testo del secondo pulsante del dialog box a partire da sinistra (solitamente riposta negativa)
 * *p_funz_da_eseguire_aff() -> puntatore alla funzione da eseguire in caso di risposta affermativa
 * *p_funz_da_eseguire_neg() -> puntatore alla funzione da eseguire in caso di risposta negativa */
gboolean crea_dialog_box_due_pulsanti(GtkDialog* dialog, const gchar* titolo_dialog, const gchar* messaggio_dialog, 
					GtkWindow* parent_window, const gchar* testo_pulsante1, const gchar* testo_pulsante2,
					void (*p_funz_da_eseguire_aff)(), void (*p_funz_da_eseguire_neg)())
{
	GtkWidget *label, *content_area;
	gint result;

	/**** IMPOSTA IL DIALOG ****/
	dialog = GTK_DIALOG(gtk_dialog_new_with_buttons(
						titolo_dialog,
						parent_window,
						GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
						testo_pulsante1, GTK_RESPONSE_YES,
						testo_pulsante2, GTK_RESPONSE_NO,
						NULL));

	/* crea una content area per visualizzare il messaggio del dialog e una label col messaggio stesso che verrà inserita nella content area */
	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	label = gtk_label_new(messaggio_dialog);

	/* aggiunge la label contenente il messaggio da visualizzare nel dialog alla content area */
	gtk_container_add(GTK_CONTAINER(content_area), label);
	
	/* toglie il pulsante di chiusua X dal dialog */
	gtk_window_set_deletable((GtkWindow*)dialog, FALSE);
	
	/**** FINE IMPOSTAZIONE DIALOG ****/

	/* visualizza il dialog ed entra in un loop che termina solo quando interagiamo col dialog. 
	* Abbiamo 3 possibili valori per result :
	* GTK_RESPONSE_DELETE_EVENT se è premuto pulsante di chiusura del dialog
	* GTK_RESPONSE_NO se è premuto pulsante NO
	* GTK_RESPONSE_YES se è premuto YES, quest'ultimi 2 sono impostati da noi durante la creazione del dialog. */
	gtk_widget_show_all((GtkWidget*)dialog);
	result = gtk_dialog_run((GtkDialog*)dialog);

	/* se viene premuto il pulsante di chiusura X del dialog esso ritorna un GTK_RESPONSE_DELETE_EVENT, ma non viene chiuso, quindi lo distruggiamo noi.
	* Questo primo if è superfluo perchè ho fatto in modo che il dialog venisse creato senza il pulsante di chiusura, ma lo lascio per chiarezza */
	if(result == GTK_RESPONSE_DELETE_EVENT)
	{
		gtk_widget_destroy((GtkWidget*)dialog);
		return FALSE;
	}
	/* se viene premuto pulsante2 (solitamente risposta negativa), distrugge il dialog  ed esegue la funzione richiesta */
	if(result == GTK_RESPONSE_NO)
	{
		gtk_widget_destroy((GtkWidget*)dialog);
		p_funz_da_eseguire_neg();
	}

	/* se viene premuto pulsante1 (solitamente risposta affermativa), distrugge il dialog ed esegue la funzione richiesta */
	if(result == GTK_RESPONSE_YES)
	{
		gtk_widget_destroy((GtkWidget*)dialog);
		p_funz_da_eseguire_aff();
	}

	return FALSE;
}


/* crea un dialog box con due pulsanti e un entry. La stringa che passiamo come ultimo parametro (str_entry) verrà inizializzata col testo che abbiamo inserito 
 * nell'entry nel caso venga premuto il pulsante OK . 
 * In caso venga premuto il pulsante No (o Cancel o quello che è) imposta a TRUE la variabile globale 'cancel' che serve per fare sapere alla funzione della 
 * quale è stato invocato il dialog che non deve fare niente perchè non è stata fatta nessuna modifica.
 * Parametri della funzione :
 * dialog -> identificativo del dialog box
 * titolo_dialog -> testo nella barra titolo del dialog box
 * messaggio_dialog -> testo della richiesta del dialog box
 * parent_window -> finestra a cui appartiene il dialog box
 * entry -> entry del dialog box
 * max_char_entry -> numero massimo di caratteri che si possono inserire nell' entry
 * testo_pulsante1 -> testo del primo pulsante del dialog box a partire da sinistra (solitamente riposta affermativa)
 * testo_pulsante2 -> testo del secondo pulsante del dialog box a partire da sinistra (solitamente riposta negativa)
 * str_entry -> stringa che conterrà il testo che abbiamo inserito nell'entry */
gboolean crea_dialog_box_due_pulsanti_con_entry(GtkDialog* dialog, const gchar* titolo_dialog, 
								const gchar* messaggio_dialog, GtkWindow* parent_window,
								GtkWidget* entry, int max_char_entry,
								const gchar* testo_pulsante1, const gchar* testo_pulsante2,
								gchar* str_entry)
{
	const gchar *testo_entry;
	GtkWidget *label, *content_area;
	gint result;

	dialog = GTK_DIALOG(gtk_dialog_new_with_buttons(
						titolo_dialog,
						parent_window,
						GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
						testo_pulsante1, GTK_RESPONSE_YES,
						testo_pulsante2, GTK_RESPONSE_NO,
						NULL));


	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	label = gtk_label_new(messaggio_dialog);

	entry = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(entry), max_char_entry);
	gtk_entry_set_width_chars(GTK_ENTRY(entry), max_char_entry);

	gtk_container_add(GTK_CONTAINER(content_area), label);
	gtk_container_add(GTK_CONTAINER(content_area), entry);
	gtk_widget_show_all((GtkWidget*)dialog);
	result = gtk_dialog_run((GtkDialog*)dialog);

	if(result == GTK_RESPONSE_DELETE_EVENT)
	{
		gtk_widget_destroy((GtkWidget*)dialog);
		cancel = TRUE;
		return FALSE;
	}
	if(result == GTK_RESPONSE_NO)
	{
		gtk_widget_destroy((GtkWidget*)dialog);
		cancel = TRUE;
		return FALSE;
	}

	if(result == GTK_RESPONSE_YES)
	{
		testo_entry = gtk_entry_get_text(GTK_ENTRY(entry));
		strcpy(str_entry, testo_entry);
		gtk_widget_destroy((GtkWidget*)dialog);
	}

	return TRUE;
}


/* non fa assolutamente niente, serve in alcuni casi per non assegnare un puntatore nullo all'ultimo parametro della funzione crea_dialog_box_due_pulsanti,
 * precisamente quello in cui dobbiamo passare il puntatore della funzione da eseguire in caso di risposta negativa */
void non_fare_nulla()
{
	return;
}


/* aggiorna_db_locale è la funzione che viene chiamata se rispondiamo in maniera affermativa alla richiesta 
* di aggiornare il database locale ,che ci viene fatta tramite il dialog box creato dalla seguente funzione  crea_dialog_box_due_pulsanti */
void aggiorna_db_locale()
{
	g_sprintf(comando, "xterm %s/aggiorna_db_locale_gechilx.sh", program_dir);
	system(comando);
}


/* esce dal programma */
void uscita_normale()
 {
	 if(modalita_ripristino)
	{
		/* apre il file di configurazione 'system.gec' in lettura e per future modifiche (r+) e ne ottiene il puntatore allo stream */
		FILE *stream_file_config;
		stream_file_config = fopen(file_config_path, "r+");

		/* scrive '0' nel primo byte del file 'system.gec' */
		fseek(stream_file_config, 0, SEEK_SET);
		fputc('0', stream_file_config);

		fclose(stream_file_config);

		gtk_main_quit();
	}

	/* non visualizza il dialog di richiesta di aggiornamento del database locale se non siamo ancora entrati con la password o se siamo in modalità 
	 * ripristino */ 
	if(!esci_subito)
	{
		crea_dialog_box_due_pulsanti(dialog_aggiorna_db, "ATTENZIONE!", "\n   Vuoi aggiornare il database locale ?  \n", 
									window_principale, "Si", "No", aggiorna_db_locale, non_fare_nulla);
	}

	/* se stiamo lavorando su una copia del database locale imposta a 1 il primo byte del file 'system.gec' in modo che al prossimo avvio del programma,
	* controllando quel byte sapremo se la volta precedente abbiamo lavorato sulla copia del database locale. Se cosi fosse il programma ci 
	* avviserà e ci dirà di sistemare le cose */
	if(database_locale)
	{
		/* apre il file di configurazione 'system.gec' in lettura e per future modifiche (r+) e ne ottiene il puntatore allo stream */
		FILE *stream_file_config;
		stream_file_config = fopen(file_config_path, "r+");

		/* scrive '1' nel primo byte del file 'system.gec' */
		fseek(stream_file_config, 0, SEEK_SET);
		fputc('1', stream_file_config);

		fclose(stream_file_config);

		gtk_main_quit();
	}

	// scollega il database smontando la condivisione col server
	g_sprintf(comando, "umount %s", database_dir);
	system(comando);

	gtk_main_quit();
}


void uscita_forzata()
{
	esci_subito = TRUE;

	// scollega il database smontando la condivisione col server
	g_sprintf(comando, "umount %s", database_dir);
	system(comando);

	gtk_widget_destroy((GtkWidget*)window_principale);
	gtk_main_quit();
}


/* ordina alfabeticamente una lista di stringhe */
GList* ordina_alfabeticamente(guint n_elem_lista, GList* lista, GList* lista_ordinata)
{
	int i;
	gchar string_array[n_elem_lista][MAXSTRLEN];
	
	/* crea un' array bidimensionale per contenere tutte le stringhe della lista e lo inizializza con tutti zeri */
	memset(string_array, 0, n_elem_lista * MAXSTRLEN);

	/* riempie l'array con le stringhe di ogni elemento della lista nell'ordine in cui si trovano */
	for(i = 0; i <= (n_elem_lista -1); i++) {
		strcpy(&string_array[i][0], lista->data);
		lista = lista->next;
	}

	/* algoritmo di bubble sort adattato alle stringhe invece che ai numeri che ordina alfabeticamente le stringhe dell'array */
	gchar temp_string[MAXSTRLEN];
	int r;
	memset(temp_string, 0, MAXSTRLEN);
	int k;
	for(i = n_elem_lista - 2; i >= 0; i--) 
	{
		for(k = 0; k <= i; k++) 
		{
			r = strcmp(&string_array[k][0], &string_array[k + 1][0]);
			if(r > 0) 
			{
				memset(temp_string, 0, MAXSTRLEN);
				strcpy(temp_string, &string_array[k][0]);

				memset(&string_array[k][0], 0, MAXSTRLEN);
				strcpy(&string_array[k][0], &string_array[k + 1][0]);

				memset(&string_array[k + 1][0], 0, MAXSTRLEN);
				strcpy(&string_array[k + 1][0], temp_string);
			}
		}
	} 

	/* copia l'array di stringhe in una zona alloccata di memoria in modo che all'uscita di questa funzione questa zona di memoria non venga contaminata e 
	 * che quindi la lista ordinata non punti a stringhe sballate */
	gpointer *alloccated_mem;
	alloccated_mem = g_slice_copy(n_elem_lista * MAXSTRLEN, &string_array[0][0]);

	/* crea la lista ordinata alfabeticamente inserendo una alla volta le stringhe dell'array che abbiamo ordinato 
	* con l'algoritmo di bubble sort e copiato nella zona di memoria alloccata */
	for(i = 0; i < n_elem_lista; i++) 
	{
		lista_ordinata = g_list_append(lista_ordinata, &alloccated_mem[i * MAXSTRLEN / (ARCH_NUM / 8)]); 
							/* NB!: il dividendo (ARCH_NUM / 8) varia in base alla versione 32 o 64 bit.*/ 
	}

	return lista_ordinata;
}


/* copia_db_locale è la funzione che viene chiamata se rispondiamo in maniera affermativa alla richiesta di proseguire su di
* una copia del database locale ,che ci viene fatta tramite il dialog box creato dalla seguente funzione crea_dialog_box_due_pulsanti */
void copia_db_locale()
{
	// copia il database locale (cartella 'database.local') nella cartella database per poterci lavorare
	g_sprintf(comando, "cp %s/database.local/* %s/database", program_dir, program_dir);
	if(!(system(comando))) database_locale = TRUE;	/* se la copia del database locale dalla directory 'database.local' alla directory 'database' è andata
							 * a buon fine, setta la variabile 'database_locale' a TRUE che servirà in seguito per sapere che stiamo
							 * lavorando su una copia del database locale e non  sul server. */
	return;
}


/* converte una stringa di numeri nell'intero corrispondente, max 9 cifre */
int str_to_int(gchar *str)
{
	gchar s[20];
	strcpy(s,str);
	int strLenght = strlen(s);
	int n;
	switch (strLenght)
	{
		case 1: n = (s[0]-0x30); break;
		case 2: n = (s[0]-0x30)*10 + (s[1]-0x30); break;
		case 3: n = (s[0]-0x30)*100 + (s[1]-0x30)*10 + (s[2]-0x30); break;
		case 4: n = (s[0]-0x30)*1000 + (s[1]-0x30)*100 + (s[2]-0x30)*10 + (s[3]-0x30); break;
		case 5: n = (s[0]-0x30)*10000 + (s[1]-0x30)*1000 + (s[2]-0x30)*100 + (s[3]-0x30)*10 + (s[4]-0x30); break;
		case 6: n = (s[0]-0x30)*100000 + (s[1]-0x30)*10000 + (s[2]-0x30)*1000 + (s[3]-0x30)*100 + (s[4]-0x30)*10 + (s[5]-0x30); break;
		case 7: n = (s[0]-0x30)*1000000 + (s[1]-0x30)*100000 + (s[2]-0x30)*10000 + (s[3]-0x30)*1000 + (s[4]-0x30)*100 + (s[5]-0x30)*10 + (s[6]-0x30); break;
		case 8: n = (s[0]-0x30)*10000000 + (s[1]-0x30)*1000000 + (s[2]-0x30)*100000 + (s[3]-0x30)*10000 + (s[4]-0x30)*1000 + (s[5]-0x30)*100 + 
			    (s[6]-0x30)*10 + (s[7]-0x30); break;
		case 9: n = (s[0]-0x30)*100000000 + (s[1]-0x30)*10000000 + (s[2]-0x30)*1000000 + (s[3]-0x30)*100000 + (s[4]-0x30)*10000 + (s[5]-0x30)*1000 +
			    (s[6]-0x30)*100 + (s[7]-0x30)*10 + (s[8]-0x30); break;
	}

	return n;
}


/* converte un intero (da 0 a 4294967295) nella stringa corrispondente     */
GString *positive_int_to_string(unsigned int i)
{
	GString *string;
	string = g_string_new("0000000000"); 

	int cifra;
	unsigned int minuendo;
	minuendo = 0;

	/* converte le cifre dell'intero i una alla volta (a partire da quella di esponente maggiore  e le mette nella giusta posizione della stringa string */
	cifra = i / 1000000000;
	string->str[0] = 0x30 + cifra;

	minuendo = minuendo + (cifra * 1000000000);
	cifra = (i - minuendo)  / 100000000;
	string->str[1] = 0x30 + cifra;

	minuendo = minuendo + (cifra * 100000000);
	cifra = (i - minuendo)  / 10000000;
	string->str[2] = 0x30 + cifra;

	minuendo = minuendo + (cifra * 10000000);
	cifra = (i - minuendo)  / 1000000;
	string->str[3] = 0x30 + cifra;

	minuendo = minuendo + (cifra * 1000000);
	cifra = (i - minuendo)  / 100000;
	string->str[4] = 0x30 + cifra;

	minuendo = minuendo + (cifra * 100000);
	cifra = (i - minuendo)  / 10000;
	string->str[5] = 0x30 + cifra;

	minuendo = minuendo + (cifra * 10000);
	cifra = (i - minuendo)  / 1000;
	string->str[6] = 0x30 + cifra;

	minuendo = minuendo + (cifra * 1000);
	cifra = (i - minuendo)  / 100;
	string->str[7] = 0x30 + cifra;

	minuendo = minuendo + (cifra * 100);
	cifra = (i - minuendo)  / 10;
	string->str[8] = 0x30 + cifra;

	minuendo = minuendo + (cifra * 10);
	cifra = (i - minuendo)  / 1;
	string->str[9] = 0x30 + cifra;

	/* se otteniamo una stringa tipo "000002013" la riduce a "2013" */
	gchar *new_string_address;
	new_string_address = &(string->str[0]);
	int n;
	for(n = 0; n < 9; n++)
	{
		if(string->str[0] == 0x30)
		{
			new_string_address++;
			string = g_string_new(new_string_address);
		}
		else
			break;
	}

	return string;
}


/* ritorna il nome del mese ricevendo in ingresso il numero corrispondente al mese */
const gchar *num_to_mese(int nmese)
{
	const gchar *m = "Gennaio";
	switch(nmese)
	{
		case 0: m = "Gennaio"; break;
		case 1: m = "Febbraio"; break;
		case 2: m = "Marzo"; break;
		case 3: m = "Aprile"; break;
		case 4: m = "Maggio"; break;
		case 5: m = "Giugno"; break;
		case 6: m = "Luglio"; break;
		case 7: m = "Agosto"; break;
		case 8: m = "Settembre"; break;
		case 9: m = "Ottobre"; break;
		case 10: m = "Novembre"; break;
		case 11: m = "Dicembre"; break;
	}

	return m;
}


/* ritorna il nome del giorno della settimana ricevendo in ingresso il numero corrispondente */
const gchar *num_to_giorno(int ngiorno)
{
	const gchar *g = "Lun";
	switch(ngiorno)
	{
		case 1: g = "Lunedi"; break;
		case 2: g = "Martedi"; break;
		case 3: g = "Mercoledi"; break;
		case 4: g = "Giovedi"; break;
		case 5: g = "Venerdi"; break;
		case 6: g = "Sabato"; break;
		case 0: g = "Domenica"; break;
	}

	return g;
}


/* nasconde la schermata corrente e visualizza quella scelta */
void cambia_schermata(GtkBox *box_corr, GtkBox *box_scelto)
{
	/* nasconde il pulsante 'indietro' e lo visualizza solo se serve nellla schermata che sarà visualizzata */
	gtk_widget_hide((GtkWidget*)button_indietro);

	/* visualizza la barre dei menù */
	gtk_widget_show((GtkWidget*)barra_menu);

	/* setta vari paramatri in base alla schermata che sarà visualizzata */
	if(box_scelto == box_menu_princ)
	{
		gtk_widget_grab_focus((GtkWidget*)button_reg_chiamata);	/* appena cambia la schermata, il focus è sul pulsante 'Registra chiamata' */
		gtk_label_set_text(label_titolo, "Menù Principale");
		schermata_corrente = SCHERMATA_MENU_PRINC;
	}
	if(box_scelto == box_cambio_pw)
	{
		gtk_widget_grab_focus((GtkWidget*)entry_vecchia_pw);
		gtk_label_set_text(label_titolo, "Cambio Password");
		gtk_widget_show((GtkWidget*)button_indietro);
		schermata_corrente = SCHERMATA_CAMBIO_PW;
	}
	if(box_scelto == box_clienti_appar)
	{
		gtk_label_set_text(label_titolo, "Database Clienti e Apparecchiature");
		gtk_widget_show((GtkWidget*)button_indietro);
		schermata_corrente = SCHERMATA_DATABASE_CL_APP;
	}
	if(box_scelto == box_lista_ch_mese)
	{
		gtk_label_set_text(label_titolo, "Lista Chiamate Mese");
		gtk_widget_show((GtkWidget*)button_indietro);
		gtk_widget_show((GtkWidget*)button_elimina_ch);
		gtk_widget_show((GtkWidget*)button_modifica_ch);
		schermata_corrente = SCHERMATA_LISTA_CH_MESE;
	}
	if(box_scelto == box_modifica_chiamata)
	{
		gtk_label_set_text(label_titolo, "Modifica Chiamata");
		gtk_widget_show((GtkWidget*)button_indietro);
		schermata_corrente = SCHERMATA_MOD_CH;
	}
	if(box_scelto == box_mod_comp_att)
	{
		gtk_widget_grab_focus((GtkWidget*)entry_comp_ch_t);
		gtk_label_set_text(label_titolo, "Modifica Compensi Attività");
		gtk_widget_show((GtkWidget*)button_indietro);
		schermata_corrente = SCHERMATA_MOD_COMP_ATT;
	}
	if(box_scelto == box_reg_chiamata)
	{
		gtk_widget_grab_focus((GtkWidget*)entry_ora_in_spost);
		gtk_label_set_text(label_titolo, "Registra Chiamata");
		gtk_widget_show((GtkWidget*)button_indietro);
		schermata_corrente = SCHERMATA_REG_CH;
	}
	if(box_scelto == box_resoconto_mese)
	{
		gtk_label_set_text(label_titolo, "Resoconto Mese");
		gtk_widget_show((GtkWidget*)button_indietro);
		schermata_corrente = SCHERMATA_RESOCONTO;
	}
	if(box_scelto == box_statistiche_annuali)
	{
		gtk_label_set_text(label_titolo, "Statistiche Annuali");
		gtk_widget_show((GtkWidget*)button_indietro);
		schermata_corrente = SCHERMATA_STAT_ANN;
	}
 
	gtk_widget_hide((GtkWidget*)box_corr); 

	gtk_widget_show_all((GtkWidget*)box_scelto);
	
	return;
}


/* ritorna la dimensione del file chiamate mese */
goffset get_file_ch_mese_size()
{
	/* ottiene dimensione file chiamate mese , creando un oggetto GFileInfo , poi lo riempie con le info sul file chiamate mese facendo una query. 
	 * In questo caso scegliamo come info solo la dimensione(G_FILE_ATTRIBUTE_STANDARD_SIZE) del file. Infine per ottenere la dimensione dalle 
	 * info usiamo la g_file_info_get_size. */
	GFileInfo *file_info;

	file_info = g_file_info_new();

	file_chiamate_mese = g_file_new_for_path(file_chiamate_mese_path);
	file_info = g_file_query_info(file_chiamate_mese, G_FILE_ATTRIBUTE_STANDARD_SIZE, G_FILE_QUERY_INFO_NONE, NULL, NULL);

	return g_file_info_get_size(file_info);
}


/* ritorna la dimensione del file  appar.gec */
goffset get_file_appar_size()
{
	/* ottiene dimensione file appar.gec , creando un oggetto GFileInfo ,poi lo riempie con le info sul file chiamate mese facendo una query. 
	 * In questo caso scegliamo come info solo la dimensione(G_FILE_ATTRIBUTE_STANDARD_SIZE) del file. Infine per ottenere la dimensione dalle 
	 * info usiamo la g_file_info_get_size. */
	GFileInfo *file_info;

	file_info = g_file_info_new();

	file_appar = g_file_new_for_path(file_appar_path);
	file_info = g_file_query_info(file_appar, G_FILE_ATTRIBUTE_STANDARD_SIZE, G_FILE_QUERY_INFO_NONE, NULL, NULL);

	return g_file_info_get_size(file_info);
}


/*ritorna la dimensione del file  clienti.gec */
goffset get_file_clienti_size()
{
	/* ottiene dimensione file clienti.gec , creando un oggetto GFileInfo ,poi lo riempie con le info sul file chiamate mese facendo una query. 
	 * In questo caso scegliamo come info solo la dimensione(G_FILE_ATTRIBUTE_STANDARD_SIZE) del file. Infine per ottenere la dimensione dalle 
	 * info usiamo la g_file_info_get_size. */
	GFileInfo *file_info;

	file_info = g_file_info_new();

	file_clienti = g_file_new_for_path(file_clienti_path);
	file_info = g_file_query_info(file_clienti, G_FILE_ATTRIBUTE_STANDARD_SIZE, G_FILE_QUERY_INFO_NONE, NULL, NULL);

	return g_file_info_get_size(file_info);
}


/* ritorna la dimensione del file di configurazione system.gec */
goffset get_file_config_size()
{
	GFileInfo *file_info;

	file_info = g_file_info_new();

	file_config = g_file_new_for_path(file_config_path);
	file_info = g_file_query_info(file_config, G_FILE_ATTRIBUTE_STANDARD_SIZE, G_FILE_QUERY_INFO_NONE, NULL, NULL);

	return g_file_info_get_size(file_info);
}


/* ritorna la dimensione del file compensi.gec */
goffset get_file_compensi_size()
{
	GFileInfo *file_info;

	file_info = g_file_info_new();

	file_compensi = g_file_new_for_path(file_compensi_path);
	file_info = g_file_query_info(file_compensi, G_FILE_ATTRIBUTE_STANDARD_SIZE, G_FILE_QUERY_INFO_NONE, NULL, NULL);

	return g_file_info_get_size(file_info);
}


/* apre in lettura e scrittura il file chiamate mese ne legge il contenuto e lo copia nel buffer passato come argomento e ritorna il numero di bytes letti */
gssize open_rw_file_ch_mese_read(gchar* buffer)
{
	/* apre in lettura e scrittura il file chiamate mese */
	file_chiamate_mese = g_file_new_for_path(file_chiamate_mese_path);
	file_chiamate_mese_iostream = g_file_open_readwrite(file_chiamate_mese, NULL, NULL);

	GInputStream *input_stream;
	input_stream = g_io_stream_get_input_stream((GIOStream*) file_chiamate_mese_iostream);

	/* legge il contenuto del file chiamate mese e lo copia in buffer */
	gssize bytes_letti;
	bytes_letti = g_input_stream_read(input_stream, buffer, file_chiamate_mese_size, NULL, NULL);

	/* chiude lo stream per la lettura */
	g_input_stream_close(input_stream, NULL, NULL);

	/* chiude lo stream per l'apertura del file */
	g_io_stream_close((GIOStream*)file_chiamate_mese_iostream, NULL, NULL);

	return bytes_letti;
}


/* apre in lettura e scrittura il file appar.gec ne legge il contenuto e lo copia nel buffer passato come argomento e ritorna il numero di bytes letti  */
 gssize open_rw_file_appar_read(gchar* buffer)
{
	GInputStream *input_stream;
	gssize bytes_letti;

	file_appar = g_file_new_for_path(file_appar_path);

	/* apre per leggere o scrivere il file appar.gec */
	file_appar_iostream = g_file_open_readwrite(file_appar, NULL, NULL);

	/* stream per leggere file appar.gec */
	input_stream = g_io_stream_get_input_stream((GIOStream*)file_appar_iostream);

	/* legge il file appar.gec e copia il contenuto in buffer */
	bytes_letti = g_input_stream_read(input_stream, buffer, file_appar_size, NULL, NULL);

	/* chiude lo stream per la lettura */
	g_input_stream_close(input_stream, NULL, NULL);

	g_io_stream_close((GIOStream*)file_appar_iostream, NULL, NULL);

	return bytes_letti;
}
   

/* apre in lettura e scrittura il file clienti.gec ne legge il contenuto e lo copia nel buffer passato come argomento e ritorna il numero di bytes letti */
 gssize open_rw_file_clienti_read(gchar* buffer)
{
	file_clienti = g_file_new_for_path(file_clienti_path);

	/* apre per leggere o scrivere il file clienti.gec */
	file_clienti_iostream = g_file_open_readwrite(file_clienti, NULL, NULL);

	/* stream per leggere file appar.gec */
	GInputStream *input_stream;
	input_stream = g_io_stream_get_input_stream((GIOStream*)file_clienti_iostream);

	/* legge il file clienti.gec e copia il contenuto in buffer */
	gssize bytes_letti;
	bytes_letti = g_input_stream_read(input_stream, buffer, file_clienti_size, NULL, NULL);

	/* chiude lo stream per la lettura */
	g_input_stream_close(input_stream, NULL, NULL);

	g_io_stream_close((GIOStream*)file_clienti_iostream, NULL, NULL);

	return bytes_letti;
}


/* mostra il file chiamate mese sul quale stiamo lavorando (percorso completo) */
void mostra_nome_file_ch_mese_caricato()
{
	gchar buffer[MAXPATHLEN * 3];
	sprintf(buffer, "Stai lavorando sul file:  %s", file_chiamate_mese_path);
	gtk_text_buffer_set_text(textbuffer_mese, buffer, strlen(buffer));

	return;
}


/* conta le attivita' del mese e le organizza giorno per giorno*/
void conta_attivita(attivita* p_attivita)
{
	int n_tot_chiamate;
	int i, n;
	int n_giorno;
	int ore;
	int catns;
	gunichar2 *p_flag;
	gunichar2 *p_ore;
	gunichar2 *ustr_giorno;
	gchar *str_giorno;
	gchar *str_ore;
	gunichar2 *p_catns;
	gchar *str_catns;
	gunichar2 *p_catns_cent;
	gchar *str_catns_cent;
	float catns_cent;

	/* ottiene dimensione file chiamate mese */
	file_chiamate_mese_size = get_file_ch_mese_size();

	/* apre in lettura e scrittura il file chiamate mese , ne legge il contenuto e lo copia in buffer.  */
	gchar buffer[file_chiamate_mese_size];
	open_rw_file_ch_mese_read(buffer);

	/* numero totale delle chiamate del mese */
	n_tot_chiamate = (file_chiamate_mese_size / LENGHT_CHIAMATA_BYTE) - 1;

	/* ciclo che esamina tutte le attivita (chiamate) del mese , ne rileva il giorno e lo usa come indice dell'array
	* puntato da p_attivita e poi esamina i flag di ogni chiamata per conteggiare giorno per giorno che tipo
	* di attivita sono state eseguite e salvare il tutto nell' array di strutture di tipo attivita del quale passiamo
	* il puntatore come argomento a questa funzione */
	for(i = 1; i <= n_tot_chiamate; i++)
	{
		/* ottiene stringa rappresentante il numero del giorno della chiamata in esame */
		ustr_giorno = (gunichar2*)(&buffer[i * LENGHT_CHIAMATA_BYTE + OFFSET_DATA]);
		str_giorno = g_utf16_to_utf8(ustr_giorno, 2, NULL, NULL, NULL);

		/* converte stringa del giorno in intero */
		n_giorno = str_to_int(str_giorno);

		/* punta i flags della chiamata in esame */
		p_flag =(gunichar2*)(&buffer[i * LENGHT_CHIAMATA_BYTE + OFFSET_FLAGS]);

		/* ciclo che esamina uno a uno i flag della chiamata in esame e incrementa il conto delle attivita
		* quando il flag corrispondente è a 1. Setta anche la zona di lavoro della giornata lavorativa con
		* con quella piu' lontana tra quelle delle varie attività della giornata*/
		for(n = 1; n <= NUM_FLAGS; n++)
		{
			if(p_flag[n - 1] == '1')
			{
				switch(n)
				{
					case 1: 	(p_attivita[n_giorno].ch_t)++; break;
					case 2: 	(p_attivita[n_giorno].ch_tl)++; break;
					case 3: 	(p_attivita[n_giorno].uvt)++; break;
					case 4: 	(p_attivita[n_giorno].ch_p)++; break;
					case 5: 	(p_attivita[n_giorno].dis_p)++; break;
					case 6: 	(p_attivita[n_giorno].uvp)++; break;
					case 7: 	if((p_attivita[n_giorno].zb == 0) &&  (p_attivita[n_giorno].zc == 0))
							{
								(p_attivita[n_giorno].za) = 1;
							} break;
					case 8: 	if(p_attivita[n_giorno].zc == 0)
							{
								(p_attivita[n_giorno].zb) = 1;
								(p_attivita[n_giorno].za) = 0;
							} break;
					case 9: 	(p_attivita[n_giorno].zc) = 1;
							(p_attivita[n_giorno].zb) = 0;
							(p_attivita[n_giorno].za) = 0;break;
				}
			}
		}

		/* punta le ore di lavoro della chiamata in esame */
		p_ore =(gunichar2*)(&buffer[i * LENGHT_CHIAMATA_BYTE + OFFSET_ORE]);

		/* converte la stringa delle ore di lavoro in int */
		if (*p_ore == 0)
		{
			ore = 0;
		}
		else
		{
			str_ore = g_utf16_to_utf8(p_ore, 1, NULL, NULL, NULL);
			ore = str_to_int(str_ore);
			g_free(str_ore);
		}

		p_attivita[n_giorno].lav_ore = p_attivita[n_giorno].lav_ore + ore;

		/* punta i compensi per attività a tariffa non standard */
		p_catns =(gunichar2*)(&buffer[i * LENGHT_CHIAMATA_BYTE + OFFSET_CATNS]);

		/* converte la stringa dei compensi per attività a tariffa non standard in int */
		if (*p_catns == 0)
		{
			catns = 0;
		}
		else
		{
			str_catns = g_utf16_to_utf8(p_catns, -1, NULL, NULL, NULL);
			catns = str_to_int(str_catns);
			g_free(str_catns);
		}

		p_attivita[n_giorno].catns = p_attivita[n_giorno].catns + catns;

		/* punta i centesimi dei compensi per attività a tariffa non standard */
		p_catns_cent =(gunichar2*)(&buffer[i * LENGHT_CHIAMATA_BYTE + OFFSET_CATNS_CENT]);

		/* converte la stringa dei centesimi dei compensi per attività a tariffa non standard in float */
		
		if (*p_catns_cent == 0)
		{
			catns_cent = 0;
		}
		else
		{
			str_catns_cent = g_utf16_to_utf8(p_catns_cent, -1, NULL, NULL, NULL);
			catns_cent = str_to_int(str_catns_cent);
			catns_cent = catns_cent / 100;
			g_free(str_catns_cent);
		}

		p_attivita[n_giorno].catns = p_attivita[n_giorno].catns + catns_cent;

		g_free(str_giorno);
	}

	return ;
}


/* ottiene intero rappresentante anno al quale si riferisce il file chiamate caricato */
int get_int_anno()
{
	int anno;
	int n_tot_char;
	gchar str_buffer[MAXPATHLEN];
	gchar str_anno[5];

	/* copia il percorso del file chiamate mese caricato in una stringa buffer sulla quale possiamo lavorare tranquilli*/
	strcpy(str_buffer, file_chiamate_mese_path);

	/* numero di caratteri che compongono il percorso del file chiamate mese caricato*/
	n_tot_char = strlen(str_buffer);

	/* ogni percorso di file chiamate finisce sempre con '.gec' preceduto dall' anno al quale si riferisce il file chiamate, quindi la strin
	 * ga dell'anno sara' sempre puntata da &str_buffer[n_tot_char - 8] finchè gli anni saranno di 4 cifre (e sara' ancora per un bel pezzo). */
	strcpy(str_anno, &str_buffer[n_tot_char - 8]);
	str_anno[4] = 0;
	anno = str_to_int(str_anno);

	return anno;
}


/* ottiene intero rappresentante mese al quale si riferisce il file chiamate caricato                              */
int get_int_mese()
{
	int i, x;
	int mese;
	gchar str_buffer[MAXPATHLEN];
	
	/* copia il percorso del file chiamate mese caricato in una stringa buffer sulla quale possiamo lavorare tranquilli*/
	strcpy(str_buffer, file_chiamate_mese_path);

	/* numero di caratteri che compongono il percorso del file chiamate mese caricato*/
	int n_tot_char = strlen(str_buffer);

	/* ottiene la posizione in str_buffer dell'ultimo carattere '/' dopo il quale inizia il nome del mese caricato */
	for(i = 0; i < n_tot_char; i++)
	{
		if(str_buffer[i] == '/') x=i;
	}

	/* quindi il nome del mese inizia da str_buffer[x + 1] e finisce a str_buffer[n_tot_char - 9] finchè gli anni saranno di 4 cifre. A questo punto 
	 * mettiamo a NULL il primo carattere dopo la fine del nome del mese che sarà str_buffer[n_tot_char - 8] cosi possiamo isolare il nome del mese */
	str_buffer[n_tot_char - 8] = 0;

	/* ottiene l'intero del mese */
	for(i = 1; i <=12; i++)
	{
		switch(i)
		{
			case 1 : 	if(!strcmp(&str_buffer[x + 1], "Gennaio")) mese = i; break;
			case 2 : 	if(!strcmp(&str_buffer[x + 1], "Febbraio")) mese = i; break;
			case 3 : 	if(!strcmp(&str_buffer[x + 1], "Marzo")) mese = i; break;
			case 4 : 	if(!strcmp(&str_buffer[x + 1], "Aprile")) mese = i; break;
			case 5 : 	if(!strcmp(&str_buffer[x + 1], "Maggio")) mese = i; break;
			case 6 : 	if(!strcmp(&str_buffer[x + 1], "Giugno")) mese = i; break;
			case 7 : 	if(!strcmp(&str_buffer[x + 1], "Luglio")) mese = i; break;
			case 8 : 	if(!strcmp(&str_buffer[x + 1], "Agosto")) mese = i; break;
			case 9 : 	if(!strcmp(&str_buffer[x + 1], "Settembre")) mese = i; break;
			case 10: 	if(!strcmp(&str_buffer[x + 1], "Ottobre")) mese = i; break;
			case 11: 	if(!strcmp(&str_buffer[x + 1], "Novembre")) mese = i; break;
			case 12: 	if(!strcmp(&str_buffer[x + 1], "Dicembre")) mese = i; break;
		}
	}

	return mese;
}


void ottieni_compensi_attivita(compenso *comp)
{
	GInputStream *input_stream;
	gchar buffer_compensi[file_compensi_size];
	
	/* apre in lettura e scrittura il file chiamate mese */
	file_chiamate_mese = g_file_new_for_path(file_chiamate_mese_path);
	file_chiamate_mese_iostream = g_file_open_readwrite(file_chiamate_mese, NULL, NULL);

	/* ottiene lo stream per leggere dal file chiamate mese */
	input_stream = g_io_stream_get_input_stream((GIOStream*) file_chiamate_mese_iostream);

	/* sposta la posizione dello stream al byte 192 da poter leggere nel file partendo da esso*/
	g_seekable_seek((GSeekable *)input_stream, OFFSET_COMPENSI, G_SEEK_CUR, NULL, NULL);

	/* legge i compensi dal file chaimate mese e li copia in buffer compensi */
	g_input_stream_read(input_stream, buffer_compensi, file_compensi_size, NULL, NULL);

	/* dal buffer compensi ottiene i compensi e l'iva convertendoli da char a float */
	comp->ch_t = ((buffer_compensi[0] - 48) * 10) + (buffer_compensi[1] - 48) + ((buffer_compensi[3] - 48) * 0.1);
	comp->ch_tl = ((buffer_compensi[5] - 48) * 10) + (buffer_compensi[6] - 48) + ((buffer_compensi[8] - 48) * 0.1);
	comp->uvt = ((buffer_compensi[10] - 48) * 10) + (buffer_compensi[11] - 48) + ((buffer_compensi[13] - 48) * 0.1);
	comp->ch_p = ((buffer_compensi[15] - 48) * 10) + (buffer_compensi[16] - 48) + ((buffer_compensi[18] - 48) * 0.1);
	comp->dis_p = ((buffer_compensi[20] - 48) * 10) + (buffer_compensi[21] - 48) + ((buffer_compensi[23] - 48) * 0.1);
	comp->uvp = ((buffer_compensi[25] - 48) * 10) + (buffer_compensi[26] - 48) + ((buffer_compensi[28] - 48) * 0.1);
	comp->zb = ((buffer_compensi[30] - 48) * 10) + (buffer_compensi[31] - 48) + ((buffer_compensi[33] - 48) * 0.1);
	comp->zc = ((buffer_compensi[35] - 48) * 10) + (buffer_compensi[36] - 48) + ((buffer_compensi[38] - 48) * 0.1);
	comp->lav_ore = ((buffer_compensi[40] - 48) * 10) + (buffer_compensi[41] - 48) + ((buffer_compensi[43] - 48) * 0.1);
	comp->iva = ((buffer_compensi[45] - 48) * 0.1) + ((buffer_compensi[46] - 48) * 0.01);

	g_input_stream_close(input_stream, NULL, NULL);
	g_io_stream_close((GIOStream*)file_chiamate_mese_iostream, NULL, NULL);

	return ;
}


/****
 ***
 ***		SIGNAL HANDLERS
 ***
 ****/

/* Fa in modo che in un entry possano essere inseriti solo caratteri numerici. Il segnale "insert-text" viene
 * emesso da un entry nel momento in cui l'utilizzatore scrive testo in esso. Questa funzione è il gestore del
 * segnale "insert-text" che abbiamo collegato ad un entry nel quale vogliamo che si possano inserire solo 
 * numeri. I parametri della funzione sono :
 * widget -> puntatore all'entry che ha emesso il segnale (insomma l'entry nel quale stiamo scrivendo)
 * text -> puntatore al nuovo testo o carattere che abbiamo digitato
 * lenght -> lunghezza del nuovo testo
 * position -> posizione nel buffer dove viene inserito il nuovo testo o carattere
 * data -> data che possiamo inserire noi quando connettiamo questa funzione signal handler. 
 * Tutti i parametri vengono inizializzati coi valori corretti al momento dell'emissione del segnale
 * "insert-text" e noi possiamo usarli nella funzione per i nostri scopi. */
void inserisci_solo_numeri(GtkEditable *widget, gchar *text, gchar *length, gint *position, gpointer data)
{
	gchar carattere_immesso;

	/* se il carattere che abbiamo immesso è una cifra (0-9) , esce dalla funzione e prosegue con la gestione di
	* default del segnale "insert-text" e quindi visualizza il carattere nell'entry. */
	carattere_immesso = *text;
	if((carattere_immesso == '0') || (carattere_immesso == '1') || (carattere_immesso == '2') ||
		(carattere_immesso == '3') || (carattere_immesso == '4') || (carattere_immesso == '5') ||
		(carattere_immesso == '6') || (carattere_immesso == '7') || (carattere_immesso == '8') ||
		(carattere_immesso == '9') ) return;

	/* se il carattere che abbiamo immesso non è una cifra, stoppa il segnale "insert-text" e quindi non prosegue
	* neanche con la gestione di default del segnale per cui non visualizza il carttere nell'entry. */
	g_signal_stop_emission_by_name(widget, "insert_text");

	return;
}


/* se viene premuto il pulsante di chiusura sulla titlebar , quando questa funzione ritorna FALSE , il delete-event diventa un destroy siganl
 * però se viene premuto il pulsante di chiusura sulla titlebar della window_modifica_chiamata, non vogliamo che venga distrutta ma solo nascosta,
 * quindi facciamo in modo che la funzione ritorni TRUE ,così il delete-event non diventa un destroy signal */
gint delete_event(GtkWidget *widget, GdkEvent event, gpointer data)
{
	/* se vogliamo uscire senza ancora aver effettuato il login, non ci chiede di aggiornare il database locale */
	if((schermata_corrente == SCHERMATA_ACCESSO) || (schermata_corrente == SCHERMATA_RIPRISTINO))
	{
		esci_subito = TRUE;
	}

	/* se il delete-event è arrivato dalla window_modifica_chiamata ,la finestra non viene distrutta ma solo nascosta. Così facendo non dobbiamo ricarcarla
	 * dal builder con tutti i suoi widget ogni volta che premiamo il button_modifica_chiamata */
	if(widget == ((GtkWidget*)window_modifica_chiamata))
	{
		gtk_widget_hide((GtkWidget*)window_modifica_chiamata);
		
		return TRUE;
	}

	/* quando questa funzione ritorna FALSE , il delete-event diventa un destroy signal */
	return FALSE;
}


/* se viene premuto 'enter' nell'entry della password manda un click al pulsante 'button_accedi' */
static gboolean enter_key_pressed(GtkWidget *widget, GdkEvent *event, gpointer  callback_data)
{
	/* ottiene codice del tasto premuto */
	guint key;
	key = event->key.hardware_keycode;

	/* se il tasto premuto è 'enter' */
	if(key == 36) g_signal_emit_by_name(button_accedi, "clicked");

	return FALSE;
}


/* se viene premuto il 'button_accedi' controlla se è stato montato il database dal server , controlla la password e se è corretta entra nel menu principale, 
 * altrmenti segnala errore */
static gboolean pressed_button_accedi(GtkWidget *widget, gpointer callback_data)
{
	FILE *stream_file_config;
	time_t rawtime;
	struct tm *timeinfo;
	char pw_accesso[MAX_CHAR_PW];
	int i;
	gint offset;
	const gchar *pw_immessa;
	GInputStream *input_stream;
	GOutputStream *output_stream;

	/* apre il file di configurazione 'system.gec' in lettura e ne ottiene il puntatore allo stream */
	stream_file_config = fopen(file_config_path, "r");

	/* se non riesce ad aprire il file di configurazione segnala l'errore con l' apposito dialog */
	if(stream_file_config == 0)
	{
		crea_dialog_box_un_pulsante(dialog_fceom, "ATTENZIONE!", "\n   File di configurazione errato o mancante    \n", window_principale, "Chiudi");
		return FALSE;
	}

	/* legge password dall'apposito file */
	/* sposta il puntatore all'offset partendo dall'inizio dello stream (il terzo parametro a SEEK_SET dice di calcolare
	* l'offset a partire dall'inizio dello stream, altrimenti si può usare SEEK_CUR che calcola dalla posizione 
	* corrente nello stream o SEEK_END che calcola dalla fine dello stream) perchè la pw è scritta li. 
	* Come offset ho scelto un valore a caso. */
	offset = 326;
	fseek(stream_file_config, offset, SEEK_SET);
	for(i = 0; i < MAX_CHAR_PW; i++)
	{
		/*legge un carattere e lo copia nell'array e sposta il puntatore al byte successivo */
		pw_accesso[i] = getc(stream_file_config);

		if (pw_accesso[i] == 0) break;          // se è carattere nullo termina ciclo

		/*sposta il puntatore al carattere successivo perchè il file 'system.gec è in wide_char e ogni carattere è di 
		* 2 byte di cui il secondo nel nostro caso è sempre 0 visto che non abbiamo caratteri particolari */
		getc(stream_file_config);
	}
	pw_accesso[i+1] = 0; // mette lo zero per terminare la stringa

	/* salva il contenuto della password di accesso nella GString globale pw_corrente che ci servirà nella pressed_button_ok */
	pw_corrente = g_string_new(NULL);
	g_string_assign(pw_corrente, pw_accesso);

	/* ottiene password immessa dall'utente */
	pw_immessa = gtk_entry_get_text(entry_password);

	/* se pw_accesso e pw_immessa non sono uguali */
	if (strcmp(pw_accesso, pw_immessa) != 0)
	{
		/* e se pw_immessa è  diversa dalla password amministratore */
		if (strcmp(pw_immessa, "cqfpadmin"))
		{
			crea_dialog_box_un_pulsante(dialog_pwerr, "ATTENZIONE !",  "\n       Password ERRATA !        \n", window_principale, "   Chiudi  ");
			return FALSE;
		}
	}

	/* se non è stato montato il database sul server segnala il problema e chiede se si vuole proseguire con una copia del database locale */
	if(!(database_server_montato))
	{
		crea_dialog_box_due_pulsanti(dialog_db_server_no, "ATTENZIONE !", 
					"\n Non è stato possibile montare il database sul server.\n Vuoi proseguire con una copia del database locale ? \n", 
					window_principale, "Si", "No", copia_db_locale, uscita_forzata);
	}

	/* ottiene data corrente e mese e giorno della settimana come stringhe */
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	str_giorno_settimana_lett = num_to_giorno(timeinfo->tm_wday);
	str_mese_lett = num_to_mese(timeinfo->tm_mon);
	giorno = timeinfo->tm_mday;
	mese = timeinfo->tm_mon + 1;		// tm_mon va da 0 a 11
	anno = timeinfo->tm_year + 1900;	// lo zero degli anni è il 1900

	/* formatta la data corrente e la visualizza nell'apposito textview */
	gchar data[32];
	sprintf(data, "DATA: %s %i / %i /%i", str_giorno_settimana_lett, giorno, mese, anno);
	gtk_text_buffer_set_text(textbuffer_data, data, strlen(data));

	/* formatta percorso file chiamate */
	sprintf(file_chiamate_mese_path, "%s/database/%s%i.gec", program_dir, str_mese_lett, anno);

	mostra_nome_file_ch_mese_caricato();

	/* gfile relativo al file chiamate */
	file_chiamate_mese = g_file_new_for_path(file_chiamate_mese_path);

	/* prova a creare il file chiamate in lettura/scrittura dal gfile */
	file_chiamate_mese_iostream = g_file_create_readwrite(file_chiamate_mese, G_FILE_CREATE_NONE, NULL, NULL);

	/* se la g_file_create_readwrite ritorna  valore diverso da zero è andata a buon fine quindi inseriamo l' intestazione all'inizio del file */
	if(file_chiamate_mese_iostream != 0)
	{
		gchar buffer[LENGHT_CHIAMATA_BYTE] = {0};
		sprintf(buffer, "***** File di salvataggio chiamate generato da GECHI *****\rData Creazione :\
				%d\\%d\\%d\r NB: non modificare il file per non perdere tutti i dati!", giorno, mese, anno);

		GOutputStream *output_stream;
		output_stream = g_io_stream_get_output_stream((GIOStream*)file_chiamate_mese_iostream);
		g_output_stream_write(output_stream, buffer, LENGHT_CHIAMATA_BYTE, NULL, NULL);
		g_output_stream_close(output_stream, NULL, NULL);
	}

	/* se la g_file_create_readwrite ritorna 0, o c'è stato un errore strano nella creazione del file, o il file esiste  già, per cui adesso proviamo 
	 * ad aprirlo */
	if(file_chiamate_mese_iostream == 0)
	{
		/* prova ad aprire il file chiamate in lettura/scrittura */
		file_chiamate_mese_iostream = g_file_open_readwrite(file_chiamate_mese, NULL, NULL);

		/* se non riesce neanche ad aprirlo segnala l'errore */
		if(file_chiamate_mese_iostream == 0)
		{
			crea_dialog_box_un_pulsante(dialog_icoafc, "ATTENZIONE!", "\n   Impossibile creare o aprire il file chiamate    \n",
							window_principale, "Chiudi");
			return FALSE;
		}
	}

	/* formatta percorso file compensi */
	sprintf(file_compensi_path, "%s/database/compensi.gec", program_dir);

	/* gfile relativo al file compensi */
	file_compensi = g_file_new_for_path(file_compensi_path);

	/* prova ad aprire il file compensi in lettura/scrittura */
	file_compensi_iostream = g_file_open_readwrite(file_compensi, NULL, NULL);

	/* se non riesce  ad aprirlo segnala l'errore */
	if(file_compensi_iostream == 0)
	{
		crea_dialog_box_un_pulsante(dialog_iafc, "ATTENZIONE!", "\n   Impossibile aprire il file compensi    \n", window_principale, "Chiudi");
		return FALSE;
	}

	/* ottiene dimensione del file compensi e crea un buffer per copiarne il contenuto */
	file_compensi_size = get_file_compensi_size();
	gchar buffer_compensi[file_compensi_size];

	/* copia il contenuto del file compensi nel buffer */
	input_stream = g_io_stream_get_input_stream((GIOStream*)file_compensi_iostream);
	g_input_stream_read(input_stream, buffer_compensi, file_compensi_size, NULL, NULL);

	/* ottiene nuovamente lo stream per scrivere nel file chiamate mese */
	output_stream = g_io_stream_get_output_stream((GIOStream*)file_chiamate_mese_iostream);

	/* sposta la posizione dello stream al byte 192 da poter scrivere nel file partendo da esso*/
	g_seekable_seek((GSeekable *)output_stream, OFFSET_COMPENSI, G_SEEK_CUR, NULL, NULL);

	/* copia il contenuto del file compensi nel file chiamate mese a partire dal byte 192, c0 in esadecimale */
	g_output_stream_write(output_stream, buffer_compensi, file_compensi_size, NULL, NULL);

	g_output_stream_close(output_stream, NULL, NULL);

	g_io_stream_close((GIOStream*)file_chiamate_mese_iostream, NULL, NULL);
	g_io_stream_close((GIOStream*)file_compensi_iostream, NULL, NULL);

	fclose(stream_file_config);

	gtk_window_maximize(window_principale);

	cambia_schermata(box_accedi, box_menu_princ);

	return FALSE;
}


/* se viene premuto il 'button_resoc_mese' visualizza la tabella col numero di attivita' suddivise giorno per giorno*/
static gboolean pressed_button_resoc_mese(GtkWidget *widget, gpointer callback_data)
{
	attivita    att[32] = {0};
	compenso comp;
	gchar tabella_path[MAXPATHLEN];
	cairo_surface_t *immagine_tabella;
	cairo_t *cr;
	gchar titolo_resoconto[MAXPATHLEN];
	float tot_giorno = 0;
	int tot_ch_t_mese = 0;
	int tot_ch_tl_mese = 0;
	int tot_uvt_mese = 0;
	int tot_ch_p_mese = 0;
	int tot_dis_p_mese = 0;
	int tot_uvp_mese = 0;
	int tot_zb_mese = 0;
	int tot_zc_mese = 0;
	int tot_lav_ore_mese = 0;
	float tot_mese = 0;
	float tot_catns_mese = 0;
	gchar str_numero[10];

	/* conta le attivita svolte nel mese e le salva i totali nell'array di strutture att. Il valore dell'indice dell'array corrisponde al giorno del mese*/
	conta_attivita(&att[0]);

	/* ottiene i compensi per le attività standard e l'iva*/
	ottieni_compensi_attivita(&comp);

	/* carica in una surface cairo l'immagine della tabella di resoconto mese */
	g_sprintf(tabella_path, "%s/tabella3.png", gui_dir);
	immagine_tabella = cairo_image_surface_create_from_png(tabella_path);

	/* crea il cairo context (insieme di colori, pennelli, ecc... per disegnare)  per la surface appena generata */
	cr = cairo_create(immagine_tabella);

	/* setta il tipo di carattere con cui completare la tabella */
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 11);  

	g_sprintf(titolo_resoconto, "Resoconto mese %s", file_chiamate_mese_path);
	cairo_move_to(cr, 5, 20);
	cairo_show_text(cr, titolo_resoconto); 

	int i;      /* ciclo che scrive i valori numerici delle attivita nella tabella*/
	for(i = 1; i <= 31; i++)
	{
		/* scrive il numero di chiamate terminale giorno per giorno se è diverso da zero */
		if(att[i].ch_t != 0)
		{
			/* coneverte numero chiamate terminale del giorno in stringa */
			g_sprintf(str_numero, "%i", att[i].ch_t);

			/* visualizza il dato nella tabella*/
			cairo_move_to(cr, (LARG_CELLA + X_OFFSET_TESTO), ((i - 1) * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
			cairo_show_text(cr, str_numero); 
		}

		/* identico all' if qui sopra, cambia solo il tipo di attivita da visualizzare*/
		if(att[i].ch_tl != 0)
		{
			g_sprintf(str_numero, "%i", att[i].ch_tl);
			cairo_move_to(cr, (LARG_CELLA * 2 + X_OFFSET_TESTO), ((i - 1) * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
			cairo_show_text(cr, str_numero); 
		}

		if(att[i].ch_p != 0)
		{
			g_sprintf(str_numero, "%i", att[i].ch_p);
			cairo_move_to(cr, (LARG_CELLA * 3 + X_OFFSET_TESTO), ((i - 1) * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
			cairo_show_text(cr, str_numero); 
		}

		if(att[i].dis_p != 0)
		{
			g_sprintf(str_numero, "%i", att[i].dis_p);
			cairo_move_to(cr, (LARG_CELLA * 4 + X_OFFSET_TESTO), ((i - 1) * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
			cairo_show_text(cr, str_numero);
		}

		if(att[i].uvt != 0)
		{
			g_sprintf(str_numero, "%i", att[i].uvt);
			cairo_move_to(cr, (LARG_CELLA * 5 + X_OFFSET_TESTO), ((i - 1) * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
			cairo_show_text(cr, str_numero);
		}

		if(att[i].uvp != 0)
		{
			g_sprintf(str_numero, "%i", att[i].uvp);
			cairo_move_to(cr, (LARG_CELLA * 6 + X_OFFSET_TESTO), ((i - 1) * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
			cairo_show_text(cr, str_numero);
		}

		if(att[i].zb != 0)
		{
			g_sprintf(str_numero, "%i", att[i].zb);
			cairo_move_to(cr, (LARG_CELLA * 7 + X_OFFSET_TESTO), ((i - 1) * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
			cairo_show_text(cr, str_numero);
		}

		if(att[i].zc != 0)
		{
			g_sprintf(str_numero, "%i", att[i].zc);
			cairo_move_to(cr, (LARG_CELLA * 8 + X_OFFSET_TESTO), ((i - 1) * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
			cairo_show_text(cr, str_numero);
		}

		if(att[i].lav_ore != 0)
		{
			g_sprintf(str_numero, "%i", att[i].lav_ore);
			cairo_move_to(cr, (LARG_CELLA * 9 + X_OFFSET_TESTO), ((i - 1) * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
			cairo_show_text(cr, str_numero);
		}

		if(att[i].catns != 0)
		{
			/* coneverte numero chiamate terminale del giorno in stringa */
			g_sprintf(str_numero, "%.2f", att[i].catns);

			/* visualizza il dato nella tabella*/
			cairo_move_to(cr, (LARG_CELLA * 10 + X_OFFSET_TESTO), ((i - 1) * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
			cairo_show_text(cr, str_numero); 
		}

		tot_giorno = (att[i].ch_t * comp.ch_t) + (att[i].ch_tl * comp.ch_tl) + (att[i].uvt * comp.uvt) + (att[i].ch_p * comp.ch_p) +
				(att[i].dis_p * comp.dis_p) + (att[i].uvp * comp.uvp) + (att[i].lav_ore * comp.lav_ore) + (att[i].zb * comp.zb) +
				(att[i].zc * comp.zc) + (att[i].catns);
								
		if(tot_giorno != 0)
		{
			g_sprintf(str_numero, "%.2f", tot_giorno);
			cairo_move_to(cr, (LARG_CELLA * 10 + X_OFFSET_TESTO + LARG_CELLA_CATNS), ((i - 1) * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
			cairo_show_text(cr, str_numero);
		}

		/* numeri  totali delle attivita del mese */
		tot_ch_t_mese = tot_ch_t_mese + att[i].ch_t;
		tot_ch_tl_mese = tot_ch_tl_mese + att[i].ch_tl;
		tot_uvt_mese = tot_uvt_mese + att[i].uvt;
		tot_ch_p_mese = tot_ch_p_mese + att[i].ch_p;
		tot_dis_p_mese = tot_dis_p_mese + att[i].dis_p;
		tot_uvp_mese = tot_uvp_mese + att[i].uvp;
		tot_zb_mese = tot_zb_mese + att[i].zb;
		tot_zc_mese = tot_zc_mese + att[i].zc;
		tot_lav_ore_mese = tot_lav_ore_mese + att[i].lav_ore;
		tot_mese = tot_mese + tot_giorno;
		tot_catns_mese = tot_catns_mese + att[i].catns;
	}

	/* disegna i  numeri totali del mese */
	g_sprintf(str_numero, "%i", tot_ch_t_mese);
	cairo_move_to(cr, (LARG_CELLA  + X_OFFSET_TESTO), (31 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%i", tot_ch_tl_mese);
	cairo_move_to(cr, (LARG_CELLA * 2 + X_OFFSET_TESTO), (31 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%i", tot_ch_p_mese);
	cairo_move_to(cr, (LARG_CELLA * 3 + X_OFFSET_TESTO), (31 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%i", tot_dis_p_mese);
	cairo_move_to(cr, (LARG_CELLA * 4 + X_OFFSET_TESTO), (31 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%i", tot_uvt_mese);
	cairo_move_to(cr, (LARG_CELLA * 5 + X_OFFSET_TESTO), (31 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%i", tot_uvp_mese);
	cairo_move_to(cr, (LARG_CELLA * 6 + X_OFFSET_TESTO), (31 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%i", tot_zb_mese);
	cairo_move_to(cr, (LARG_CELLA * 7 + X_OFFSET_TESTO), (31 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%i", tot_zc_mese);
	cairo_move_to(cr, (LARG_CELLA * 8 + X_OFFSET_TESTO), (31 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%i", tot_lav_ore_mese);
	cairo_move_to(cr, (LARG_CELLA * 9 + X_OFFSET_TESTO), (31 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);


	/* visualizza totali in soldi delle attivita del mese */
	g_sprintf(str_numero, "%.2f", tot_ch_t_mese * comp.ch_t);
	cairo_move_to(cr, (LARG_CELLA  + X_OFFSET_TESTO), (32 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%.2f", tot_ch_tl_mese * comp.ch_tl);
	cairo_move_to(cr, (LARG_CELLA * 2 + X_OFFSET_TESTO), (32 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%.2f", tot_ch_p_mese * comp.ch_p);
	cairo_move_to(cr, (LARG_CELLA * 3 + X_OFFSET_TESTO), (32 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%.2f", tot_dis_p_mese * comp.dis_p);
	cairo_move_to(cr, (LARG_CELLA * 4 + X_OFFSET_TESTO), (32 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%.2f", tot_uvt_mese * comp.uvt);
	cairo_move_to(cr, (LARG_CELLA * 5 + X_OFFSET_TESTO), (32 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%.2f", tot_uvp_mese * comp.uvp);
	cairo_move_to(cr, (LARG_CELLA * 6 + X_OFFSET_TESTO), (32 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%.2f", tot_zb_mese * comp.zb);
	cairo_move_to(cr, (LARG_CELLA * 7 + X_OFFSET_TESTO), (32 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%.2f", tot_zc_mese * comp.zc);
	cairo_move_to(cr, (LARG_CELLA * 8 + X_OFFSET_TESTO), (32 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%.2f", tot_lav_ore_mese * comp.lav_ore);
	cairo_move_to(cr, (LARG_CELLA * 9 + X_OFFSET_TESTO), (32 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%.2f", tot_catns_mese);
	cairo_move_to(cr, (LARG_CELLA * 10 + X_OFFSET_TESTO), (32 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);

	/* disegna totale mese iva esclusa*/
	g_sprintf(str_numero, "%.2f", tot_mese);
	cairo_move_to(cr, (LARG_CELLA * 9 + X_OFFSET_TESTO), (34 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);

	/* disegna i compensi per ogni tipo di attività in cima ad ogni colonna della tabella */
	g_sprintf(str_numero, "%.2f", comp.ch_t);
	cairo_move_to(cr, (LARG_CELLA  + X_OFFSET_TESTO), (Y_INIZIO - ALT_CELLA));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%.2f", comp.ch_tl);
	cairo_move_to(cr, (LARG_CELLA * 2 + X_OFFSET_TESTO), (Y_INIZIO - ALT_CELLA));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%.2f", comp.ch_p);
	cairo_move_to(cr, (LARG_CELLA * 3 + X_OFFSET_TESTO), (Y_INIZIO - ALT_CELLA));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%.2f", comp.dis_p);
	cairo_move_to(cr, (LARG_CELLA * 4 + X_OFFSET_TESTO), (Y_INIZIO - ALT_CELLA));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%.2f", comp.uvt);
	cairo_move_to(cr, (LARG_CELLA * 5 + X_OFFSET_TESTO), (Y_INIZIO - ALT_CELLA));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%.2f", comp.uvp);
	cairo_move_to(cr, (LARG_CELLA * 6 + X_OFFSET_TESTO), (Y_INIZIO - ALT_CELLA));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%.2f", comp.zb);
	cairo_move_to(cr, (LARG_CELLA * 7 + X_OFFSET_TESTO), (Y_INIZIO - ALT_CELLA));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%.2f", comp.zc);
	cairo_move_to(cr, (LARG_CELLA * 8 + X_OFFSET_TESTO), (Y_INIZIO - ALT_CELLA));
	cairo_show_text(cr, str_numero);

	g_sprintf(str_numero, "%.2f", comp.lav_ore);
	cairo_move_to(cr, (LARG_CELLA * 9 + X_OFFSET_TESTO), (Y_INIZIO - ALT_CELLA));
	cairo_show_text(cr, str_numero);

	/* ottiene interi rappresentanti il mese a l'anno del file chiamate mese caricato*/
	anno_caricato = get_int_anno();
	mese_caricato = get_int_mese();

	/* disegna valore IVA del mese */
	g_sprintf(str_numero, "%.0f%%",  (comp.iva * 100));
	cairo_move_to(cr, (LARG_CELLA * 4 + X_OFFSET_TESTO + 10), (34 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);

	/* disegna IVA del mese */
	g_sprintf(str_numero, "%.2f", tot_mese * comp.iva);
	cairo_move_to(cr, (LARG_CELLA * 5 + X_OFFSET_TESTO), (34 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);

	/* disegna totale fattura del mese */
	float tot_fattura = tot_mese + tot_mese * comp.iva;
	g_sprintf(str_numero, "%.2f", tot_fattura);
	cairo_move_to(cr, (LARG_CELLA * 2 + X_OFFSET_TESTO), (34 * ALT_CELLA + Y_OFFSET_TESTO + Y_INIZIO));
	cairo_show_text(cr, str_numero);

	/* ottiene il pixbuf corrispondente al 'surface cairo' che contiene l'immagine */
	GdkPixbuf *pixbuf;
	pixbuf = gdk_pixbuf_get_from_surface(immagine_tabella, 0, 0, 1000, 660);

	/* setta il pixbuf nell' image_widget , CIOÈ VISUALIZZA L'IMMAGINE .PNG NELL' IMAGE_WIDGET, che è quello che vogliamo */
	gtk_image_set_from_pixbuf(GTK_IMAGE(image_resoconto_mese), pixbuf); 

	cambia_schermata(box_menu_princ, box_resoconto_mese);

	return FALSE;
}


/*calcola gli imponibili totali , l'iva e il totale mese per mese e li copia nell'array che passiamo come argomento*/
void calcola_totali_mesi(float *imp_mese, float *iva_mese, float *tot_mese, const gchar *string_anno_scelto)
{
	int mese_corrente;
	int anno_scelto;
	int m;

	/* salva il percorso del file chiamate mese corrente per poi ripristinarlo a fine funzione */
	gchar copia_file_chiamate_mese_path[MAXPATHLEN];
	strcpy(copia_file_chiamate_mese_path, file_chiamate_mese_path);

	gchar str_mese[13][20] = {"", "Gennaio", "Febbraio", "Marzo", "Aprile", "Maggio", "Giugno", "Luglio",
							"Agosto", "Settembre", "Ottobre", "Novembre", "Dicembre"};

	anno_scelto = str_to_int((gchar*)string_anno_scelto);
	mese_corrente = mese;


	for(m = 1; m <= 12; m++)
	{
		if(anno_scelto == anno)
		{
			/* solo se l'anno scelto è uguale all'anno corrente*/
			if(mese_corrente == m) 
			{
				/* ripristina la stringa globale rappresentante il percorso del file chiamate mese sul quale stavamo lavorando */
				strcpy(file_chiamate_mese_path, copia_file_chiamate_mese_path);
				return;
			}
		}

		/* formatta percorso file chiamate */
		sprintf(file_chiamate_mese_path, "%s/database/%s%i.gec", program_dir, &str_mese[m] [0], anno_scelto);

		/* conta le attivita svolte nel mese e le salva i totali nell'array di strutture att. Il valore del primo
		* indice dell'array corrisponde al mese, il valore del secondo indice dell'array corrisponde al giorno del mese*/
		attivita    att[13][32] = {0};
		conta_attivita(&att[m][0]);

		compenso comp;
		ottieni_compensi_attivita(&comp);

		/* calcola l' imponibile totale del mese in esame*/
		int imp_tot_giorno = 0;
		int imp_tot_mese[13] = {0};
		int i;
		for(i = 1; i <= 31; i++)
		{
			imp_tot_giorno = (att[m][i].ch_t * comp.ch_t) + (att[m][i].ch_tl * comp.ch_tl) +
							(att[m][i].uvt * comp.uvt) + (att[m][i].ch_p * comp.ch_p) +
							(att[m][i].dis_p * comp.dis_p) + (att[m][i].uvp * comp.uvp) +
							(att[m][i].lav_ore * comp.lav_ore) + (att[m][i].zb * comp.zb) +
							(att[m][i].zc * comp.zc + att[m][i].catns);

			imp_tot_mese[m] = imp_tot_mese[m] + imp_tot_giorno;
		}

		*imp_mese = imp_tot_mese[m];
        
		*iva_mese = imp_tot_mese[m] * comp.iva;
        
		*tot_mese = imp_tot_mese[m] + (imp_tot_mese[m] * comp.iva);

		imp_mese ++;
		iva_mese ++;
		tot_mese ++;
	}

	/*ripristina la stringa globale rappresentante il percorso del file chiamate mese sul quale stavamo lavorando*/
	strcpy(file_chiamate_mese_path, copia_file_chiamate_mese_path);

	return;
}

/* calcola le attivita totali eseguite mese per mese (cioè quante chiamate terminali, chiamate pos, lavori a ore, 
 * zone b, ecc...) nell'anno scelto che passiamo come argomento alla funzione. L'altro argomento della funzione 
 * deve essere un'array di almeno 13 strutture attivita che verrà riempito coi valori ottenuti */
void calcola_attivita_tot_mesi(attivita *tot_attivita_mese, const gchar *string_anno_scelto)
{
	gchar str_mese[13][20] = {"", "Gennaio", "Febbraio", "Marzo", "Aprile", "Maggio", "Giugno", "Luglio",
							"Agosto", "Settembre", "Ottobre", "Novembre", "Dicembre"};
	gchar copia_file_chiamate_mese_path[MAXPATHLEN];
	attivita    att[13][32] = {0};
	int anno_scelto = str_to_int((gchar*)string_anno_scelto);
	int mese_corrente = mese;
	int m, i;
	
	/* salva il percorso del file chiamate mese corrente per poi ripristinarlo a fine funzione */
	strcpy(copia_file_chiamate_mese_path, file_chiamate_mese_path);
	
	for(m = 1; m <= 12; m++)
	{
		if(anno_scelto == anno)
		{
			/* solo se l'anno scelto è uguale all'anno corrente*/
			if(mese_corrente == m) 
			{
				/* ripristina la stringa globale rappresentante il percorso del file chiamate mese sul quale stavamo lavorando */
				strcpy(file_chiamate_mese_path, copia_file_chiamate_mese_path);
				return;
			}
		}

	/* formatta percorso file chiamate */
	sprintf(file_chiamate_mese_path, "%s/database/%s%i.gec", program_dir, &str_mese[m] [0], anno_scelto);

	/* conta le attivita svolte nel mese m e le salva i totali nell'array di strutture tot_attivita_mese[m]. 
	 * Il valore del primo indice dell'array corrisponde al mese, il valore del secondo indice dell'array 
	 * corrisponde al giorno del mese*/
	conta_attivita(&att[m][0]);

	tot_attivita_mese[m].ch_t = 0;
	tot_attivita_mese[m].ch_tl = 0;
	tot_attivita_mese[m].uvt = 0;
	tot_attivita_mese[m].ch_p= 0;
	tot_attivita_mese[m].dis_p = 0;
	tot_attivita_mese[m].uvp = 0;
	tot_attivita_mese[m].za = 0;
	tot_attivita_mese[m].zb = 0;
	tot_attivita_mese[m].zc = 0;
	tot_attivita_mese[m].lav_ore = 0;

	for(i = 1; i <= 31; i++) /* ottiene il tot di ogni tipo di attivita sommando quelle rilevate giorno per giorno */
	{
		/* totale delle chiamate terminali del mese m, toale delle chiamate terminali lunghe del mese m, ecc...*/
		tot_attivita_mese[m].ch_t = tot_attivita_mese[m].ch_t + att[m][i].ch_t;
		tot_attivita_mese[m].ch_tl = tot_attivita_mese[m].ch_tl + att[m][i].ch_tl;
		tot_attivita_mese[m].uvt = tot_attivita_mese[m].uvt + att[m][i].uvt;
		tot_attivita_mese[m].ch_p = tot_attivita_mese[m].ch_p + att[m][i].ch_p;
		tot_attivita_mese[m].dis_p = tot_attivita_mese[m].dis_p + att[m][i].dis_p;
		tot_attivita_mese[m].uvp = tot_attivita_mese[m].uvp + att[m][i].uvp;
		tot_attivita_mese[m].za = tot_attivita_mese[m].za + att[m][i].za;
		tot_attivita_mese[m].zb = tot_attivita_mese[m].zb + att[m][i].zb;
		tot_attivita_mese[m].zc = tot_attivita_mese[m].zc + att[m][i].zc;
		tot_attivita_mese[m].lav_ore = tot_attivita_mese[m].lav_ore + att[m][i].lav_ore;                             
	}
}

	/* ripristina la stringa globale rappresentante il percorso del file chiamate mese sul quale stavamo lavorando*/
	strcpy(file_chiamate_mese_path, copia_file_chiamate_mese_path);

	return;
}


/* se viene premuto il 'button_stat_annuali' */
static gboolean pressed_button_stat_annuali(GtkWidget *widget, gpointer callback_data)
{
	unsigned int ui;

	for(ui = anno; ui >= 2008; ui--)
	{
		GString *str_anno;
		str_anno = positive_int_to_string(ui);
		gtk_combo_box_text_append_text(combo_box_text_anno, str_anno->str);

		/* visualizza di default l'anno corrente nell'entry associato al comboboxanno */
		if(ui == anno) gtk_entry_set_text(entry_anno_scelto, str_anno->str);
	}

	g_signal_emit_by_name(button_aggiorna, "clicked", NULL);

	cambia_schermata(box_menu_princ, box_statistiche_annuali);

	return FALSE;
}


/* disegna grafico statistiche annuali. Riceve come parametri l'intero e la stringa dell'anno scelto di cui vogliamo visualizzare il grafico*/
void disegna_grafico_stat_annuali(int a, const gchar* str_a)
{  
	int m, i, totale_imponibile_annuo;
	int totale_ch_t_anno = 0;
	int totale_ch_tl_anno = 0;
	int totale_uvt_anno = 0;
	int totale_ch_p_anno = 0;
	int totale_dis_p_anno = 0;
	int totale_uvp_anno = 0;
	int totale_za_anno = 0;
	int totale_zb_anno = 0;
	int totale_zc_anno = 0;
	int totale_lav_ore_anno = 0; 
	float imp_mese[13] = {0};
	float iva_mese[13] = {0};
	float tot_mese[13] = {0};
	float totale_fatturato_annuo, totale_iva_annua;
	gchar grafico4_path[MAXPATHLEN];
	gchar valore[10] = {0};
	cairo_surface_t *immagine_grafico;
	cairo_t *cr;
	attivita totali_attivita_mesi[13];
	GdkPixbuf *pixbuf;

	calcola_totali_mesi(&imp_mese[1], &iva_mese[1], &tot_mese[1], str_a);

	/* carica in una surface cairo l'immagine del grafico statistiche annuali */
	g_sprintf(grafico4_path, "%s/grafico4.png", gui_dir);
	immagine_grafico = cairo_image_surface_create_from_png(grafico4_path);

	/* crea il cairo context (insieme di colori, pennelli, ecc... per disegnare)  per la surface appena generata*/
	cr = cairo_create(immagine_grafico);

	/* se l'anno scelto è uguale all'anno corrente m sarà uguale al mese corrente perchè non possiamo ottenere dati di mesi del futuro */
	if(a == anno)
	{
		m = mese;
	}
	else
	{
		m = 13;
	}

	/* regola spessore pennello e lo sposta alle coordinate iniziali del grafico del fatturato */
	cairo_set_line_width(cr, 1.5);
	cairo_move_to(cr, X0_GRAFICO_FATTURATO, Y0_GRAFICO_FATTURATO);

	/* imponibile */
	totale_imponibile_annuo = 0;   /* totale imponibile dell'anno fino al mese corrente */
	for(i = 1; i <= (m - 1); i ++)
	{
		cairo_set_source_rgb (cr, 0.957, 0, 0);
		cairo_line_to(cr, X0_GRAFICO_FATTURATO + DELTA_MESE_X * i, Y0_GRAFICO_FATTURATO - (imp_mese[i] * DELTA_1000_EURO_Y) / 1000 );

		totale_imponibile_annuo = totale_imponibile_annuo + imp_mese[i];
	}
	cairo_stroke(cr);

	/* fatturato */
	totale_fatturato_annuo = 0;
	cairo_move_to(cr, X0_GRAFICO_FATTURATO, Y0_GRAFICO_FATTURATO);
	for(i = 1; i <= (m - 1); i ++)
	{
		cairo_set_source_rgb (cr, 0.839, 0.839, 0.31);
		cairo_line_to(cr, X0_GRAFICO_FATTURATO + DELTA_MESE_X * i, Y0_GRAFICO_FATTURATO - ((imp_mese[i] + iva_mese[i])* DELTA_1000_EURO_Y) / 1000 );

		totale_fatturato_annuo = totale_fatturato_annuo + (imp_mese[i] + iva_mese[i]);
	}
	cairo_stroke(cr);

	/* IVA */
	totale_iva_annua = 0;
	cairo_move_to(cr, X0_GRAFICO_FATTURATO, Y0_GRAFICO_FATTURATO);
	for(i = 1; i <= (m - 1); i ++)
	{
        
		cairo_set_source_rgb (cr, 0.102, 0.416, 0.737);
		cairo_line_to(cr, X0_GRAFICO_FATTURATO + DELTA_MESE_X * i, Y0_GRAFICO_FATTURATO - (iva_mese[i] * DELTA_1000_EURO_Y) / 1000 );
	
		totale_iva_annua = totale_iva_annua + iva_mese[i];
	}
	cairo_stroke(cr);

	/* imposta il font per la scrittura dei valiori ottenuti */
	cairo_select_font_face(cr, "Georgia", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(cr, 13);

	/* scrive i valori dei totali annui(fatturato, imponibile, iva) nel grafico */
	cairo_set_source_rgb(cr, 0.96, 0, 0);
	cairo_move_to(cr, 780, 86);
	g_sprintf(valore, "%i", totale_imponibile_annuo);
	cairo_show_text(cr, valore);

	cairo_set_source_rgb(cr, 0.84, 0.84, 0.03);
	cairo_move_to(cr, 780, 66);
	g_sprintf(valore, "%.2f", totale_fatturato_annuo);
	cairo_show_text(cr, valore);

	cairo_set_source_rgb(cr, 0.12, 0.55, 0.94);
	cairo_move_to(cr, 780, 106);
	g_sprintf(valore, "%.2f", totale_iva_annua);
	cairo_show_text(cr, valore);


	/* scrive i nomi dei mesi con relativi imponibili e rivcavi */
	cairo_select_font_face(cr, "Georgia", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_move_to(cr, 670, 136);
	g_sprintf(valore, "%s", "Gennaio");
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.96, 0, 0);
	cairo_move_to(cr, 750, 136);
	g_sprintf(valore, "%.2f", imp_mese[1]);
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.84, 0.84, 0.03);
	cairo_move_to(cr, 820, 136);
	g_sprintf(valore, "%.2f", (imp_mese[1] + iva_mese[1]));
	cairo_show_text(cr, valore);

	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_move_to(cr, 670, 151);
	g_sprintf(valore, "%s", "Febbraio");
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.96, 0, 0);
	cairo_move_to(cr, 750, 151);
	g_sprintf(valore, "%.2f", imp_mese[2]);
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.84, 0.84, 0.03);
	cairo_move_to(cr, 820, 151);
	g_sprintf(valore, "%.2f", (imp_mese[2] + iva_mese[2]));
	cairo_show_text(cr, valore);

	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_move_to(cr, 670, 166);
	g_sprintf(valore, "%s", "Marzo");
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.96, 0, 0);
	cairo_move_to(cr, 750, 166);
	g_sprintf(valore, "%.2f", imp_mese[3]);
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.84, 0.84, 0.03);
	cairo_move_to(cr, 820, 166);
	g_sprintf(valore, "%.2f", (imp_mese[3] + iva_mese[3]));
	cairo_show_text(cr, valore);

	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_move_to(cr, 670, 181);
	g_sprintf(valore, "%s", "Aprile");
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.96, 0, 0);
	cairo_move_to(cr, 750, 181);
	g_sprintf(valore, "%.2f", imp_mese[4]);
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.84, 0.84, 0.03);
	cairo_move_to(cr, 820, 181);
	g_sprintf(valore, "%.2f", (imp_mese[4] + iva_mese[4]));
	cairo_show_text(cr, valore);

	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_move_to(cr, 670, 196);
	g_sprintf(valore, "%s", "Maggio");
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.96, 0, 0);
	cairo_move_to(cr, 750, 196);
	g_sprintf(valore, "%.2f", imp_mese[5]);
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.84, 0.84, 0.03);
	cairo_move_to(cr, 820, 196);
	g_sprintf(valore, "%.2f", (imp_mese[5] + iva_mese[5]));
	cairo_show_text(cr, valore);

	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_move_to(cr, 670, 211);
	g_sprintf(valore, "%s", "Giugno");
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.96, 0, 0);
	cairo_move_to(cr, 750, 211);
	g_sprintf(valore, "%.2f", imp_mese[6]);
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.84, 0.84, 0.03);
	cairo_move_to(cr, 820, 211);
	g_sprintf(valore, "%.2f", (imp_mese[6] + iva_mese[6]));
	cairo_show_text(cr, valore);

	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_move_to(cr, 670, 226);
	g_sprintf(valore, "%s", "Luglio");
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.96, 0, 0);
	cairo_move_to(cr, 750, 226);
	g_sprintf(valore, "%.2f", imp_mese[7]);
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.84, 0.84, 0.03);
	cairo_move_to(cr, 820, 226);
	g_sprintf(valore, "%.2f", (imp_mese[7] + iva_mese[7]));
	cairo_show_text(cr, valore);

	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_move_to(cr, 670, 241);
	g_sprintf(valore, "%s", "Agosto");
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.96, 0, 0);
	cairo_move_to(cr, 750, 241);
	g_sprintf(valore, "%.2f", imp_mese[8]);
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.84, 0.84, 0.03);
	cairo_move_to(cr, 820, 241);
	g_sprintf(valore, "%.2f", (imp_mese[8] + iva_mese[8]));
	cairo_show_text(cr, valore);

	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_move_to(cr, 670, 256);
	g_sprintf(valore, "%s", "Settembre");
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.96, 0, 0);
	cairo_move_to(cr, 750, 256);
	g_sprintf(valore, "%.2f", imp_mese[9]);
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.84, 0.84, 0.03);
	cairo_move_to(cr, 820, 256);
	g_sprintf(valore, "%.2f", (imp_mese[9] + iva_mese[9]));
	cairo_show_text(cr, valore);

	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_move_to(cr, 670, 271);
	g_sprintf(valore, "%s", "Ottobre");
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.96, 0, 0);
	cairo_move_to(cr, 750, 271);
	g_sprintf(valore, "%.2f", imp_mese[10]);
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.84, 0.84, 0.03);
	cairo_move_to(cr, 820, 271);
	g_sprintf(valore, "%.2f", (imp_mese[10] + iva_mese[10]));
	cairo_show_text(cr, valore);

	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_move_to(cr, 670, 286);
	g_sprintf(valore, "%s", "Novembre");
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.96, 0, 0);
	cairo_move_to(cr, 750, 286);
	g_sprintf(valore, "%.2f", imp_mese[11]);
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.84, 0.84, 0.03);
	cairo_move_to(cr, 820, 286);
	g_sprintf(valore, "%.2f", (imp_mese[11] + iva_mese[11]));
	cairo_show_text(cr, valore);

	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_move_to(cr, 670, 301);
	g_sprintf(valore, "%s", "Dicembre");
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.96, 0, 0);
	cairo_move_to(cr, 750, 301);
	g_sprintf(valore, "%.2f", imp_mese[12]);
	cairo_show_text(cr, valore);
	cairo_set_source_rgb(cr, 0.84, 0.84, 0.03);
	cairo_move_to(cr, 820, 301);
	g_sprintf(valore, "%.2f", (imp_mese[12] + iva_mese[12]));
	cairo_show_text(cr, valore);

	/***
	**** PARTE RELATIVA AL GRAFICO ATTIVITA' E RIMBORSI CARBURANTE
	****/
	calcola_attivita_tot_mesi(totali_attivita_mesi, str_a);

	for(i = 1; i <= (m - 1); i ++)
	{
		totale_ch_t_anno = totale_ch_t_anno + totali_attivita_mesi[i].ch_t;
		totale_ch_tl_anno = totale_ch_tl_anno + totali_attivita_mesi[i].ch_tl;
		totale_uvt_anno = totale_uvt_anno + totali_attivita_mesi[i].uvt;
		totale_ch_p_anno = totale_ch_p_anno + totali_attivita_mesi[i].ch_p;
		totale_dis_p_anno = totale_dis_p_anno + totali_attivita_mesi[i].dis_p;
		totale_uvp_anno = totale_uvp_anno + totali_attivita_mesi[i].uvp;
		totale_za_anno = totale_za_anno + totali_attivita_mesi[i].za;
		totale_zb_anno = totale_zb_anno + totali_attivita_mesi[i].zb;
		totale_zc_anno = totale_zc_anno + totali_attivita_mesi[i].zc;
		totale_lav_ore_anno = totale_lav_ore_anno + totali_attivita_mesi[i].lav_ore;
	}

	cairo_move_to(cr, X0_GRAFICO_ATTIVITA, Y0_GRAFICO_ATTIVITA);
	cairo_set_source_rgb(cr, 0.067, 0.514, 0.024);
	for(i = 1; i <= (m - 1); i ++)
	{
		cairo_line_to(cr, X0_GRAFICO_ATTIVITA + (DELTA_MESE_X * i), Y0_GRAFICO_ATTIVITA - (totali_attivita_mesi[i].ch_t * DELTA_1_ATTIVITA_Y));
	}
	cairo_stroke(cr);
	cairo_move_to(cr, 830, 399);
	g_sprintf(valore, "%i", totale_ch_t_anno);
	cairo_show_text(cr, valore);

	cairo_move_to(cr, X0_GRAFICO_ATTIVITA, Y0_GRAFICO_ATTIVITA);
	cairo_set_source_rgb(cr, 0.859, 0.224, 0.808);
	for(i = 1; i <= (m - 1); i ++)
	{  
		cairo_line_to(cr, X0_GRAFICO_ATTIVITA + (DELTA_MESE_X * i), Y0_GRAFICO_ATTIVITA - (totali_attivita_mesi[i].ch_tl * DELTA_1_ATTIVITA_Y));
	}
	cairo_stroke(cr);
	cairo_move_to(cr, 830, 419);
	g_sprintf(valore, "%i", totale_ch_tl_anno);
	cairo_show_text(cr, valore);

	cairo_move_to(cr, X0_GRAFICO_ATTIVITA, Y0_GRAFICO_ATTIVITA);
	cairo_set_source_rgb(cr, 0.957, 0.286, 0.106);
	for(i = 1; i <= (m - 1); i ++)
	{  
		cairo_line_to(cr, X0_GRAFICO_ATTIVITA + (DELTA_MESE_X * i), Y0_GRAFICO_ATTIVITA - (totali_attivita_mesi[i].uvt * DELTA_1_ATTIVITA_Y));
	}
	cairo_stroke(cr);
	cairo_move_to(cr, 830, 439);
	g_sprintf(valore, "%i", totale_uvt_anno);
	cairo_show_text(cr, valore);

	cairo_move_to(cr, X0_GRAFICO_ATTIVITA, Y0_GRAFICO_ATTIVITA);
	cairo_set_source_rgb(cr, 0.957, 0.969, 0.106);
	for(i = 1; i <= (m - 1); i ++)
	{  
		cairo_line_to(cr, X0_GRAFICO_ATTIVITA + (DELTA_MESE_X * i), Y0_GRAFICO_ATTIVITA - (totali_attivita_mesi[i].ch_p * DELTA_1_ATTIVITA_Y));
	}
	cairo_stroke(cr);
	cairo_move_to(cr, 830, 459);
	g_sprintf(valore, "%i", totale_ch_p_anno);
	cairo_show_text(cr, valore);

	cairo_move_to(cr, X0_GRAFICO_ATTIVITA, Y0_GRAFICO_ATTIVITA);
	cairo_set_source_rgb(cr, 0.106, 0.145, 0.969);
	for(i = 1; i <= (m - 1); i ++)
	{  
		cairo_line_to(cr, X0_GRAFICO_ATTIVITA + (DELTA_MESE_X * i), Y0_GRAFICO_ATTIVITA - (totali_attivita_mesi[i].dis_p * DELTA_1_ATTIVITA_Y));
	}
	cairo_stroke(cr);
	cairo_move_to(cr, 830, 479);
	g_sprintf(valore, "%i", totale_dis_p_anno);
	cairo_show_text(cr, valore);

	cairo_move_to(cr, X0_GRAFICO_ATTIVITA, Y0_GRAFICO_ATTIVITA);
	cairo_set_source_rgb(cr, 1, 0, 0);
	for(i = 1; i <= (m - 1); i ++)
	{  
		cairo_line_to(cr, X0_GRAFICO_ATTIVITA + (DELTA_MESE_X * i), Y0_GRAFICO_ATTIVITA - (totali_attivita_mesi[i].uvp * DELTA_1_ATTIVITA_Y));
	}
	cairo_stroke(cr);
	cairo_move_to(cr, 830, 499);
	g_sprintf(valore, "%i", totale_uvp_anno);
	cairo_show_text(cr, valore);

	cairo_move_to(cr, X0_GRAFICO_ATTIVITA, Y0_GRAFICO_ATTIVITA);
	cairo_set_source_rgb(cr, 0.369, 0.133, 0.988);
	for(i = 1; i <= (m - 1); i ++)
	{  
		cairo_line_to(cr, X0_GRAFICO_ATTIVITA + (DELTA_MESE_X * i), Y0_GRAFICO_ATTIVITA - (totali_attivita_mesi[i].za * DELTA_1_ATTIVITA_Y));
	}
	cairo_stroke(cr);
	cairo_move_to(cr, 830, 519);
	g_sprintf(valore, "%i", totale_za_anno);
	cairo_show_text(cr, valore);

	cairo_move_to(cr, X0_GRAFICO_ATTIVITA, Y0_GRAFICO_ATTIVITA);
	cairo_set_source_rgb(cr, 0.439, 0.239, 0.008);
	for(i = 1; i <= (m - 1); i ++)
	{
		cairo_line_to(cr, X0_GRAFICO_ATTIVITA + (DELTA_MESE_X * i), Y0_GRAFICO_ATTIVITA - (totali_attivita_mesi[i].zb * DELTA_1_ATTIVITA_Y));
	}
	cairo_stroke(cr);
	cairo_move_to(cr, 830, 539);
	g_sprintf(valore, "%i", totale_zb_anno);
	cairo_show_text(cr, valore);

	cairo_move_to(cr, X0_GRAFICO_ATTIVITA, Y0_GRAFICO_ATTIVITA);
	cairo_set_source_rgb(cr, 0.020, 0.588, 0.580);
	for(i = 1; i <= (m - 1); i ++)
	{
		cairo_line_to(cr, X0_GRAFICO_ATTIVITA + (DELTA_MESE_X * i), Y0_GRAFICO_ATTIVITA - (totali_attivita_mesi[i].zc * DELTA_1_ATTIVITA_Y));
	}
	cairo_stroke(cr);
	cairo_move_to(cr, 830, 559);
	g_sprintf(valore, "%i", totale_zc_anno);
	cairo_show_text(cr, valore);

	cairo_move_to(cr, X0_GRAFICO_ATTIVITA, Y0_GRAFICO_ATTIVITA);
	cairo_set_source_rgb(cr, 0.420, 0.439, 0.439);
	for(i = 1; i <= (m - 1); i ++)
	{  
		cairo_line_to(cr, X0_GRAFICO_ATTIVITA + (DELTA_MESE_X * i), Y0_GRAFICO_ATTIVITA - (totali_attivita_mesi[i].lav_ore * DELTA_1_ATTIVITA_Y));
	}
	cairo_stroke(cr);
	cairo_move_to(cr, 830, 579);
	g_sprintf(valore, "%i", totale_lav_ore_anno);
	cairo_show_text(cr, valore);

	/* ottiene il pixbuf corrispondente al 'surface cairo' che contiene l'immagine */
	pixbuf = gdk_pixbuf_get_from_surface(immagine_grafico, 0, 0, 900, 675);

	/* setta il pixbuf nell' image_widget , CIOÈ VISUALIZZA L'IMMAGINE .PNG NELL' IMAGE_WIDGET, che è quello che vogliamo */
	gtk_image_set_from_pixbuf(GTK_IMAGE(image_stat_annuali), pixbuf);

	return;
}


/* se viene premuto il 'button_aggiorna' della schermata statistiche annuali */
static gboolean pressed_button_aggiorna(GtkWidget *widget, gpointer callback_data)
{
	const gchar *str_anno_scelto;
	int anno_scelto;

	/* ottiene la stringa e poi la converte anche in intero dell'anno di cui vogliamo visualizzare il grafico dall'apposito entry*/
	str_anno_scelto = gtk_entry_get_text(entry_anno_scelto);
	anno_scelto= str_to_int((gchar*)str_anno_scelto);

	disegna_grafico_stat_annuali(anno_scelto, str_anno_scelto);

	return FALSE;
}

/* se viene premuto il 'button_reg_chiamata' */
static gboolean pressed_button_reg_chiamata(GtkWidget *widget, gpointer callback_data)
{
	/* anticipa la scrittura di giorno , mese e anno correnti negli appositi entry della schermata registra chiamata */
	gchar   str_giorno[MAX_CHAR_GG + 1],
	str_mese[MAX_CHAR_MM +1],
	str_anno[MAX_CHAR_AAAA +1];

	g_sprintf(str_giorno, "%i", giorno);       	// converte in stringa i numeri
	g_sprintf(str_mese, "%i", mese); 		// di mese giorno e anno
	g_sprintf(str_anno, "%i", anno);

	/* se le stringhe di giorno e mese sono formate da un solo carattere, es: 1, 2, 3 ... , le converte in 01, 02, 03 ... */
	if(strlen(str_giorno) == 1) g_sprintf(str_giorno, "0%i", giorno);
	if(strlen(str_mese) == 1) g_sprintf(str_mese, "0%i", mese);

	gtk_entry_set_text(entry_gg, str_giorno);
	gtk_entry_set_text(entry_mm, str_mese);
	gtk_entry_set_text(entry_aaaa, str_anno);

	/* crea l'elenco dei clienti per il combo apposito, leggendo i clienti salvati nel file clienti.gec .E li ritorna sotto forma di lista concatenata (GList) */
	elenco_clienti = NULL;     /* NB: il puntatore a GList DEVE sempre essere inizializzato a NULL */
	elenco_clienti = crea_elenco_clienti(elenco_clienti);

	/* conta gli elementi della lista elenco_clienti*/
	guint n_elementi = g_list_length(elenco_clienti);

	/* ordina alfabeticamente le stringhe della lista elenco clienti */
	elenco_clienti_ordinato = NULL;     
	elenco_clienti_ordinato = ordina_alfabeticamente(n_elementi, elenco_clienti, elenco_clienti_ordinato);

	/* rimuove tutti gli elementi dal combo_box_text_cliente altrimenti ogni volta che si esce e si rientra nella schermata 'registra chiamata' 
	 * al combo_box_text_cliente viene aggiunta un' altra lista di clienti successiva a quella gia presente creando una ripetizione degli elementi */
	gtk_combo_box_text_remove_all(combo_box_text_cliente);

	/* assegna l'elenco clienti al combo prendendo uno alle volta gli elementi della lista */
	while(elenco_clienti_ordinato->next != 0)
	{
		gtk_combo_box_text_append_text(combo_box_text_cliente, elenco_clienti_ordinato->data);
		elenco_clienti_ordinato = elenco_clienti_ordinato->next;

		/* senza questa uscirebbe dal ciclo prima di aver inserito l'ultimo elemento della lista */
		if(elenco_clienti_ordinato->next == 0) gtk_combo_box_text_append_text(combo_box_text_cliente, elenco_clienti_ordinato->data);
	}

	/* come per il combo dei clienti ma per le apparecchiature */
	elenco_apparecchiature = NULL;
	elenco_apparecchiature = crea_elenco_appar(elenco_apparecchiature);

	/* conta gli elementi della lista elenco_apparecchiature*/
	n_elementi = g_list_length(elenco_apparecchiature);

	/* ordina alfabeticamente le stringhe della lista elenco apparecchiature */
	elenco_apparecchiature_ordinato = NULL;     
	elenco_apparecchiature_ordinato = ordina_alfabeticamente(n_elementi, elenco_apparecchiature, elenco_apparecchiature_ordinato);

	gtk_combo_box_text_remove_all(combo_box_text_appar);

	while(elenco_apparecchiature_ordinato->next != 0)
	{
		gtk_combo_box_text_append_text(combo_box_text_appar, elenco_apparecchiature_ordinato->data);
		elenco_apparecchiature_ordinato = elenco_apparecchiature_ordinato->next;
		if(elenco_apparecchiature_ordinato->next == 0) 
		gtk_combo_box_text_append_text(combo_box_text_appar, elenco_apparecchiature_ordinato->data);
	}

	cambia_schermata(box_menu_princ, box_reg_chiamata);

	return FALSE;
}


/* se viene premuto il pulsante 'salva compensi attività' */
static gboolean pressed_button_salva_comp_att(GtkWidget *widget, gpointer callback_data)
{
	/* ottieni le stringhe dei vari compensi dagli entry */
	const gchar* str_comp_ch_t = gtk_entry_get_text(entry_comp_ch_t);
	const gchar* str_comp_ch_t_dec = gtk_entry_get_text(entry_comp_ch_t_dec);
	const gchar* str_comp_ch_tl = gtk_entry_get_text(entry_comp_ch_tl);
	const gchar* str_comp_ch_tl_dec = gtk_entry_get_text(entry_comp_ch_tl_dec);
	const gchar* str_comp_uvt = gtk_entry_get_text(entry_comp_uvt);
	const gchar* str_comp_uvt_dec = gtk_entry_get_text(entry_comp_uvt_dec);
	const gchar* str_comp_ch_p = gtk_entry_get_text(entry_comp_ch_p);
	const gchar* str_comp_ch_p_dec = gtk_entry_get_text(entry_comp_ch_p_dec);
	const gchar* str_comp_dis_p = gtk_entry_get_text(entry_comp_dis_p);
	const gchar* str_comp_dis_p_dec = gtk_entry_get_text(entry_comp_dis_p_dec);
	const gchar* str_comp_uvp = gtk_entry_get_text(entry_comp_uvp);
	const gchar* str_comp_uvp_dec = gtk_entry_get_text(entry_comp_uvp_dec);
	const gchar* str_comp_zb = gtk_entry_get_text(entry_comp_zb);
	const gchar* str_comp_zb_dec = gtk_entry_get_text(entry_comp_zb_dec);
	const gchar* str_comp_zc = gtk_entry_get_text(entry_comp_zc);
	const gchar* str_comp_zc_dec = gtk_entry_get_text(entry_comp_zc_dec);
	const gchar* str_comp_lav_ore = gtk_entry_get_text(entry_comp_lav_ore);
	const gchar* str_comp_lav_ore_dec = gtk_entry_get_text(entry_comp_lav_ore_dec);
	const gchar* str_comp_iva = gtk_entry_get_text(entry_comp_iva);

	/* controlla che i compensi siano tutti inseriti con due cifre nella parte intera, anche quelli inferiori a dieci. Ad esempio se dobbiamo inserire un compenso 
	 * di 5 euro, dobbiamo inserirlo come 05. se non facessimo  cosi si incasinerebbe tutto il file compensi. In caso di errore lo sgnala con un dialog */
	if((strlen(str_comp_ch_t) < 2) || (strlen(str_comp_ch_tl) < 2) || (strlen(str_comp_uvt) < 2) || (strlen(str_comp_ch_p) < 2) ||
	(strlen(str_comp_dis_p) < 2) || (strlen(str_comp_uvp) < 2) || (strlen(str_comp_zb) < 2) || (strlen(str_comp_zc) < 2) ||
	(strlen(str_comp_lav_ore) < 2) || (strlen(str_comp_iva) < 2))
	{
		crea_dialog_box_un_pulsante(dialog_comp_err, "ATTENZIONE!",
			"\n   La parte intera dei compensi deve essere di due cifre,\n  ad esempio un compenso di 5 euro deve essere inserito come 05.    \n",
			window_principale, "Chiudi");

		return FALSE;
	}

	/* prima salviamo i nuovi valori dei compensi nel file compensi.gec, poi li salviamo anche nel file chiamate mese in esame */

	/* gfile relativo al file compensi */
	file_compensi = g_file_new_for_path(file_compensi_path);

	/* prova ad aprire il file compensi in lettura/scrittura */
	file_compensi_iostream = g_file_open_readwrite(file_compensi, NULL, NULL);

	/* stream per scrivere nel file compensi */
	GOutputStream *output_stream;
	output_stream = g_io_stream_get_output_stream((GIOStream*)file_compensi_iostream);

	/* scrive in ordine i vari compensi nel file compensi e separa ogni compenso con carattere nullo ("\0") come deve essere */
	g_output_stream_write(output_stream, str_comp_ch_t, strlen(str_comp_ch_t), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_ch_t_dec, strlen(str_comp_ch_t_dec), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_ch_tl, strlen(str_comp_ch_tl), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_ch_tl_dec, strlen(str_comp_ch_tl_dec), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_uvt, strlen(str_comp_uvt), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_uvt_dec, strlen(str_comp_uvt_dec), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_ch_p, strlen(str_comp_ch_p), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_ch_p_dec, strlen(str_comp_ch_p_dec), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_dis_p, strlen(str_comp_dis_p), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_dis_p_dec, strlen(str_comp_dis_p_dec), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_uvp, strlen(str_comp_uvp), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_uvp_dec, strlen(str_comp_uvp_dec), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_zb, strlen(str_comp_zb), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_zb_dec, strlen(str_comp_zb_dec), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_zc, strlen(str_comp_zc), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_zc_dec, strlen(str_comp_zc_dec), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_lav_ore, strlen(str_comp_lav_ore), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_lav_ore_dec, strlen(str_comp_lav_ore_dec), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_iva, strlen(str_comp_iva), NULL, NULL);

	g_output_stream_close(output_stream, NULL, NULL);
	g_io_stream_close((GIOStream*)file_compensi_iostream, NULL, NULL);

	/* ora salviamo i nuovi valori dei compensi nel file chiamate mese in esame */

	/* gfile relativo al file chiamate mese */
	file_chiamate_mese = g_file_new_for_path(file_chiamate_mese_path);

	/* prova ad aprire il file chiamate mese in lettura/scrittura */
	file_chiamate_mese_iostream = g_file_open_readwrite(file_chiamate_mese, NULL, NULL);

	/* stream per scrivere nel file chiamate mese */
	output_stream = g_io_stream_get_output_stream((GIOStream*)file_chiamate_mese_iostream);

	/* sposta la posizione dello stream al byte 192 da poter scrivere nel file partendo da esso*/
	g_seekable_seek((GSeekable *)output_stream, OFFSET_COMPENSI, G_SEEK_CUR, NULL, NULL);

	/* scrive in ordine i vari compensi nel file chiamate mese e separa ogni compenso con carattere nullo ("\0") come deve essere */
	g_output_stream_write(output_stream, str_comp_ch_t, strlen(str_comp_ch_t), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_ch_t_dec, strlen(str_comp_ch_t_dec), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_ch_tl, strlen(str_comp_ch_tl), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_ch_tl_dec, strlen(str_comp_ch_tl_dec), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_uvt, strlen(str_comp_uvt), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_uvt_dec, strlen(str_comp_uvt_dec), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_ch_p, strlen(str_comp_ch_p), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_ch_p_dec, strlen(str_comp_ch_p_dec), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_dis_p, strlen(str_comp_dis_p), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_dis_p_dec, strlen(str_comp_dis_p_dec), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_uvp, strlen(str_comp_uvp), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_uvp_dec, strlen(str_comp_uvp_dec), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_zb, strlen(str_comp_zb), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_zb_dec, strlen(str_comp_zb_dec), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_zc, strlen(str_comp_zc), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_zc_dec, strlen(str_comp_zc_dec), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_lav_ore, strlen(str_comp_lav_ore), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_lav_ore_dec, strlen(str_comp_lav_ore_dec), NULL, NULL);
	g_output_stream_write(output_stream, "\0", 1, NULL, NULL);
	g_output_stream_write(output_stream, str_comp_iva, strlen(str_comp_iva), NULL, NULL);

	g_output_stream_close(output_stream, NULL, NULL);
	g_io_stream_close((GIOStream*)file_chiamate_mese_iostream, NULL, NULL);

	return FALSE;
}


/* se viene premuto il pulsante 'salva' della schermata registra chiamata */
static gboolean pressed_button_salva_chiamata(GtkWidget *widget, gpointer callback_data)
{
	chiamata ch;
	gboolean flag_cht, flag_chtl, flag_uvt, flag_chp, flag_dis, flag_uvp, flag_za, flag_zb, flag_zc;
	gboolean flag_catns;
	gint ore_lav;
	gint n;
	gchar  *s;
	gchar *buffer_chiamata_utf16;
	gunichar2 *s_utf16;
	GOutputStream *output_stream;
	goffset offset;

	/* ottiene dimensione file chiamate mese */
	file_chiamate_mese_size = get_file_ch_mese_size();

	/* ottiene dati della chiamata  da salvare */
	ch.num_chiamata = (file_chiamate_mese_size / LENGHT_CHIAMATA_BYTE);
	ch.data_gg = gtk_entry_get_text(entry_gg);
	ch.data_mm = gtk_entry_get_text(entry_mm);
	ch.data_aaaa = gtk_entry_get_text(entry_aaaa);
	ch.ora_spost = gtk_entry_get_text(entry_ora_in_spost);
	ch.ora_in_lav = gtk_entry_get_text(entry_ora_in_lav);
	ch.ora_fine_lav = gtk_entry_get_text(entry_ora_fine_lav);
	ch.cliente = gtk_entry_get_text(entry_cliente);
	ch.luogo = gtk_entry_get_text(entry_luogo);
	ch.appar = gtk_entry_get_text(entry_appar);
	ch.guasto = gtk_entry_get_text(entry_guasto);
	ch.lav_eseguito = gtk_entry_get_text(entry_lav_eseguito);
	ch.ore_lav = gtk_entry_get_text(entry_ore_lav);
	ch.comp_att_tariffa_non_stand = gtk_entry_get_text(entry_compenso);
	ch.comp_att_tariffa_non_stand_cent = gtk_entry_get_text(entry_compenso_cent);

	/* ottiene stati dei vari flags e li memorizza tutti in un unico buffer */
	flag_cht = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_cht));
	g_sprintf(&ch.buffer_flags[0], "%i", flag_cht);
	flag_chtl = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_chtl));
	g_sprintf(&ch.buffer_flags[1], "%i", flag_chtl);
	flag_uvt = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_uvt));
	g_sprintf(&ch.buffer_flags[2], "%i", flag_uvt);
	flag_chp = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_chp));
	g_sprintf(&ch.buffer_flags[3], "%i", flag_chp);
	flag_dis = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_dis));
	g_sprintf(&ch.buffer_flags[4], "%i", flag_dis);
	flag_uvp = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_uvp));
	g_sprintf(&ch.buffer_flags[5], "%i", flag_uvp);
	flag_za = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_za));
	g_sprintf(&ch.buffer_flags[6], "%i", flag_za);
	flag_zb = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_zb));
	g_sprintf(&ch.buffer_flags[7], "%i", flag_zb);
	flag_zc = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_zc));
	g_sprintf(&ch.buffer_flags[8], "%i", flag_zc);
	ch.buffer_flags[9] = 0;

	/* controllo se è flaggato il compenso per attività a tariffa non standard. se cosi è, controllo che non sia selezionato nessun altro flag e che 
	 * non siano segnate ore di lavoro. se tutte queste condizioni sono soddisfatte vuol dire che sto salvando una chiamata concompenso a tariffa 
	 * non standard e quindi salto tutti gli altri controlli e procedo col salvataggio. altrimenti segnalo l'errore con l'apposito dialog. */
	flag_catns = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_compenso));
	ore_lav = atoi(ch.ore_lav);
	if(flag_catns == 1)
	{
		if((flag_cht + flag_chtl + flag_uvt + flag_chp + flag_dis + flag_uvp + flag_za + flag_zb + flag_zc + ore_lav) == 0)
		{
			goto FINE_CONTROLLI;
		}
		else
		{
			crea_dialog_box_un_pulsante(dialog_flag, "ATTENZIONE!",
			"\n   Devi flaggare o UN TIPO DI CHIAMATA e UNA ZONA o    \n   ORE DI LAVORO e UNA ZONA o   \n\
	CHIAMATA A TARIFFA NON STANDARD E METTERE L'IMPORTO",
						window_principale, "Chiudi");
			return FALSE;
		}
	}

	/* controllo che sia flaggata almeno un TIPO DI CHIAMATA e una ZONA, oppure ORE DI LAVORO e una ZONA ,se nessuna ZONA flaggata da errore */
	if(((flag_za + flag_zb + flag_zc) != 1) )
	{
		crea_dialog_box_un_pulsante(dialog_flag, "ATTENZIONE!",
		"\n   Devi flaggare o UN TIPO DI CHIAMATA e UNA ZONA o    \n   ORE DI LAVORO e UNA ZONA o   \n\
	CHIAMATA A TARIFFA NON STANDARD E METTERE L'IMPORTO",
						window_principale, "Chiudi");
		return FALSE;
	}
	/* se ZONA flaggata, ma inseriti più di un TIPO DI CHIAMATA ,segnala errore */
	if(((flag_cht + flag_chtl + flag_uvt + flag_chp + flag_dis + flag_uvp) > 1))
	{
		crea_dialog_box_un_pulsante(dialog_flag, "ATTENZIONE!",
		"\n   Devi flaggare o UN TIPO DI CHIAMATA e UNA ZONA o    \n   ORE DI LAVORO e UNA ZONA o   \n\
	CHIAMATA A TARIFFA NON STANDARD E METTERE L'IMPORTO",
						window_principale, "Chiudi");
		return FALSE;
	}
	/* se ZONA flaggata, ma inserita sia TIPO DI CHIAMATA sia ORA DI LAVORO , segnala errore */
	if(((flag_cht + flag_chtl + flag_uvt + flag_chp + flag_dis + flag_uvp) == 1) && (*ch.ore_lav != 0))
	{
		crea_dialog_box_un_pulsante(dialog_flag, "ATTENZIONE!",
		"\n   Devi flaggare o UN TIPO DI CHIAMATA e UNA ZONA o    \n   ORE DI LAVORO e UNA ZONA o   \n\
	CHIAMATA A TARIFFA NON STANDARD E METTERE L'IMPORTO",
						window_principale, "Chiudi");
		return FALSE;
	}
	/* se ZONA flaggata, ma non inserita nè TIPO DI CHIAMATA nè ORA DI LAVORO , segnala errore */
	if(((flag_cht + flag_chtl + flag_uvt + flag_chp + flag_dis + flag_uvp) == 0) && (*ch.ore_lav == 0))
	{
		crea_dialog_box_un_pulsante(dialog_flag, "ATTENZIONE!",
		"\n   Devi flaggare o UN TIPO DI CHIAMATA e UNA ZONA o    \n   ORE DI LAVORO e UNA ZONA o   \n\
	CHIAMATA A TARIFFA NON STANDARD E METTERE L'IMPORTO	",
						window_principale, "Chiudi");
		return FALSE;
	}

	FINE_CONTROLLI :

	s = (gchar *) g_malloc0(MAX_CHAR_LAV_ESEG);		/* allocca il buffer (MAX_CHAR_LAV_ESEG è il massimo numero di caratteri che 
								 * potrà avere la stringa perchè è il campo più grande) per la stringa s e lo setta a tutti zeri */
	buffer_chiamata_utf16 = (gchar *) g_malloc0(LENGHT_CHIAMATA_BYTE);		// allocca ilbuffer per la chiamata da salvare e lo setta a tutti zeri

	/* crea il buffer della chiamata da salvare con tutti i valori ottenuti dai vari campi immessi dall'utente */
	n = g_sprintf(s, "N/%i",ch.num_chiamata);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[0], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);						// resetta il buffer per la stringa s
	n = g_sprintf(s,  "Dt/%s/%s/%s",ch.data_gg, ch.data_mm, ch.data_aaaa);		// riempie la stringa s e in n ritorna i byte scritti
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);				// converte la stringa s in utf16
	memcpy(&buffer_chiamata_utf16[20], s_utf16, n *2);				// copia la stringa s nella giusta posizione nel  buffer

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s,  "Ore/%s",ch.ora_spost);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[50], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s,  "%s",ch.ora_in_lav);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[70], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s,  "%s",ch.ora_fine_lav);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[90], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s,  "Cl/%s",ch.cliente);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[100], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s, "LdL/%s",ch.luogo);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[200], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s, "Ap/%s",ch.appar);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[300], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s, "Dif/%s",ch.guasto);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[380], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s, "LE/%s",ch.lav_eseguito);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[460], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s, "CATNS/%s",ch.comp_att_tariffa_non_stand);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[1460], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s, "%s",ch.comp_att_tariffa_non_stand_cent);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[1488], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s, "Flag/%s",ch.buffer_flags);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[1500], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s, "LI/%s",ch.ore_lav);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[1540], s_utf16, n * 2);

	free(s);

	/* apre in lettura e scrittura il file chiamate mese */
	file_chiamate_mese = g_file_new_for_path(file_chiamate_mese_path);
	file_chiamate_mese_iostream = g_file_open_readwrite(file_chiamate_mese, NULL, NULL);

	/* ottiene lo stream per scrivere nel file chiamate */
	output_stream = g_io_stream_get_output_stream((GIOStream*)file_chiamate_mese_iostream);

	/* ottiene la posizione nello stream che in questo caso visto che non abbiamo ancora fatto nessuna operazione con esso , sarà zero*/
	offset = g_seekable_tell((GSeekable *)output_stream);

	/* sposta la posizione alla fine dello stream in modo da poter scrivere nel file partendo dalla fine di esso*/
	g_seekable_seek((GSeekable *)output_stream, offset, G_SEEK_END, NULL, NULL);

	/* aggiunge la chiamata da salvare al file chiamate mese scrivendola in coda ad esso */
	g_output_stream_write(output_stream, buffer_chiamata_utf16, LENGHT_CHIAMATA_BYTE, NULL, NULL);

	/* chiude i vari stream */
	g_output_stream_close(output_stream, NULL, NULL);
	g_io_stream_close((GIOStream*)file_chiamate_mese_iostream, NULL, NULL);

	free(buffer_chiamata_utf16);

	return FALSE;
}


/* se viene premuto il pulsante 'salva' della schermata modifica chiamata */
static gboolean pressed_button_salva_chiamata1(GtkWidget *widget, gpointer callback_data)
{
	chiamata ch;
	gboolean flag_cht, flag_chtl, flag_uvt, flag_chp, flag_dis, flag_uvp, flag_za, flag_zb, flag_zc;
	gboolean flag_catns;
	gint ore_lav;
	gint n;
	gchar  *s;
	gchar *buffer_chiamata_utf16;
	gunichar2 *s_utf16;
	GOutputStream *output_stream;

	/* ottiene dati della chiamata modificata da salvare */
	ch.num_chiamata = num_chiamata_selezionata;
	ch.data_gg = gtk_entry_get_text(entry_gg1);
	ch.data_mm = gtk_entry_get_text(entry_mm1);
	ch.data_aaaa = gtk_entry_get_text(entry_aaaa1);
	ch.ora_spost = gtk_entry_get_text(entry_ora_in_spost1);
	ch.ora_in_lav = gtk_entry_get_text(entry_ora_in_lav1);
	ch.ora_fine_lav = gtk_entry_get_text(entry_ora_fine_lav1);
	ch.cliente = gtk_entry_get_text(entry_cliente1);
	ch.luogo = gtk_entry_get_text(entry_luogo1);
	ch.appar = gtk_entry_get_text(entry_appar1);
	ch.guasto = gtk_entry_get_text(entry_guasto1);
	ch.lav_eseguito = gtk_entry_get_text(entry_lav_eseguito1);
	ch.ore_lav = gtk_entry_get_text(entry_ore_lav1);
	ch.comp_att_tariffa_non_stand = gtk_entry_get_text(entry_compenso1);
	ch.comp_att_tariffa_non_stand_cent = gtk_entry_get_text(entry_compenso_cent1);

	/* ottiene stati dei vari flags e li memorizza tutti in un unico buffer */
	flag_cht = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_cht1));
	g_sprintf(&ch.buffer_flags[0], "%i", flag_cht);
	flag_chtl = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_chtl1));
	g_sprintf(&ch.buffer_flags[1], "%i", flag_chtl);
	flag_uvt = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_uvt1));
	g_sprintf(&ch.buffer_flags[2], "%i", flag_uvt);
	flag_chp = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_chp1));
	g_sprintf(&ch.buffer_flags[3], "%i", flag_chp);
	flag_dis = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_dis1));
	g_sprintf(&ch.buffer_flags[4], "%i", flag_dis);
	flag_uvp = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_uvp1));
	g_sprintf(&ch.buffer_flags[5], "%i", flag_uvp);
	flag_za = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_za1));
	g_sprintf(&ch.buffer_flags[6], "%i", flag_za);
	flag_zb = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_zb1));
	g_sprintf(&ch.buffer_flags[7], "%i", flag_zb);
	flag_zc = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_zc1));
	g_sprintf(&ch.buffer_flags[8], "%i", flag_zc);
	ch.buffer_flags[9] = 0;

	/* controllo se è flaggato il compenso per attività a tariffa non standard. se cosi è, controllo che non sia selezionato nessun altro flag
	 * e che non siano segnate ore di lavoro. se tutte queste condizioni sono soddisfatte vuol dire che sto salvando una chiamata concompenso a 
	 * tariffa non standard e quindi salto tutti gli altri controlli e procedo col salvataggio. altrimenti segnalo l'errore con l'apposito dialog. */
	flag_catns = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_compenso1));
	ore_lav = atoi(ch.ore_lav);
	if(flag_catns == 1)
	{
		if((flag_cht + flag_chtl + flag_uvt + flag_chp + flag_dis + flag_uvp + flag_za + flag_zb + flag_zc + ore_lav) == 0)
		{
			goto FINE_CONTROLLI;
		}
		else
		{
			crea_dialog_box_un_pulsante(dialog_flag, "ATTENZIONE!",
			"\n	Devi flaggare o UN TIPO DI CHIAMATA e UNA ZONA o \n	ORE DI LAVORO e UNA ZONA o   \n\
	CHIAMATA A TARIFFA NON STANDARD E METTERE L'IMPORTO	\n", window_modifica_chiamata, "Chiudi");
		}
	}

	/* controllo che sia flaggata almeno un TIPO DI CHIAMATA e una ZONA, oppure ORE DI LAVORO e una ZONA , se nessuna ZONA flaggata da errore */
	if((flag_za + flag_zb + flag_zc) != 1)
	{
		crea_dialog_box_un_pulsante(dialog_flag, "ATTENZIONE!",
		"\n	Devi flaggare o UN TIPO DI CHIAMATA e UNA ZONA o \n	ORE DI LAVORO e UNA ZONA o   \n\
	CHIAMATA A TARIFFA NON STANDARD E METTERE L'IMPORTO	\n", window_modifica_chiamata, "Chiudi");
	}
	/* se ZONA flaggata, ma inseriti più di TIPO DI CHIAMATA , segnala errore */
	if(((flag_cht + flag_chtl + flag_uvt + flag_chp + flag_dis + flag_uvp) > 1))
	{
		crea_dialog_box_un_pulsante(dialog_flag, "ATTENZIONE!",
		"\n	Devi flaggare o UN TIPO DI CHIAMATA e UNA ZONA o \n	ORE DI LAVORO e UNA ZONA o   \n\
	CHIAMATA A TARIFFA NON STANDARD E METTERE L'IMPORTO	\n", window_modifica_chiamata, "Chiudi");
	}
	/* se ZONA flaggata, ma inserita sia TIPO DI CHIAMATA sia ORA DI LAVORO , segnala errore */
	if(((flag_cht + flag_chtl + flag_uvt + flag_chp + flag_dis + flag_uvp) == 1) && (*ch.ore_lav != 0))
	{
		crea_dialog_box_un_pulsante(dialog_flag, "ATTENZIONE!",
		"\n	Devi flaggare o UN TIPO DI CHIAMATA e UNA ZONA o \n	ORE DI LAVORO e UNA ZONA o   \n\
	CHIAMATA A TARIFFA NON STANDARD E METTERE L'IMPORTO	\n", window_modifica_chiamata, "Chiudi");
	}
	/* se ZONA flaggata, ma non inserita nè TIPO DI CHIAMATA nè ORA DI LAVORO , segnala errore */
	if(((flag_cht + flag_chtl + flag_uvt + flag_chp + flag_dis + flag_uvp) == 0) && (*ch.ore_lav == 0))
	{
		crea_dialog_box_un_pulsante(dialog_flag, "ATTENZIONE!",
		"\n	Devi flaggare o UN TIPO DI CHIAMATA e UNA ZONA o \n	ORE DI LAVORO e UNA ZONA o   \n\
	CHIAMATA A TARIFFA NON STANDARD E METTERE L'IMPORTO	\n", window_modifica_chiamata, "Chiudi");
	}

	FINE_CONTROLLI:

	s = (gchar *) g_malloc0(MAX_CHAR_LAV_ESEG);	// allocca il buffer (MAX_CHAR_LAV_ESEG è il massimo numero di caratteri che potrà avere la stringa  
							// perchè è il campo più grande) per la stringa s e lo setta a tutti zeri
	buffer_chiamata_utf16 = (gchar *) g_malloc0(LENGHT_CHIAMATA_BYTE);		// allocca il buffer per la chiamata da salvare e lo setta a tutti zeri

	/* crea il buffer della chiamata da salvare con tutti i valori ottenuti dai vari campi immessi dall'utente */
	n = g_sprintf(s, "N/%i",ch.num_chiamata);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[0], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);						// resetta il buffer per la stringa s
	n = g_sprintf(s,  "Dt/%s/%s/%s",ch.data_gg, ch.data_mm, ch.data_aaaa);		// riempie la stringa s e in n ritorna i byte scritti
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);				// converte la stringa s in utf16
	memcpy(&buffer_chiamata_utf16[20], s_utf16, n *2);				// copia la stringa s nella giusta posizione nel  buffer

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s,  "Ore/%s",ch.ora_spost);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[50], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s,  "%s",ch.ora_in_lav);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[70], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s,  "%s",ch.ora_fine_lav);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[90], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s,  "Cl/%s",ch.cliente);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[100], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s, "LdL/%s",ch.luogo);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[200], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s, "Ap/%s",ch.appar);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[300], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s, "Dif/%s",ch.guasto);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[380], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s, "LE/%s",ch.lav_eseguito);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[460], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s, "CATNS/%s",ch.comp_att_tariffa_non_stand);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[1460], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s, "%s",ch.comp_att_tariffa_non_stand_cent);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[1488], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s, "Flag/%s",ch.buffer_flags);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[1500], s_utf16, n * 2);

	memset(s, 0, MAX_CHAR_LAV_ESEG);
	n = g_sprintf(s, "LI/%s",ch.ore_lav);
	s_utf16 = g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	memcpy(&buffer_chiamata_utf16[1540], s_utf16, n * 2);

	free(s);

	/* ottiene dimensione file chiamate mese */
	file_chiamate_mese_size = get_file_ch_mese_size();

	/* apre in lettura e scrittura il file chiamate mese , ne legge il contenuto e lo copia in buffer.  */
	gchar buffer[file_chiamate_mese_size];
	open_rw_file_ch_mese_read(buffer);

	/* sovrascrive (nel buffer) la chiamata da modificare con quella moficata */
	memmove(&buffer[num_chiamata_selezionata * LENGHT_CHIAMATA_BYTE], &buffer_chiamata_utf16[0], LENGHT_CHIAMATA_BYTE);

	/* cancella il file chiamate mese */
	file_chiamate_mese = g_file_new_for_path(file_chiamate_mese_path);
	g_file_delete(file_chiamate_mese, NULL, NULL);

	/* ricrea il file chiamate mese vuoto */
	file_chiamate_mese_iostream = g_file_create_readwrite(file_chiamate_mese, G_FILE_CREATE_NONE, NULL, NULL);

	/* scrive nel file chiamate mese il buffer con la chiamata modificata */
	output_stream = g_io_stream_get_output_stream((GIOStream*)file_chiamate_mese_iostream);
	g_output_stream_write(output_stream, buffer, file_chiamate_mese_size, NULL, NULL);

	/* chiude l'output stream */
	g_output_stream_close(output_stream, NULL, NULL);
	
	free(buffer_chiamata_utf16);

	gtk_widget_destroy((GtkWidget*)view_lista_ch_mese);

	/* manda un segnale "clicked" (premuto) al pulsante 'lista chiamate mese' cosi' viene ricreata la schermata 
	* lista chiamate mese per fare in modo che la lista delle chiamate sia visualizzata senza la chiamata appena eliminata */
	g_signal_emit_by_name(button_lista_ch_mese, "clicked", NULL);

	return FALSE;
}


/* se viene premuto il pulsante 'annulla' della schermata modifica chiamata */
static gboolean pressed_button_annulla1(GtkWidget *widget, gpointer callback_data)
{
	gtk_widget_hide((GtkWidget*)window_modifica_chiamata);

	return FALSE;
}

/* genera lista concatenata coi clienti salvati nel file clienti.gec */
GList* crea_elenco_clienti(GList *list)
{
	gchar *str_cliente;
	glong dim_stringa;
	gunichar2 *ustr_cliente;
	int i;

	list = NULL;

	/* legge il file clienti.gec e copia il contenuto in buffer */
	file_clienti_size = get_file_clienti_size();
	gchar buffer[file_clienti_size];
	open_rw_file_clienti_read(buffer);

	i = 2;
	while(i < file_clienti_size)
	{
		ustr_cliente = (gunichar2*)(&buffer[i]);

		str_cliente = g_utf16_to_utf8(ustr_cliente, -1, NULL, &dim_stringa, NULL);

		list = g_list_append(list, (gpointer)str_cliente);

		i = i + (dim_stringa + 1) * 2;
	}

	return list;
}


/* genera lista concatenata con le apparecchiature salvate nel file appar.gec */
GList* crea_elenco_appar(GList *list)
{
	gchar *str_appar;
	glong dim_stringa;
	gunichar2 *ustr_appar;
	int i;

	/* legge il file appar.gec e copia il contenuto in buffer */
	file_appar_size = get_file_appar_size();
	gchar buffer[file_appar_size];
	open_rw_file_appar_read(buffer);

	i = 2;
	while(i < file_appar_size)
	{
		ustr_appar = (gunichar2*)(&buffer[i]);

		str_appar = g_utf16_to_utf8(ustr_appar, -1, NULL, &dim_stringa, NULL);

		list = g_list_append(list, (gpointer)str_appar);

		i = i + (dim_stringa + 1) * 2;
	}

	return list;
}


/* se viene premuto il pulsante 'lista chiamate mese' della schermata menu */
static gboolean pressed_button_lista_ch_mese(GtkWidget *widget, gpointer callback_data)
{
	/* colonne della lista */
	enum{
		NUM_CHIAMATA_COLUMN,
		DATA_COLUMN,
		CLIENTE_COLUMN,
		LUOGO_COLUMN,
		APPAR_COLUMN,
		GUASTO_COLUMN,
		FLAGS_COLUMN,
		ORE_COLUMN,
		COMPENSO_COLUMN,
		N_COLUMNS
		};

	int i,n_tot_chiamate;
	gunichar2 *ustr_data, *ustr_cliente, *ustr_luogo, *ustr_appar, *ustr_guasto, *ustr_flags, *ustr_ore, *ustr_catns, *ustr_catns_cent;
	gchar *str_data, *str_cliente,*str_luogo, *str_appar, *str_guasto, *str_flags, *str_ore, *str_catns, *str_catns_cent;
	gchar str_compenso[32];
	GtkListStore *list;
	GtkTreeIter iter;
	GtkTreeViewColumn *num_column, *data_column, *cliente_column, *luogo_column, *appar_column, *guasto_column, *flags_column, *ore_column, *compenso_column;
	GtkCellRenderer *text_renderer;

	file_chiamate_mese_size = get_file_ch_mese_size();

	/* apre in lettura e scrittura il file chiamate mese , ne legge il contenuto e lo copia in buffer.  */
	gchar buffer[file_chiamate_mese_size];
	open_rw_file_ch_mese_read(buffer);

	/* numero totale delle chiamate del mese */
	n_tot_chiamate = (file_chiamate_mese_size / LENGHT_CHIAMATA_BYTE) - 1;

	/* crea una lista a nove colonne */
	list = gtk_list_store_new(N_COLUMNS, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
					G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,  G_TYPE_STRING);

	/* carica i dati del file chiamate mese nella lista */
	for (i = 1; i <= n_tot_chiamate; i++)
	{
		/* ottiene i dati dal file chiamate mese */
		ustr_data = (gunichar2*)(&buffer[i * LENGHT_CHIAMATA_BYTE + OFFSET_DATA]);
		str_data = g_utf16_to_utf8(ustr_data, -1, NULL, NULL, NULL);

		ustr_cliente = (gunichar2*)(&buffer[i * LENGHT_CHIAMATA_BYTE + OFFSET_CLIENTE]);
		str_cliente = g_utf16_to_utf8(ustr_cliente, -1, NULL, NULL, NULL);

		ustr_luogo = (gunichar2*)(&buffer[i * LENGHT_CHIAMATA_BYTE + OFFSET_LUOGO]);
		str_luogo = g_utf16_to_utf8(ustr_luogo, -1, NULL, NULL, NULL);

		ustr_appar = (gunichar2*)(&buffer[i * LENGHT_CHIAMATA_BYTE + OFFSET_APPAR]);
		str_appar = g_utf16_to_utf8(ustr_appar, -1, NULL, NULL, NULL);

		ustr_guasto = (gunichar2*)(&buffer[i * LENGHT_CHIAMATA_BYTE + OFFSET_GUASTO]);
		str_guasto = g_utf16_to_utf8(ustr_guasto, -1, NULL, NULL, NULL);

		ustr_flags = (gunichar2*)(&buffer[i * LENGHT_CHIAMATA_BYTE + OFFSET_FLAGS]);
		str_flags = g_utf16_to_utf8(ustr_flags, -1, NULL, NULL, NULL);

		ustr_ore = (gunichar2*)(&buffer[i * LENGHT_CHIAMATA_BYTE + OFFSET_ORE]);
		str_ore = g_utf16_to_utf8(ustr_ore, -1, NULL, NULL, NULL);

		ustr_catns = (gunichar2*)(&buffer[i * LENGHT_CHIAMATA_BYTE + OFFSET_CATNS]);
		str_catns = g_utf16_to_utf8(ustr_catns, -1, NULL, NULL, NULL);
		
		ustr_catns_cent = (gunichar2*)(&buffer[i * LENGHT_CHIAMATA_BYTE + OFFSET_CATNS_CENT]);
		str_catns_cent = g_utf16_to_utf8(ustr_catns_cent, -1, NULL, NULL, NULL);

		/* formatta la stringa del compenso per attività non standard unendo la parte intera con i centesimi */ 
		sprintf(str_compenso, "%s,%s", str_catns, str_catns_cent);

		/* riempie la lista coi dati prelevati dal file chiamate mese */
		gtk_list_store_append(list, &iter);
		gtk_list_store_set(list, &iter,
					NUM_CHIAMATA_COLUMN, i,
					DATA_COLUMN, str_data,
					CLIENTE_COLUMN, str_cliente,
					LUOGO_COLUMN, str_luogo,
					APPAR_COLUMN, str_appar,
					GUASTO_COLUMN, str_guasto,
					FLAGS_COLUMN, str_flags,
					ORE_COLUMN, str_ore,
					COMPENSO_COLUMN, str_compenso,
					-1);

		/* libera le stringhe per il ciclo successivo */
		g_free(str_data);
		g_free(str_cliente);
		g_free(str_luogo);
		g_free(str_appar);
		g_free(str_guasto);
		g_free(str_flags);
		g_free(str_ore);
		g_free(str_catns);
		g_free(str_catns_cent);
	}

	/* crea tree view per la lista */
	view_lista_ch_mese = g_object_new(GTK_TYPE_TREE_VIEW,
					"model", list,
					"headers-visible", TRUE,
					NULL);

	/* crea e inizializza text renderer per le celle */
	text_renderer = gtk_cell_renderer_text_new();

	/* crea column views */
	num_column = gtk_tree_view_column_new_with_attributes("N° Ch",
								text_renderer,
								"text", NUM_CHIAMATA_COLUMN,
								NULL);

	g_object_set(num_column,
			"fixed-width", LARGHEZZA_NUM_CH_COLUMN,
			"clickable", TRUE,
			"reorderable", TRUE,
			NULL);
			
	data_column = gtk_tree_view_column_new_with_attributes("Data",
								text_renderer,
								"text", DATA_COLUMN,
								NULL);

	g_object_set(data_column,
			"fixed-width", LARGHEZZA_DATA_COLUMN,
			"clickable", TRUE,
			"reorderable", TRUE,
			NULL);
			
	cliente_column = gtk_tree_view_column_new_with_attributes("Cliente",
									text_renderer,
									"text", CLIENTE_COLUMN,
									NULL);

	g_object_set(cliente_column,
			"fixed-width", LARGHEZZA_CLIENTE_COLUMN,
			"clickable", TRUE,
			"reorderable", TRUE,
			NULL);

	luogo_column = gtk_tree_view_column_new_with_attributes("Luogo",
								text_renderer,
								"text", LUOGO_COLUMN,
								NULL);

	g_object_set(luogo_column,
			"fixed-width", LARGHEZZA_LUOGO_COLUMN,
			"clickable", TRUE,
			"reorderable", TRUE,
			NULL);
			
	appar_column = gtk_tree_view_column_new_with_attributes("Apparecchiatura",
								text_renderer,
								"text", APPAR_COLUMN,
								NULL);

	g_object_set(appar_column,
			"fixed-width", LARGHEZZA_APPAR_COLUMN,
			"clickable", TRUE,
			"reorderable", TRUE,
			NULL);
			
	guasto_column = gtk_tree_view_column_new_with_attributes("Guasto",
									text_renderer,
									"text", GUASTO_COLUMN,
									NULL);

	g_object_set(guasto_column,
			"fixed-width", LARGHEZZA_GUASTO_COLUMN,
			"clickable", TRUE,
			"reorderable", TRUE,
			NULL);
			
	flags_column = gtk_tree_view_column_new_with_attributes("Flags",
								text_renderer,
								"text", FLAGS_COLUMN,
								NULL);

	g_object_set(flags_column,
			"fixed-width", LARGHEZZA_FLAGS_COLUMN,
			"clickable", TRUE,
			"reorderable", TRUE,
			NULL);
			
	ore_column = gtk_tree_view_column_new_with_attributes("Ore",
								text_renderer,
								"text", ORE_COLUMN,
								NULL);

	g_object_set(ore_column,
			"fixed-width", LARGHEZZA_ORE_COLUMN,
			"clickable", TRUE,
			"reorderable", TRUE,
			NULL);
			
	compenso_column = gtk_tree_view_column_new_with_attributes("Compenso",
									text_renderer,
									"text", COMPENSO_COLUMN,
									NULL);

	g_object_set(compenso_column,
			"fixed-width", LARGHEZZA_COMPENSO_COLUMN,
			"clickable", TRUE,
			"reorderable", TRUE,
			NULL);
			
	/* inserisce le colonne nel view */
	gtk_tree_view_append_column(view_lista_ch_mese, num_column);
	gtk_tree_view_append_column(view_lista_ch_mese, data_column);
	gtk_tree_view_append_column(view_lista_ch_mese, cliente_column);
	gtk_tree_view_append_column(view_lista_ch_mese, luogo_column);
	gtk_tree_view_append_column(view_lista_ch_mese, appar_column);
	gtk_tree_view_append_column(view_lista_ch_mese, guasto_column);
	gtk_tree_view_append_column(view_lista_ch_mese, flags_column);
	gtk_tree_view_append_column(view_lista_ch_mese, ore_column);
	gtk_tree_view_append_column(view_lista_ch_mese, compenso_column);
	
	/* aggiunge la view della lista al box */
	gtk_container_add((GtkContainer*)scr_window_lista_ch_mese, GTK_WIDGET(view_lista_ch_mese));

	/* visualizza il tutto */
	cambia_schermata(box_menu_princ, box_lista_ch_mese);

	return FALSE;
}


/* elimina_chiamata è la funzione che viene chiamata se rispondiamo in maniera affermativa alla richiesta 
 * di eliminare la chiamata,che ci viene fatta tramite il dialog box creato dalla seguente funzione  crea_dialog_box_due_pulsanti */
void elimina_chiamata()
{
	int i, n_tot_chiamate;
	gchar buffer_file_ch_prima[file_chiamate_mese_size];
	GOutputStream *output_stream;

	/* numero totale delle chiamate del mese */
	n_tot_chiamate = (file_chiamate_mese_size / LENGHT_CHIAMATA_BYTE) - 1;

	/* apre in lettura e scrittura il file chiamate mese , ne legge il contenuto e lo copia in buffer_file_ch_prima. */
	open_rw_file_ch_mese_read(buffer_file_ch_prima);

	/* buffer che conterra' il file chiamate meno la chiamata eliminata */
	gchar buffer_file_ch_dopo[file_chiamate_mese_size - LENGHT_CHIAMATA_BYTE];

	/* copia all' inizio del buffer_file_ch_dopo la parte dall'inizio del file chiamate fino a dove * comincia la chiamata da eliminare */
	memmove(buffer_file_ch_dopo, buffer_file_ch_prima, num_chiamata_selezionata * LENGHT_CHIAMATA_BYTE);

	/* copia nel buffer_file_ch_dopo, a partire da dove dovrebbe cominciare la chiamata da eliminare, tutte le 
	* chiamate seguenti quella da eliminare. Con questi 2 spostamenti di memoria otteniamo che 
	* buffer_file_ch_dopo contiene tutte le chiamate esclusa quella eliminata, bisogna solo
	* rinumerare le chiamate */
	memmove((buffer_file_ch_dopo + num_chiamata_selezionata * LENGHT_CHIAMATA_BYTE),
				(buffer_file_ch_prima + (num_chiamata_selezionata + 1) * LENGHT_CHIAMATA_BYTE),
				((n_tot_chiamate - num_chiamata_selezionata) * LENGHT_CHIAMATA_BYTE));

	/* ciclo che rinumera le chiamate nel buffer_file_ch_dopo visto che manca quella eliminata */
	for(i = 1; i <= (n_tot_chiamate - 1); i++)
	{
		/* converte in stringa il valore di i che poi è il numero chiamata */
		gchar p_string_n_ch[MAX_CHAR_N_CHIAMATA + 1];
		g_sprintf(p_string_n_ch, "%i", i);

		/* converte la stringa col numero chiamata in unichar2 e ottiene anche la lunghezza della stringa */
		glong dim_ustring_n_ch;
		gunichar2 *p_ustring_n_ch;
		p_ustring_n_ch = g_utf8_to_utf16(p_string_n_ch, -1, NULL, &dim_ustring_n_ch, NULL);

		/* copia la stringa unichar2 con scritto il numero chiamata al giusto offset nel buffer_file_ch_dopo */
		memmove((buffer_file_ch_dopo + (i * LENGHT_CHIAMATA_BYTE) + OFFSET_N_CHIAMATA), p_ustring_n_ch, (dim_ustring_n_ch + 1) * 2);
	}

	/* cancella il file chiamate mese */
	file_chiamate_mese = g_file_new_for_path(file_chiamate_mese_path);
	g_file_delete(file_chiamate_mese, NULL, NULL);

	/* ricrea il file chiamate mese vuoto */
	file_chiamate_mese_iostream = g_file_create_readwrite(file_chiamate_mese, G_FILE_CREATE_NONE, NULL, NULL);

	/* scrive nel file chiamate mese il buffer_file_ch_dopo, che corrisponde al file chiamate mese senza la chiamata che abbiamo eliminato */
	output_stream = g_io_stream_get_output_stream((GIOStream*)file_chiamate_mese_iostream);

	g_output_stream_write(output_stream, buffer_file_ch_dopo, LENGHT_CHIAMATA_BYTE * n_tot_chiamate, NULL, NULL);

	/* chiude i vari stream */
	g_output_stream_close(output_stream, NULL, NULL);

	gtk_widget_destroy((GtkWidget*)view_lista_ch_mese);

	/* manda un segnale "clicked" (premuto) al pulsante 'lista chiamate mese' cosi' viene ricreata la schermata 
	* lista chiamate mese per fare in modo che la lista delle chiamate sia visualizzata senza la chiamata appena eliminata */
	g_signal_emit_by_name(button_lista_ch_mese, "clicked", NULL);
}	


/* se viene premuto il pulsante 'elimina chiamata' */
static gboolean pressed_button_elimina_ch(GtkWidget *widget, gpointer callback_data)
{
	GtkTreeSelection *selezione; 
	GtkTreeModel *model;
	GtkTreeIter iter;

	/* oggetto GtkTreeSelection che indica da quale GtkTreeView andiamo a ottenere la selezione*/
	selezione = gtk_tree_view_get_selection(GTK_TREE_VIEW(view_lista_ch_mese));

	/* selezione singola */
	gtk_tree_selection_set_mode (selezione, GTK_SELECTION_SINGLE);

	/* ottiene la riga scelta */
	gtk_tree_selection_get_selected(selezione, &model, &iter);

	/* ottiene il numero della chiamata selezionata dalla lista*/
	gtk_tree_model_get(model, &iter, 0, &num_chiamata_selezionata, -1);
	
	crea_dialog_box_due_pulsanti(dialog_elimina, "ATTENZIONE!", "\n   Confermi ELIMINAZIONE chiamata? \n", 
							window_principale, "Si", "No", elimina_chiamata, non_fare_nulla);

	return FALSE;
}


/* se viene premuto il pulsante 'modifica chiamata selezionata' */
static gboolean pressed_button_modifica_ch(GtkWidget *widget, gpointer callback_data)
{
	GtkTreeSelection *selezione; 
	GtkTreeModel 	*model;
	GtkTreeIter 	iter;
	gunichar2 	*ustr_gg, *ustr_mm, *ustr_aaaa, *ustr_ora_in_spost, *ustr_ora_in_lav, *ustr_ora_fine_lav, *ustr_cliente, *ustr_appar,
			*ustr_luogo, *ustr_guasto, *ustr_lav_eseguito, *ustr_ore_lav, *ustr_catns, *ustr_catns_cent;
	gunichar2 	*uc_flag;     /* puntatore a carattere unicode */

	gchar 		*str_gg,*str_mm, *str_aaaa, *str_ora_in_spost, *str_ora_in_lav, *str_ora_fine_lav, *str_cliente, *str_appar,
			*str_luogo, *str_guasto, *str_lav_eseguito, *str_ore_lav, *str_catns, *str_catns_cent;
	guint 		n_elementi;

	/* oggetto GtkTreeSelection che indica da quale GtkTreeView andiamo a ottenere la selezione*/
	selezione = gtk_tree_view_get_selection(GTK_TREE_VIEW(view_lista_ch_mese));

	/* selezione singola */
	gtk_tree_selection_set_mode (selezione, GTK_SELECTION_SINGLE);

	/* ottiene la riga scelta */
	gtk_tree_selection_get_selected(selezione, &model, &iter);

	/* ottiene il numero della chiamata selezionata dalla lista*/
	gtk_tree_model_get(model, &iter, 0, &num_chiamata_selezionata, -1);

	/* dimensione file chiamate mese */
	file_chiamate_mese_size = get_file_ch_mese_size();

	/* apre in lettura e scrittura il file chiamate mese , ne legge il contenuto e lo copia in buffer_file_ch.  */
	gchar buffer_file_ch[file_chiamate_mese_size];
	open_rw_file_ch_mese_read(buffer_file_ch);

	/* ottiene il giorno della chiamata selezionata e lo visualizza nell' entry */
	ustr_gg =(gunichar2*)(&buffer_file_ch[num_chiamata_selezionata * LENGHT_CHIAMATA_BYTE + OFFSET_DATA]);
	str_gg = g_utf16_to_utf8(ustr_gg, 2, NULL, NULL, NULL);
	gtk_entry_set_text(entry_gg1, str_gg);
	g_free(str_gg);

	/* ottiene il mese della chiamata selezionata e lo visualizza nell' entry */
	ustr_mm = (gunichar2*)(&buffer_file_ch[num_chiamata_selezionata * LENGHT_CHIAMATA_BYTE + OFFSET_DATA + 6]);
	str_mm = g_utf16_to_utf8(ustr_mm, 2, NULL, NULL, NULL);
	gtk_entry_set_text(entry_mm1, str_mm);
	g_free(str_mm);

	/* ottiene l' anno della chiamata selezionata e lo visualizza nell' entry */
	ustr_aaaa = (gunichar2*)(&buffer_file_ch[num_chiamata_selezionata * LENGHT_CHIAMATA_BYTE + OFFSET_DATA + 12]);
	str_aaaa = g_utf16_to_utf8(ustr_aaaa, 4, NULL, NULL, NULL);
	gtk_entry_set_text(entry_aaaa1, str_aaaa);
	g_free(str_aaaa);

	/* ottiene l' ora spostamento della chiamata selezionata e lo visualizza nell' entry */
	ustr_ora_in_spost = (gunichar2*)(&buffer_file_ch[num_chiamata_selezionata * LENGHT_CHIAMATA_BYTE + OFFSET_ORA_IN_SPOST]);
	str_ora_in_spost = g_utf16_to_utf8(ustr_ora_in_spost, 4, NULL, NULL, NULL);
	gtk_entry_set_text(entry_ora_in_spost1, str_ora_in_spost);
	g_free(str_ora_in_spost);

	/* ottiene l' ora inizio lavoro della chiamata selezionata e lo visualizza nell' entry */
	ustr_ora_in_lav = (gunichar2*)(&buffer_file_ch[num_chiamata_selezionata * LENGHT_CHIAMATA_BYTE + OFFSET_ORA_IN_LAV]);
	str_ora_in_lav = g_utf16_to_utf8(ustr_ora_in_lav, 4, NULL, NULL, NULL);
	gtk_entry_set_text(entry_ora_in_lav1, str_ora_in_lav);
	g_free(str_ora_in_lav);

	/* ottiene l' ora fine lavoro della chiamata selezionata e lo visualizza nell' entry */
	ustr_ora_fine_lav = (gunichar2*)(&buffer_file_ch[num_chiamata_selezionata * LENGHT_CHIAMATA_BYTE + OFFSET_ORA_FINE_LAV]);
	str_ora_fine_lav = g_utf16_to_utf8(ustr_ora_fine_lav, 4, NULL, NULL, NULL);
	gtk_entry_set_text(entry_ora_fine_lav1, str_ora_fine_lav);
	g_free(str_ora_fine_lav);

	/* crea l'elenco dei clienti per il combo apposito, leggendo i clienti salvati nel file clienti.gec . E li ritorna sotto 
	* forma di lista concatenata (GList) */
	elenco_clienti = NULL;     /* NB: il puntatore a GList DEVE sempre essere inizializzato a NULL */
	elenco_clienti = crea_elenco_clienti(elenco_clienti);

	/* conta gli elementi della lista elenco_clienti*/
	n_elementi = g_list_length(elenco_clienti);

	/* ordina alfabeticamente le stringhe della lista elenco clienti */
	elenco_clienti_ordinato = NULL;     
	elenco_clienti_ordinato = ordina_alfabeticamente(n_elementi, elenco_clienti, elenco_clienti_ordinato);

	/* rimuove tutti gli elementi dal combo_box_text_cliente1 altrimenti ogni volta che si esce e si rientra nella schermata 'registra chiamata' 
	 * al combo_box_text_cliente1 viene aggiunta un' altra lista di clienti successiva a quella gia presente creando una ripetizione degli elementi */
	gtk_combo_box_text_remove_all(combo_box_text_cliente1);

	/* assegna l'elenco clienti al combo prendendo uno alle volta gli elementi della lista */
	while(elenco_clienti_ordinato->next != 0)
	{
		gtk_combo_box_text_append_text(combo_box_text_cliente1, elenco_clienti_ordinato->data);
		elenco_clienti_ordinato = elenco_clienti_ordinato->next;
        
		/* senza questa uscirebbe dal ciclo prima di aver inserito l'ultimo elemento della lista */
		if(elenco_clienti_ordinato->next == 0) gtk_combo_box_text_append_text(combo_box_text_cliente1, elenco_clienti_ordinato->data);
	}

	/* come per il combo dei clienti ma per le apparecchiature */
	elenco_apparecchiature = NULL;
	elenco_apparecchiature = crea_elenco_appar(elenco_apparecchiature);

	/* conta gli elementi della lista elenco_apparecchiature*/
	n_elementi = g_list_length(elenco_apparecchiature);

	/* ordina alfabeticamente le stringhe della lista elenco apparecchiature */
	elenco_apparecchiature_ordinato = NULL;     
	elenco_apparecchiature_ordinato = ordina_alfabeticamente(n_elementi, elenco_apparecchiature, elenco_apparecchiature_ordinato);

	gtk_combo_box_text_remove_all(combo_box_text_appar1);

	while(elenco_apparecchiature_ordinato->next != 0)
	{
		gtk_combo_box_text_append_text(combo_box_text_appar1, elenco_apparecchiature_ordinato->data);
		elenco_apparecchiature_ordinato = elenco_apparecchiature_ordinato->next;
		if(elenco_apparecchiature_ordinato->next == 0) 
		gtk_combo_box_text_append_text(combo_box_text_appar1, elenco_apparecchiature_ordinato->data);
	}

	/* ottiene il cliente della chiamata selezionata e lo visualizza nell' entry */
	ustr_cliente = (gunichar2*)(&buffer_file_ch[num_chiamata_selezionata * LENGHT_CHIAMATA_BYTE + OFFSET_CLIENTE]);
	str_cliente = g_utf16_to_utf8(ustr_cliente, -1, NULL, NULL, NULL);
	gtk_entry_set_text(entry_cliente1, str_cliente);
	g_free(str_cliente);

	/* ottiene l' apparecchiatura della chiamata selezionata e lo visualizza nell' entry */
	ustr_appar = (gunichar2*)(&buffer_file_ch[num_chiamata_selezionata * LENGHT_CHIAMATA_BYTE + OFFSET_APPAR]);
	str_appar = g_utf16_to_utf8(ustr_appar, -1, NULL, NULL, NULL);
	gtk_entry_set_text(entry_appar1, str_appar);
	g_free(str_appar);

	/* ottiene il luogo della chiamata selezionata e lo visualizza nell' entry */
	ustr_luogo = (gunichar2*)(&buffer_file_ch[num_chiamata_selezionata * LENGHT_CHIAMATA_BYTE + OFFSET_LUOGO]);
	str_luogo = g_utf16_to_utf8(ustr_luogo, -1, NULL, NULL, NULL);
	gtk_entry_set_text(entry_luogo1, str_luogo);
	g_free(str_luogo);

	/* ottiene il guasto della chiamata selezionata e lo visualizza nell' entry */
	ustr_guasto = (gunichar2*)(&buffer_file_ch[num_chiamata_selezionata * LENGHT_CHIAMATA_BYTE + OFFSET_GUASTO]);
	str_guasto = g_utf16_to_utf8(ustr_guasto, -1, NULL, NULL, NULL);
	gtk_entry_set_text(entry_guasto1, str_guasto);
	g_free(str_guasto);

	/* ottiene il lavoro eseguito della chiamata selezionata e lo visualizza nell' entry */
	ustr_lav_eseguito = (gunichar2*)(&buffer_file_ch[num_chiamata_selezionata * LENGHT_CHIAMATA_BYTE + OFFSET_LAV_ESEGUITO]);
	str_lav_eseguito = g_utf16_to_utf8(ustr_lav_eseguito, -1, NULL, NULL, NULL);
	gtk_entry_set_text(entry_lav_eseguito1, str_lav_eseguito);
	g_free(str_lav_eseguito);

	/* legge i flag della chiamata da modificare e setta quelli da settare e resetta quelli da non settare */
	/* punta il primo flag */
	uc_flag = (gunichar2*)(&buffer_file_ch[num_chiamata_selezionata * LENGHT_CHIAMATA_BYTE + OFFSET_FLAGS]);

	/* se il primo flag nella chiamata salvata è settato lo setta, altrimenti lo resetta */
	if(*uc_flag == '1')
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_cht1), 1);
	}
	else
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_cht1), 0);
	}

	uc_flag++;      /*passa al flag successivo */
	if(*uc_flag == '1')
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_chtl1), 1);
	}
	else
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_chtl1), 0);
	}

	uc_flag++;
	if(*uc_flag == '1')
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_uvt1), 1);
	}
	else
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_uvt1), 0);
	}

	uc_flag++;
	if(*uc_flag == '1')
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_chp1), 1);
	}
	else
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_chp1), 0);
	}

	uc_flag++;
	if(*uc_flag == '1')
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_dis1), 1);
	}
	else
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_dis1), 0);
	}

	uc_flag++;
	if(*uc_flag == '1')
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_uvp1), 1);
	}
	else
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_uvp1), 0);
	}

	uc_flag++;
	if(*uc_flag == '1')
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_za1), 1);
	}
	else
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_za1), 0);
	}

	uc_flag++;
	if(*uc_flag == '1')
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_zb1), 1);
	}
	else
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_zb1), 0);
	}

	uc_flag++;
	if(*uc_flag == '1')
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_zc1), 1);
	}
	else
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_zc1), 0);
	}

	/* ottiene le ore di lavoro della chiamata selezionata e le visualizza nell' entry */
	ustr_ore_lav = (gunichar2*)(&buffer_file_ch[num_chiamata_selezionata * LENGHT_CHIAMATA_BYTE + OFFSET_ORE]);
	str_ore_lav = g_utf16_to_utf8(ustr_ore_lav, 1, NULL, NULL, NULL);
	gtk_entry_set_text(entry_ore_lav1, str_ore_lav);
	g_free(str_ore_lav);

	/* ottiene il compenso per attività a tariffa non standard e lo visualizza nell' entry */
	ustr_catns = (gunichar2*)(&buffer_file_ch[num_chiamata_selezionata * LENGHT_CHIAMATA_BYTE + OFFSET_CATNS]);
	str_catns = g_utf16_to_utf8(ustr_catns, -1, NULL, NULL, NULL);
	gtk_entry_set_text(entry_compenso1, str_catns);
	g_free(str_catns);
    
	/* ottiene i centesimi del compenso per attività a tariffa non standard e lo visualizza nell' entry */
	ustr_catns_cent = (gunichar2*)(&buffer_file_ch[num_chiamata_selezionata * LENGHT_CHIAMATA_BYTE + OFFSET_CATNS_CENT]);
	str_catns_cent = g_utf16_to_utf8(ustr_catns_cent, -1, NULL, NULL, NULL);
	gtk_entry_set_text(entry_compenso_cent1, str_catns_cent);
	g_free(str_catns_cent);

	/* se è una chiamata a tariffa non standard setta il flag */
	if((ustr_catns[0] != 0) || (ustr_catns_cent[0] != 0)) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_compenso1), 1);

	gtk_widget_show_all((GtkWidget*)window_modifica_chiamata);

	return FALSE;
}


/* se viene premuto il pulsante 'database clienti e app' della schermata menu */
static gboolean pressed_button_database(GtkWidget *widget, gpointer callback_data)
{
	GtkListStore *lista_clienti, *lista_appar;
	guint n_elementi;
	GtkTreeIter iter;
	GtkCellRenderer *text_renderer;
	GtkTreeViewColumn *cliente_column, *appar_column;

	elenco_clienti = NULL;
	elenco_clienti_ordinato = NULL; 
	elenco_apparecchiature = NULL;
	elenco_apparecchiature_ordinato = NULL;   

	/* crea contenitore per la lista che visualizza l'elenco clienti , il primo parametro a 1 indica il numero di colonne presenti nel contenitore 
	 * per le liste */
	lista_clienti = gtk_list_store_new(1, G_TYPE_STRING);

	/* ottiene l'elenco clienti sotto forma di lista concatenata ,in cui ogni elemento è un puntatore a una stringa cliente , dal file */
	elenco_clienti = crea_elenco_clienti(elenco_clienti);

	/* conta gli elementi della lista elenco_clienti*/
	n_elementi = g_list_length(elenco_clienti);

	/* ordina alfabeticamente le stringhe della lista elenco clienti */
	elenco_clienti_ordinato = ordina_alfabeticamente(n_elementi, elenco_clienti, elenco_clienti_ordinato);

	/* finchè ci sono clienti inseriscili come nuove righe nel contenitore lista */
	while(elenco_clienti_ordinato)
	{
		/* aggiunge una riga al contenitore della lista */
		gtk_list_store_append(lista_clienti, &iter);

		/* setta la riga con la stringa del cliente corrente */
		gtk_list_store_set(lista_clienti, &iter, COLONNA_CLIENTE,  elenco_clienti_ordinato->data, -1);

		/* punta prossimo cliente */
		elenco_clienti_ordinato = elenco_clienti_ordinato->next;
	}

	/* crea widget che serve per visualizzare sia le liste(list) che gli alberi(tree) */
	view_clienti = g_object_new(GTK_TYPE_TREE_VIEW,
							"model", lista_clienti,
							"rules-hint", TRUE, // righe alternate bianche e grigie
							NULL); 

	text_renderer = gtk_cell_renderer_text_new();

	/* imposta titolo ("Cliente") della colonna (unica) della lista clienti e la proprieta' text del text_renderer a 0 (default), che dice 
	 * che le celle della colonna contengono stringhe di testo */
	cliente_column = gtk_tree_view_column_new_with_attributes("Cliente", text_renderer, "text", 0,NULL);

	g_object_set(cliente_column, "clickable", TRUE, NULL);

	gtk_tree_view_append_column(view_clienti, cliente_column);

	/* ripete lo stesso procedimento per visualizzare la lista apparecchiature*/
	lista_appar = gtk_list_store_new(1, G_TYPE_STRING);

	elenco_apparecchiature = crea_elenco_appar(elenco_apparecchiature);

	/* conta gli elementi della lista elenco_apparecchiature*/
	n_elementi = g_list_length(elenco_apparecchiature);
    
	/* ordina alfabeticamente le stringhe della lista elenco apparecchiature */
	elenco_apparecchiature_ordinato = ordina_alfabeticamente(n_elementi, elenco_apparecchiature, elenco_apparecchiature_ordinato);

	while(elenco_apparecchiature_ordinato)
	{
		gtk_list_store_append(lista_appar, &iter);

		gtk_list_store_set(lista_appar, &iter, COLONNA_APPAR, elenco_apparecchiature_ordinato->data, -1);

		elenco_apparecchiature_ordinato = elenco_apparecchiature_ordinato->next;
	}

	view_appar = g_object_new(GTK_TYPE_TREE_VIEW, "model", lista_appar, "rules-hint", TRUE, NULL);

	text_renderer = gtk_cell_renderer_text_new();

	appar_column = gtk_tree_view_column_new_with_attributes("Apparecchiatura", text_renderer, "text", 0, NULL);

	g_object_set(appar_column, "clickable", TRUE, NULL);

	gtk_tree_view_append_column(view_appar, appar_column);

	gtk_container_add(GTK_CONTAINER(scr_window_clienti), GTK_WIDGET(view_clienti));

	gtk_container_add(GTK_CONTAINER(scr_window_appar), GTK_WIDGET(view_appar));
	
	gtk_widget_show((GtkWidget*)view_clienti);	// IMPORTANTE altrimenti non visualizza le liste
	
	gtk_widget_show((GtkWidget*)view_appar);

	cambia_schermata(box_menu_princ, box_clienti_appar);

	return FALSE;
}


/* se viene premuto il pulsante 'modifica compensi attività' della schermata menu */
static gboolean pressed_button_mod_comp_att(GtkWidget *widget, gpointer callback_data)
{
	/* ottiene dimensione file chiamate mese */
	file_chiamate_mese_size = get_file_ch_mese_size();

	/* apre in lettura e scrittura il file chiamate mese , ne legge il contenuto e lo copia in buffer.  */
	gchar buffer[file_chiamate_mese_size];
	open_rw_file_ch_mese_read(buffer);

	/* dal buffer che contiene il file chiamate mese rileva i vari compensi per le attività, partendo dall'offet da 
	* dove sono salvati, e li visualizza negli entry corrispondenti della schermata modifica compensi attività */
	gtk_entry_set_text(entry_comp_ch_t, &buffer[OFFSET_COMPENSI]);
	gtk_entry_set_text(entry_comp_ch_t_dec, &buffer[OFFSET_COMPENSI + 3]);
	gtk_entry_set_text(entry_comp_ch_tl, &buffer[OFFSET_COMPENSI + 5]);
	gtk_entry_set_text(entry_comp_ch_tl_dec, &buffer[OFFSET_COMPENSI + 8]);
	gtk_entry_set_text(entry_comp_uvt, &buffer[OFFSET_COMPENSI + 10]);
	gtk_entry_set_text(entry_comp_uvt_dec, &buffer[OFFSET_COMPENSI + 13]);
	gtk_entry_set_text(entry_comp_ch_p, &buffer[OFFSET_COMPENSI + 15]);
	gtk_entry_set_text(entry_comp_ch_p_dec, &buffer[OFFSET_COMPENSI + 18]);
	gtk_entry_set_text(entry_comp_dis_p, &buffer[OFFSET_COMPENSI + 20]);
	gtk_entry_set_text(entry_comp_dis_p_dec, &buffer[OFFSET_COMPENSI + 23]);
	gtk_entry_set_text(entry_comp_uvp, &buffer[OFFSET_COMPENSI + 25]);
	gtk_entry_set_text(entry_comp_uvp_dec, &buffer[OFFSET_COMPENSI + 28]);
	gtk_entry_set_text(entry_comp_zb, &buffer[OFFSET_COMPENSI + 30]);
	gtk_entry_set_text(entry_comp_zb_dec, &buffer[OFFSET_COMPENSI + 33]);
	gtk_entry_set_text(entry_comp_zc, &buffer[OFFSET_COMPENSI + 35]);
	gtk_entry_set_text(entry_comp_zc_dec, &buffer[OFFSET_COMPENSI + 38]);
	gtk_entry_set_text(entry_comp_lav_ore, &buffer[OFFSET_COMPENSI + 40]);
	gtk_entry_set_text(entry_comp_lav_ore_dec, &buffer[OFFSET_COMPENSI + 43]);
	gtk_entry_set_text(entry_comp_iva, &buffer[OFFSET_COMPENSI + 45]);

	cambia_schermata(box_menu_princ, box_mod_comp_att);

	return FALSE;
}

/* se viene premuto il pulsante 'aggiungi apparecchiatura' della schermata database */
static gboolean pressed_button_agg_appar(GtkWidget *widget, gpointer callback_data)
{
	gchar str_nuova_appar[MAX_CHAR_APPAR];
	GString *gstr_nuova_appar;
	gunichar2 *uc_str_nuova_appar;
	GOutputStream *output_stream;
	goffset offset;

	/* crea e visualizza il dialog che chiede di immetere la nuova apparecchiatura da aggiungere e ritorna nel parametro str_nuova_appar, 
	 * il testo inserito nel dialog */
	crea_dialog_box_due_pulsanti_con_entry(dialog_agg_appar,
									"Nuova apparecchiatura", 
									"\n       Inserire nuova apparecchiatura      \n", 
									window_principale, 
									entry_agg_appar, MAX_CHAR_APPAR,
									"OK", "Cancel", str_nuova_appar);

	if(cancel) goto FINE;

	/* converte in una gstring,la stringa della nuova apparecchiatura */                            
	gstr_nuova_appar = g_string_new(str_nuova_appar);

	/* converte la stringa dell'apparecchiatura da aggiungere in una strnga unicode */
	uc_str_nuova_appar = g_utf8_to_utf16(str_nuova_appar, -1, NULL, NULL, NULL); 

	/* ottiene l'outputstream per scrivere nel file appar.gec */
	file_appar_iostream = g_file_open_readwrite(file_appar, NULL, NULL);
	output_stream = g_io_stream_get_output_stream((GIOStream*)file_appar_iostream);

	/* ottiene la posizione nello stream che in questo caso ,visto che non abbiamo ancora fatto nessuna operazione con esso , sarà zero */
	offset = g_seekable_tell((GSeekable *)output_stream);

	/* sposta la posizione alla fine dello stream in modo da poter scrivere nel file partendo dalla fine di esso*/
	g_seekable_seek((GSeekable *)output_stream, offset, G_SEEK_END, NULL, NULL);

	/* aggiunge la stringa della nuova apparecchiatura al file appar.gec scrivendola in coda ad esso */
	g_output_stream_write(output_stream, uc_str_nuova_appar, (gstr_nuova_appar->len + 1) * 2, NULL, NULL);

	/* chiude i vari stream */
	g_output_stream_close(output_stream, NULL, NULL);
	g_io_stream_close((GIOStream*)file_appar_iostream, NULL, NULL);

	/* distrugge il wiget che visualizza la lista apparecchiature */
	gtk_widget_destroy((GtkWidget*)view_appar);

	/* emette un segnale di click sul pulsante 'database clienti e appar' in modo che la lista apparecchiature venga ricreata con l'apparecchiatura 
	 * aggiunta e venga nuovamente visualizzata */
	g_signal_emit_by_name(button_database, "clicked", NULL);

	FINE:;
	cancel = FALSE;

	return FALSE;                                
}

/* se viene premuto il pulsante 'aggiungi cliente' della schermata database.
 * Per la descrizione dettagliata della funzione, fare riferimento alla pressed_button_agg_appar */
static gboolean pressed_button_agg_cliente(GtkWidget *widget, gpointer callback_data)
{
	gchar str_nuovo_cliente[MAX_CHAR_CLIENTE];
	GString *gstr_nuovo_cliente;
	gunichar2 *uc_str_nuovo_cliente;
	GOutputStream *output_stream;
	goffset offset;

	crea_dialog_box_due_pulsanti_con_entry(dialog_agg_cliente,
									"Nuovo Cliente", 
									"\n	Inserire nuovo cliente	\n", 
									window_principale, 
									entry_agg_cliente, MAX_CHAR_CLIENTE,
									"OK", "Cancel", str_nuovo_cliente);

	if(cancel) goto FINE;

	gstr_nuovo_cliente = g_string_new(str_nuovo_cliente);

	uc_str_nuovo_cliente = g_utf8_to_utf16(str_nuovo_cliente, -1, NULL, NULL, NULL);
 
	file_clienti_iostream = g_file_open_readwrite(file_clienti, NULL, NULL);

	output_stream = g_io_stream_get_output_stream((GIOStream*)file_clienti_iostream);

	offset = g_seekable_tell((GSeekable *)output_stream);
	g_seekable_seek((GSeekable *)output_stream, offset, G_SEEK_END, NULL, NULL);

	g_output_stream_write(output_stream, uc_str_nuovo_cliente, (gstr_nuovo_cliente->len + 1) * 2, NULL, NULL);

	g_output_stream_close(output_stream, NULL, NULL);
	g_io_stream_close((GIOStream*)file_clienti_iostream, NULL, NULL);

	gtk_widget_destroy((GtkWidget*)view_clienti);

	g_signal_emit_by_name(button_database, "clicked", NULL);

	FINE:;
	cancel = FALSE;

	return FALSE;
}


/* se viene premuto il pulsante 'elimina apparecchiatura' della schermata database */
static gboolean pressed_button_elimina_appar(GtkWidget *widget, gpointer callback_data)
{
	GtkTreeSelection *selezione; 
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *str_appar;
	GString *gstr_appar;
	gchar *stringa_letta;
	size_t num_of_bytes, num_bytes;
	GFileOutputStream *output_stream;
	int i;

	/* oggetto GtkTreeSelection che indica da quale GtkTreeView andiamo a ottenere la selezione*/
	selezione = gtk_tree_view_get_selection(GTK_TREE_VIEW(view_appar));

	/* selezione singola */
	gtk_tree_selection_set_mode (selezione, GTK_SELECTION_SINGLE);

	/* ottiene la riga scelta */
	gtk_tree_selection_get_selected(selezione, &model, &iter);

	/* ottiene la stringa della riga scelta e la converte in gstring*/
	gtk_tree_model_get(model, &iter, 0, &str_appar, -1);
	gstr_appar = g_string_new(str_appar);

	/* apre il file appar.gec e ne copia il contenuto in un buffer */
	file_appar_size = get_file_appar_size();
	gchar buffer[file_appar_size];
	open_rw_file_appar_read(buffer);

	/* esamina il buffer nel quale abbiamo copiato il contenuto del file appar.gec un byte alla volta finchè non trova una stringa (che inizia dal byte 
	 * corrente in esame) uguale alla stringa che dobbiamo eliminare. Una volta trovata la stringa esce dal ciclo, e la variabile 'i' potrà essere 
	 * usata come offset per sapere la posizione della stringa */
	for(i = 0; i < file_appar_size; i++)
	{
		stringa_letta = g_utf16_to_utf8((gunichar2*)&buffer[i], -1, NULL, NULL, NULL);
		if(strcmp(stringa_letta, gstr_appar->str) == 0) break;
	}

	/* crea un secondo buffer (buffer2) nel quale copia la parte di buffer successiva all' apparecchiatura da eliminare*/
	gchar buffer2[file_appar_size];
	num_of_bytes = file_appar_size - i - ((gstr_appar->len + 1) * 2);
	memcpy(buffer2, &buffer[i + (gstr_appar->len + 1) * 2], num_of_bytes);

	/* copia il  buffer2 in in buffer a partire da dove inizia l'apparecchiatura  da eliminare, cosi la sovrascrive e non c'è più  */
	memcpy(&buffer[i], buffer2, num_of_bytes);

	/* elimina il file appar.gec poi lo ricrea col contenuto di buffer che non avrà l'apparecchiatura eliminata. 
	 * NB: bisogna farlo perchè altrimenti anche se copiamo buffer all'interno dello stream del file, alla fine del file rimarrà della sporcizia 
	 * di un numero di byte uguale all'apperecchiatura eliminata*/
	g_file_delete(file_appar, NULL, NULL);

	/* gfile per il nuovo file appar.gec */
	file_appar = g_file_new_for_path(file_appar_path);

	/* ricrea il file appar.gec e ottiene lo stream per scriverne il contenuto*/
	output_stream = g_file_create(file_appar, G_FILE_CREATE_NONE, NULL, NULL);

	/* lunghezza dell'apparecchiatura eliminata in byte compreso lo il carattere nullo finale data dalla lunghezza 
	* della gstring + carattere nullo finale il tutto moltiplicato per 2 perchè ricordo che il file appar.gec è codificato in utf16*/
	num_bytes = (gstr_appar->len + 1) * 2;

	/* scrive buffer nel nuovo file appar.gec . ovviamente scrive un numero di bytes pari alla dimensione del 
	* vecchio file appar.gec meno la lunghezza dell'apparecchiatura eliminata */
	g_output_stream_write((GOutputStream*)output_stream, buffer, file_appar_size -  num_bytes, NULL, NULL);

	g_output_stream_close((GOutputStream*)output_stream, NULL, NULL);

	/* cosi viene aggiornata la lista delle apparecchiature togliendo quella eliminata */
	gtk_widget_destroy((GtkWidget*)view_appar);
	g_signal_emit_by_name(button_database, "clicked", NULL);

	return FALSE;
}


/* se viene premuto il pulsante 'elimina cliente' della schermata database */
static gboolean pressed_button_elimina_cliente(GtkWidget *widget, gpointer callback_data)
{
	GtkTreeSelection *selezione;
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *str_cliente;
	GString *gstr_cliente;
	gchar *stringa_letta;
	size_t num_of_bytes, num_bytes;
	GFileOutputStream *output_stream;
	int i;

	selezione = gtk_tree_view_get_selection(GTK_TREE_VIEW(view_clienti));

	gtk_tree_selection_set_mode (selezione, GTK_SELECTION_SINGLE);

	gtk_tree_selection_get_selected(selezione, &model, &iter);

	gtk_tree_model_get(model, &iter, 0, &str_cliente, -1);

	gstr_cliente = g_string_new(str_cliente);

	file_clienti_size = get_file_clienti_size();

	gchar buffer[file_clienti_size];
	open_rw_file_clienti_read(buffer);

	for(i = 0; i < file_clienti_size; i++)
	{
		stringa_letta = g_utf16_to_utf8((gunichar2*)&buffer[i], -1, NULL, NULL, NULL);
		if(strcmp(stringa_letta, gstr_cliente->str) == 0) break;
	}

	gchar buffer2[file_clienti_size];
	num_of_bytes = file_clienti_size - i - ((gstr_cliente->len + 1) * 2);
	memcpy(buffer2, &buffer[i + (gstr_cliente->len + 1) * 2], num_of_bytes);

	memcpy(&buffer[i], buffer2, num_of_bytes);

	g_file_delete(file_clienti, NULL, NULL);

	file_clienti = g_file_new_for_path(file_clienti_path);

	output_stream = g_file_create(file_clienti, G_FILE_CREATE_NONE, NULL, NULL);

	num_bytes = (gstr_cliente->len + 1) * 2;

	g_output_stream_write((GOutputStream*)output_stream, buffer, file_clienti_size -  num_bytes, NULL, NULL);

	g_output_stream_close((GOutputStream*)output_stream, NULL, NULL);

	gtk_widget_destroy((GtkWidget*)view_clienti);

	g_signal_emit_by_name(button_database, "clicked", NULL);

	return FALSE;
}


/* se viene premuto il pulsante 'carica mese' della schermata menu visualizza il dialog di apertura file */
static gboolean pressed_button_carica_mese(GtkWidget *widget, gpointer callback_data)
{
	/* crea il dialog di apertura file */
	GtkWidget *dialog;
	dialog = gtk_file_chooser_dialog_new("Carica mese",
									window_principale,
									GTK_FILE_CHOOSER_ACTION_OPEN,
									("_Cancel"), GTK_RESPONSE_CANCEL,
									("_Open"), GTK_RESPONSE_ACCEPT,
									NULL);

	/* fa in modo che il dialog appena aperto mostri subito il contenuto della cartella database dopo sono presenti i file chiamate */
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), database_dir);;

	/* visualizza ed esegue il dialog finchè non lo chiudiamo o premiamo apri o annulla. Se premiamo apri ottiene in filename il nome 
	 * (completo di percorso)
	 * del file chiamate che vogliamo aprire e dopo lo copiamo in file_chiamate_mese_path, cosi d' ora in poi lavoriamo su quel file */
	if(gtk_dialog_run(GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		char *filename;
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		g_sprintf(file_chiamate_mese_path, "%s", filename);
		g_free(filename);
	}

	mostra_nome_file_ch_mese_caricato();

	gtk_widget_destroy(dialog);

	return FALSE;
}


/* se viene premuto il 'button_cambio_pw' */
static gboolean pressed_button_cambio_pw(GtkWidget *widget, gpointer callback_data)
{
	cambia_schermata(box_menu_princ, box_cambio_pw);
        return FALSE;
}


/* se viene premuto il 'button_ok'  nella schermata Cambio Password */
static gboolean pressed_button_ok(GtkWidget *widget, gpointer callback_data)
{
	GInputStream *input_stream;
	GOutputStream *output_stream;
	const gchar *str_vecchia_pw, *str_nuova_pw, *str_conf_nuova_pw;
	void *offset;

	str_vecchia_pw = gtk_entry_get_text(GTK_ENTRY(entry_vecchia_pw));
	str_nuova_pw = gtk_entry_get_text(GTK_ENTRY(entry_nuova_pw));
	str_conf_nuova_pw = gtk_entry_get_text(GTK_ENTRY(entry_conf_nuova_pw));

	/* se la vecchia password è sbagliata visulazza il dialog col messaggio d'errore ed esce */
	if(strcmp(str_vecchia_pw, pw_corrente->str) != 0)
	{
		crea_dialog_box_un_pulsante(dialog_vecchia_pw_err, "ATTENZIONE!", "\n       Vecchia pw ERRATA!       \n",  window_principale, "OK");
	return FALSE;
	}

	/* se la nuova password e la conferma non corrispondono visulazza il dialog col messaggio d'errore ed esce */
	if(strcmp(str_nuova_pw, str_conf_nuova_pw) != 0)
	{
		crea_dialog_box_un_pulsante(dialog_nuova_pw_err, 
			"ATTENZIONE!", "\n       La nuova password e la conferma NON corrispondono!       \n", window_principale, "OK");
		return FALSE;
	}

	/* una volta verificate le password inserite possiamo procedere con la sostituzione della vecchia pw con la nuova */

	/* converte la stringa della nuova password in unichar2(unicode) e ottiene anche la lunghezza della stringa */
	glong dim_ustring_nuova_pw;
	gunichar2 *ustring_nuova_pw;
	ustring_nuova_pw = g_utf8_to_utf16(str_nuova_pw, -1, NULL, &dim_ustring_nuova_pw, NULL);

	/* dimensione file system.gec*/
	file_config_size = get_file_config_size();

	/* copia il contenuto di system.gec in buffer */
	file_config = g_file_new_for_path(file_config_path);
	file_config_iostream = g_file_open_readwrite(file_config, NULL, NULL);
	input_stream = g_io_stream_get_input_stream((GIOStream*)file_config_iostream);
	gchar buffer[file_config_size];
	g_input_stream_read(input_stream, buffer, file_config_size, NULL, NULL);
	g_input_stream_close(input_stream, NULL, NULL);
	g_io_stream_close((GIOStream*)file_config_iostream, NULL, NULL);

	/* ottiene l'offset dove è scritta la password*/
	offset = &buffer[326];

	/* in buffer sovrascrive la vecchia pw con la nuova */
	memcpy(offset, ustring_nuova_pw, MAX_CHAR_PW * 2);

	/* sostituisce il contenuto di system.gec con buffer che contiene la pw modificata */
	file_config_iostream = g_file_open_readwrite(file_config, NULL, NULL);
	output_stream = g_io_stream_get_output_stream((GIOStream*)file_config_iostream);
	gssize bytes_scritti = g_output_stream_write(output_stream, buffer, file_config_size, NULL, NULL);

	/* controlla se c'è stato errore nella scrittura della nuova pw*/
	if(bytes_scritti != file_config_size)
	{
		crea_dialog_box_un_pulsante(dialog_scrittura_nuova_pw_err, 
			"ATTENZIONE!", "\n Non riesco a slavare la nuova password nell' apposito file \n", window_principale, "OK");
		return FALSE;
	}

	/* chiude i vari stream */
	g_output_stream_close(output_stream, NULL, NULL);
	g_io_stream_close((GIOStream*)file_config_iostream, NULL, NULL);

	/* dialog di conferma del cambio pw avvenuto */
	crea_dialog_box_un_pulsante(dialog_conferma_cambio_pw, "OPERAZIONE ESEGUITA", "\n Password modificata con successo! \n", window_principale, "OK");

	return FALSE;
}


/* se viene premuto il pulsante 'indietro' */
static gboolean pressed_button_indietro(GtkWidget *widget, gpointer callback_data)
{
	if(schermata_corrente == SCHERMATA_REG_CH)
	{
		g_list_free(elenco_clienti);
		g_list_free(elenco_clienti_ordinato);
		g_list_free(elenco_apparecchiature);
		g_list_free(elenco_apparecchiature_ordinato);
		
		cambia_schermata(box_reg_chiamata, box_menu_princ);
	}
	if(schermata_corrente == SCHERMATA_RESOCONTO) cambia_schermata(box_resoconto_mese, box_menu_princ);
	if(schermata_corrente == SCHERMATA_STAT_ANN) cambia_schermata(box_statistiche_annuali, box_menu_princ);
	if(schermata_corrente == SCHERMATA_MOD_COMP_ATT) cambia_schermata(box_mod_comp_att, box_menu_princ);
	if(schermata_corrente == SCHERMATA_CAMBIO_PW) cambia_schermata(box_cambio_pw, box_menu_princ);
	if(schermata_corrente == SCHERMATA_DATABASE_CL_APP) cambia_schermata(box_clienti_appar , box_menu_princ);
	if(schermata_corrente == SCHERMATA_LISTA_CH_MESE )
	{
		gtk_widget_hide((GtkWidget*)button_elimina_ch);
		gtk_widget_hide((GtkWidget*)button_modifica_ch);
		gtk_widget_destroy((GtkWidget*)view_lista_ch_mese);
		cambia_schermata(box_lista_ch_mese , box_menu_princ);
	}
	
	return FALSE;
}


/* se viene premuto il pulsante 'copia dati' della schermata resoconto mese */
static gboolean pressed_button_copia_dati(GtkWidget *widget, gpointer callback_data)
{
	/* buffer di caratteri che conterrà tutte le attività del mese sotto forma di testo con i vari valori separati da un carattere di tabulazione e
	* con un carattere a capo per dividere ogni giorno. 10 colonne * 31 giorni * 9 massimo di caratteri per ogni dato compreso il tab */
	gchar buffer_dati[10 * 31 * 9] = {0};
	gchar dato[8] = {0};
	attivita    att[32] = {0};
	gint result;
	GtkWidget *content_area;
	GtkTextBuffer *TextBuffer;
	GtkTextView *dialog_text;
	int i;

	/* conta le attivita svolte nel mese e ne salva i totali nell'array di strutture att. Il valore dell'indice dell'array corrisponde al giorno del mese */
	conta_attivita(&att[0]);
	
	/* ciclo che riempie il buffer_dat i*/
	for(i = 1; i <= 31; i++)
	{
		/* se se il valore dell' attivita' è diverso da zero lo copia nel buffer seguito da un 'tab' , invece se è zero mette solo il 'tab' */
		if(att[i].catns != 0) g_sprintf(dato, "%.2f\t", att[i].catns);
		else g_sprintf(dato, "\t");
		strcat(buffer_dati, dato);
		
		if(att[i].ch_t != 0) g_sprintf(dato, "%i\t", att[i].ch_t);
		else g_sprintf(dato, "\t");
		strcat(buffer_dati, dato);

		if(att[i].ch_tl != 0) g_sprintf(dato, "%i\t", att[i].ch_tl);
		else g_sprintf(dato, "\t");
		strcat(buffer_dati, dato);

		if(att[i].ch_p != 0) g_sprintf(dato, "%i\t", att[i].ch_p);
		else g_sprintf(dato, "\t");
		strcat(buffer_dati, dato);

		if(att[i].dis_p != 0) g_sprintf(dato, "%i\t", att[i].dis_p);
		else g_sprintf(dato, "\t");
		strcat(buffer_dati, dato);

		if(att[i].uvt != 0) g_sprintf(dato, "%i\t", att[i].uvt);
		else g_sprintf(dato, "\t");
		strcat(buffer_dati, dato);

		if(att[i].uvp != 0)g_sprintf(dato, "%i\t", att[i].uvp);
		else g_sprintf(dato, "\t");
		strcat(buffer_dati, dato);

		if(att[i].zb != 0) g_sprintf(dato, "%i\t", att[i].zb);
		else g_sprintf(dato, "\t");
		strcat(buffer_dati, dato);

		if(att[i].zc != 0) g_sprintf(dato, "%i\t", att[i].zc);
		else g_sprintf(dato, "\t");
		strcat(buffer_dati, dato);

		if(att[i].lav_ore != 0) g_sprintf(dato, "%.3f\t\n", (float)(att[i].lav_ore * 0.125));
		else g_sprintf(dato, "\t\n");
		strcat(buffer_dati, dato);
	}

	/* crea il TextBuffer */
	TextBuffer = gtk_text_buffer_new(NULL);

	/* copia buffer_dati nel TextBuffer */
	gtk_text_buffer_set_text(TextBuffer, buffer_dati, -1);

	/* crea un textview (il cui testo è buffer_dati) ,da inserire nel dialog box che creiamo qui di seguito */
	dialog_text = g_object_new(GTK_TYPE_TEXT_VIEW,
							"cursor-visible", FALSE,
							"editable", FALSE,
							"buffer", TextBuffer,
							NULL);

	/* imposta il dialog per la copia dati */
	dialog_copia_dati = GTK_DIALOG(gtk_dialog_new_with_buttons(
								"COPIA_DATI",
								window_principale,
								GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
								"Copia negli appunti", GTK_RESPONSE_YES,
								NULL));

	/* inserisce il textview nel dialog */
	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog_copia_dati));
	gtk_container_add(GTK_CONTAINER(content_area), GTK_WIDGET(dialog_text));

	gtk_widget_show_all((GtkWidget*)dialog_copia_dati);

	result = gtk_dialog_run((GtkDialog*)dialog_copia_dati);

	/* se viene premuto il pulsante di chiusura del dialog esso ritorna un GTK_RESPONSE_DELETE_EVENT, ma non viene chiuso, 
	* quindi lo distruggiamo noi e usciamo da questa funzione */
	if(result == GTK_RESPONSE_DELETE_EVENT)
	{
		gtk_widget_destroy((GtkWidget*)dialog_copia_dati);
		return FALSE;
	}

	/* se viene premuto pulsante 'Copia negli appunti' ... */
	if(result == GTK_RESPONSE_YES)
	{
		/* ...seleziona il contenuto del dialog...*/
		g_signal_emit_by_name(dialog_text, "select-all", TRUE);

		/* ...lo copia negli appunti, NB: EQUIVALE AL 'CONTROL - C'*/
		g_signal_emit_by_name(dialog_text, "copy-clipboard");
	}

	return FALSE;
}


/* se viene premuto il pulsante 'Esci' */
static gboolean pressed_button_esci(GtkWidget *widget, gpointer callback_data)
{
	/* se siamo nella schermata ripristino o in quella di accesso, esce senza chiedere se vogliamo aggiornare il database locale */
	if((schermata_corrente == SCHERMATA_ACCESSO) ||  (schermata_corrente == SCHERMATA_RIPRISTINO))
	{
		uscita_forzata();
	}

	uscita_normale();

	return FALSE;
}


/* se viene premuto la voce 'Informazioni' del menù Aiuto */
static gboolean pressed_menu_aiuto_informazioni(GtkWidget *widget, gpointer callback_data)
{
	gchar versione[24];

	g_sprintf(versione, "Versione %s", VERSION);

	gtk_label_set_text(label_versione, versione);

	gtk_widget_show_all((GtkWidget*)window_informazioni);

	return FALSE;
}


/* se viene premuto la voce 'Apri' del menù File */
static gboolean pressed_menu_file_apri(GtkWidget *widget, gpointer callback_data)
{
	g_signal_emit_by_name(button_carica_mese, "clicked", NULL);

	return FALSE;
}


/* se viene premuto la voce 'Esci' del menù File */
static gboolean pressed_menu_file_esci(GtkWidget *widget, gpointer callback_data)
{
	g_signal_emit_by_name(button_esci, "clicked", NULL);

	return FALSE;
}


/* chiude la finestra informazioni */
static gboolean pressed_button_chiudi_informazioni(GtkWidget *widget, gpointer callback_data)
{
	gtk_widget_hide((GtkWidget*)window_informazioni);

	return FALSE;
}



/****
 ***
 ***    PUNTATORI WIDGET
 ***
 ****/
/* iniziallizza i puntatori di tutti i widget che useremo e se c'è un errore  ritorna 0, se va tutto bene ritorna 1 , inoltra setta alcune proprietà */
int puntatori_widget(GtkBuilder *builder)
{
	/* se la gtk_builder_get_object ritorna puntatore nullo c'è un errore */
	if((window_principale = (GtkWindow*)gtk_builder_get_object(builder, "window_principale")) == 0) return 0;

	if((window_modifica_chiamata = (GtkWindow*)gtk_builder_get_object(builder, "window_modifica_chiamata")) == 0) return 0;

	if((window_informazioni = (GtkWindow*)gtk_builder_get_object(builder, "window_informazioni")) == 0) return 0;

	if((label_versione = (GtkLabel*)gtk_builder_get_object(builder, "label_versione")) == 0) return 0;

	if((barra_menu = (GtkMenuBar*)gtk_builder_get_object(builder, "barra_menu")) == 0) return 0;

	if((menu_aiuto_informazioni = (GtkWidget*)gtk_builder_get_object(builder, "menu_aiuto_informazioni")) == 0) return 0;

	if((menu_file_apri = (GtkWidget*)gtk_builder_get_object(builder, "menu_file_apri")) == 0) return 0;

	if((menu_file_esci = (GtkWidget*)gtk_builder_get_object(builder, "menu_file_esci")) == 0) return 0;

	if((label_titolo = (GtkLabel*)gtk_builder_get_object(builder, "label_titolo")) == 0) return 0;

	if((label_descrizione_accesso = (GtkLabel*)gtk_builder_get_object(builder, "label_descrizione_accesso")) == 0) return 0;

	if((button_indietro = (GtkButton*)gtk_builder_get_object(builder, "button_indietro")) == 0) return 0;

	if((button_esci = (GtkButton*)gtk_builder_get_object(builder, "button_esci")) == 0) return 0;

	if((button_chiudi_informazioni= (GtkButton*)gtk_builder_get_object(builder, "button_chiudi_informazioni")) == 0) return 0;

	if((box_accedi = (GtkBox*)gtk_builder_get_object(builder, "box_accedi")) == 0) return 0;

	if((box_menu_princ = (GtkBox*)gtk_builder_get_object(builder, "box_menu_princ")) == 0) return 0;

	if((box_resoconto_mese = (GtkBox*)gtk_builder_get_object(builder, "box_resoconto_mese")) == 0) return 0;

	if((box_mod_comp_att = (GtkBox*)gtk_builder_get_object(builder, "box_mod_comp_att")) == 0) return 0;

	if((box_reg_chiamata = (GtkBox*)gtk_builder_get_object(builder, "box_reg_chiamata")) == 0) return 0;

	if((box_lista_ch_mese = (GtkBox*)gtk_builder_get_object(builder, "box_lista_ch_mese")) == 0) return 0;

	if((box_clienti_appar = (GtkBox*)gtk_builder_get_object(builder, "box_clienti_appar")) == 0) return 0;

	if((box_statistiche_annuali = (GtkBox*)gtk_builder_get_object(builder, "box_statistiche_annuali")) == 0) return 0;

	if((box_cambio_pw = (GtkBox*)gtk_builder_get_object(builder, "box_cambio_pw")) == 0) return 0;

	if((box_ripristino = (GtkBox*)gtk_builder_get_object(builder, "box_ripristino")) == 0) return 0;

	if((entry_password = (GtkEntry*)gtk_builder_get_object(builder, "entry_password")) == 0) return 0;

	if((button_accedi = (GtkButton*)gtk_builder_get_object(builder, "button_accedi")) == 0) return 0;

	if((button_reg_chiamata = (GtkButton*)gtk_builder_get_object(builder, "button_reg_chiamata")) == 0) return 0;

	if((button_lista_ch_mese = (GtkButton*)gtk_builder_get_object(builder, "button_lista_ch_mese")) == 0) return 0;

	if((button_salva_chiamata1 = (GtkButton*)gtk_builder_get_object(builder, "button_salva_chiamata1")) == 0) return 0;

	if((button_annulla1 = (GtkButton*)gtk_builder_get_object(builder, "button_annulla1")) == 0) return 0;

	if((button_database = (GtkButton*)gtk_builder_get_object(builder, "button_database")) == 0) return 0;

	if((button_carica_mese = (GtkButton*)gtk_builder_get_object(builder, "button_carica_mese")) == 0) return 0;

	if((button_resoc_mese = (GtkButton*)gtk_builder_get_object(builder, "button_resoc_mese")) == 0) return 0;

	if((button_stat_annuali = (GtkButton*)gtk_builder_get_object(builder, "button_stat_annuali")) == 0) return 0;

	if((button_cambio_pw = (GtkButton*)gtk_builder_get_object(builder, "button_cambio_pw")) == 0) return 0;

	if((button_mod_comp_att = (GtkButton*)gtk_builder_get_object(builder, "button_mod_comp_att")) == 0) return 0;

	if((button_elimina_ch = (GtkButton*)gtk_builder_get_object(builder, "button_elimina_ch")) == 0) return 0;

	if((button_modifica_ch = (GtkButton*)gtk_builder_get_object(builder, "button_modifica_ch")) == 0) return 0;

	if((textbuffer_data = (GtkTextBuffer*)gtk_builder_get_object(builder, "textbuffer_data")) == 0) return 0;

	if((textbuffer_mese = (GtkTextBuffer*)gtk_builder_get_object(builder, "textbuffer_mese")) == 0) return 0;

	if((entry_gg = (GtkEntry*)gtk_builder_get_object(builder, "entry_gg")) == 0) return 0;

	if((entry_mm = (GtkEntry*)gtk_builder_get_object(builder, "entry_mm")) == 0) return 0;

	if((entry_aaaa = (GtkEntry*)gtk_builder_get_object(builder, "entry_aaaa")) == 0) return 0;

	if((entry_ora_in_spost = (GtkEntry*)gtk_builder_get_object(builder, "entry_ora_in_spost")) == 0) return 0;

	if((entry_ora_in_lav = (GtkEntry*)gtk_builder_get_object(builder, "entry_ora_in_lav")) == 0) return 0;

	if((entry_ora_fine_lav = (GtkEntry*)gtk_builder_get_object(builder, "entry_ora_fine_lav")) == 0) return 0;

	if((combo_box_text_cliente = (GtkComboBoxText*)gtk_builder_get_object(builder, "combo_box_text_cliente")) == 0) return 0;

	if((combo_box_text_appar = (GtkComboBoxText*)gtk_builder_get_object(builder, "combo_box_text_appar")) == 0) return 0;

	if((entry_luogo = (GtkEntry*)gtk_builder_get_object(builder, "entry_luogo")) == 0) return 0;

	if((entry_guasto = (GtkEntry*)gtk_builder_get_object(builder, "entry_guasto")) == 0) return 0;

	if((entry_lav_eseguito = (GtkEntry*)gtk_builder_get_object(builder, "entry_lav_eseguito")) == 0) return 0;

	if((entry_ore_lav = (GtkEntry*)gtk_builder_get_object(builder, "entry_ore_lav")) == 0) return 0;

	if((entry_compenso = (GtkEntry*)gtk_builder_get_object(builder, "entry_compenso")) == 0) return 0;

	/* allinea il testo a destra nell'entry_compenso */
	gtk_entry_set_alignment (entry_compenso, 1);

	if((entry_compenso_cent = (GtkEntry*)gtk_builder_get_object(builder, "entry_compenso_cent")) == 0) return 0;

	if((button_salva_chiamata = (GtkButton*)gtk_builder_get_object(builder, "button_salva_chiamata")) == 0) return 0;

	if((check_cht = (GtkCheckButton*)gtk_builder_get_object(builder, "check_cht")) == 0) return 0;

	if((check_chtl = (GtkCheckButton*)gtk_builder_get_object(builder, "check_chtl")) == 0) return 0;

	if((check_uvt = (GtkCheckButton*)gtk_builder_get_object(builder, "check_uvt")) == 0) return 0;

	if((check_chp = (GtkCheckButton*)gtk_builder_get_object(builder, "check_chp")) == 0) return 0;

	if((check_dis = (GtkCheckButton*)gtk_builder_get_object(builder, "check_dis")) == 0) return 0;

	if((check_uvp = (GtkCheckButton*)gtk_builder_get_object(builder, "check_uvp")) == 0) return 0;

	if((check_za = (GtkCheckButton*)gtk_builder_get_object(builder, "check_za")) == 0) return 0;

	if((check_zb = (GtkCheckButton*)gtk_builder_get_object(builder, "check_zb")) == 0) return 0;

	if((check_zc = (GtkCheckButton*)gtk_builder_get_object(builder, "check_zc")) == 0) return 0;

	if((check_compenso = (GtkCheckButton*)gtk_builder_get_object(builder, "check_compenso")) == 0) return 0;

	if((entry_comp_ch_t = (GtkEntry*)gtk_builder_get_object(builder, "entry_comp_ch_t")) == 0) return 0;

	if((entry_comp_ch_t_dec = (GtkEntry*)gtk_builder_get_object(builder, "entry_comp_ch_t_dec")) == 0) return 0;

	if((entry_comp_ch_tl = (GtkEntry*)gtk_builder_get_object(builder, "entry_comp_ch_tl")) == 0) return 0;

	if((entry_comp_ch_tl_dec = (GtkEntry*)gtk_builder_get_object(builder, "entry_comp_ch_tl_dec")) == 0) return 0;

	if((entry_comp_uvt = (GtkEntry*)gtk_builder_get_object(builder, "entry_comp_uvt")) == 0) return 0;

	if((entry_comp_uvt_dec = (GtkEntry*)gtk_builder_get_object(builder, "entry_comp_uvt_dec")) == 0) return 0;

	if((entry_comp_ch_p = (GtkEntry*)gtk_builder_get_object(builder, "entry_comp_ch_p")) == 0) return 0;

	if((entry_comp_ch_p_dec = (GtkEntry*)gtk_builder_get_object(builder, "entry_comp_ch_p_dec")) == 0) return 0;

	if((entry_comp_dis_p = (GtkEntry*)gtk_builder_get_object(builder, "entry_comp_dis_p")) == 0) return 0;

	if((entry_comp_dis_p_dec = (GtkEntry*)gtk_builder_get_object(builder, "entry_comp_dis_p_dec")) == 0) return 0;

	if((entry_comp_uvp = (GtkEntry*)gtk_builder_get_object(builder, "entry_comp_uvp")) == 0) return 0;

	if((entry_comp_uvp_dec = (GtkEntry*)gtk_builder_get_object(builder, "entry_comp_uvp_dec")) == 0) return 0;

	if((entry_comp_zb = (GtkEntry*)gtk_builder_get_object(builder, "entry_comp_zb")) == 0) return 0;

	if((entry_comp_zb_dec = (GtkEntry*)gtk_builder_get_object(builder, "entry_comp_zb_dec")) == 0) return 0;

	if((entry_comp_zc = (GtkEntry*)gtk_builder_get_object(builder, "entry_comp_zc")) == 0) return 0;

	if((entry_comp_zc_dec = (GtkEntry*)gtk_builder_get_object(builder, "entry_comp_zc_dec")) == 0) return 0;

	if((entry_comp_lav_ore = (GtkEntry*)gtk_builder_get_object(builder, "entry_comp_lav_ore")) == 0) return 0;

	if((entry_comp_lav_ore_dec = (GtkEntry*)gtk_builder_get_object(builder, "entry_comp_lav_ore_dec")) == 0) return 0;

	if((entry_comp_iva = (GtkEntry*)gtk_builder_get_object(builder, "entry_comp_iva")) == 0) return 0;

	if((button_salva_comp_att = (GtkButton*)gtk_builder_get_object(builder, "button_salva_comp_att")) == 0) return 0;

	if((entry_gg1 = (GtkEntry*)gtk_builder_get_object(builder, "entry_gg1")) == 0) return 0;

	if((entry_mm1 = (GtkEntry*)gtk_builder_get_object(builder, "entry_mm1")) == 0) return 0;

	if((entry_aaaa1 = (GtkEntry*)gtk_builder_get_object(builder, "entry_aaaa1")) == 0) return 0;

	if((entry_ora_in_spost1 = (GtkEntry*)gtk_builder_get_object(builder, "entry_ora_in_spost1")) == 0) return 0;

	if((entry_ora_in_lav1 = (GtkEntry*)gtk_builder_get_object(builder, "entry_ora_in_lav1")) == 0) return 0;

	if((entry_ora_fine_lav1 = (GtkEntry*)gtk_builder_get_object(builder, "entry_ora_fine_lav1")) == 0) return 0;

	if((combo_box_text_cliente1 = (GtkComboBoxText*)gtk_builder_get_object(builder, "combo_box_text_cliente1")) == 0) return 0;

	if((combo_box_text_appar1 = (GtkComboBoxText*)gtk_builder_get_object(builder, "combo_box_text_appar1")) == 0) return 0;

	if((entry_luogo1 = (GtkEntry*)gtk_builder_get_object(builder, "entry_luogo1")) == 0) return 0;

	if((entry_guasto1 = (GtkEntry*)gtk_builder_get_object(builder, "entry_guasto1")) == 0) return 0;

	if((entry_lav_eseguito1 = (GtkEntry*)gtk_builder_get_object(builder, "entry_lav_eseguito1")) == 0) return 0;

	if((entry_ore_lav1 = (GtkEntry*)gtk_builder_get_object(builder, "entry_ore_lav1")) == 0) return 0;

	if((entry_compenso1 = (GtkEntry*)gtk_builder_get_object(builder, "entry_compenso1")) == 0) return 0;

	/* allinea il testo a destra nell'entry_compenso1 */
	gtk_entry_set_alignment (entry_compenso1, 1);

	if((entry_compenso_cent1 = (GtkEntry*)gtk_builder_get_object(builder, "entry_compenso_cent1")) == 0) return 0;

	if((button_salva_chiamata1 = (GtkButton*)gtk_builder_get_object(builder, "button_salva_chiamata1")) == 0) return 0;

	if((check_cht1 = (GtkCheckButton*)gtk_builder_get_object(builder, "check_cht1")) == 0) return 0;

	if((check_chtl1 = (GtkCheckButton*)gtk_builder_get_object(builder, "check_chtl1")) == 0) return 0;

	if((check_uvt1 = (GtkCheckButton*)gtk_builder_get_object(builder, "check_uvt1")) == 0) return 0;

	if((check_chp1 = (GtkCheckButton*)gtk_builder_get_object(builder, "check_chp1")) == 0) return 0;

	if((check_dis1 = (GtkCheckButton*)gtk_builder_get_object(builder, "check_dis1")) == 0) return 0;

	if((check_uvp1 = (GtkCheckButton*)gtk_builder_get_object(builder, "check_uvp1")) == 0) return 0;

	if((check_za1 = (GtkCheckButton*)gtk_builder_get_object(builder, "check_za1")) == 0) return 0;

	if((check_zb1 = (GtkCheckButton*)gtk_builder_get_object(builder, "check_zb1")) == 0) return 0;

	if((check_zc1 = (GtkCheckButton*)gtk_builder_get_object(builder, "check_zc1")) == 0) return 0;

	if((check_compenso1 = (GtkCheckButton*)gtk_builder_get_object(builder, "check_compenso1")) == 0) return 0;

	if((scr_window_clienti =(GtkScrolledWindow*)gtk_builder_get_object(builder, "scr_window_clienti")) == 0) return 0;

	if((scr_window_appar =(GtkScrolledWindow*)gtk_builder_get_object(builder, "scr_window_appar")) == 0) return 0;

    if((scr_window_lista_ch_mese =(GtkScrolledWindow*)gtk_builder_get_object(builder, "scr_window_lista_ch_mese")) == 0) return 0;

	if((button_agg_appar = (GtkButton*)gtk_builder_get_object(builder, "button_agg_appar")) == 0) return 0;

	if((button_agg_cliente = (GtkButton*)gtk_builder_get_object(builder, "button_agg_cliente")) == 0) return 0;

	if((button_elimina_appar = (GtkButton*)gtk_builder_get_object(builder, "button_elimina_appar")) == 0) return 0;

	if((button_elimina_cliente = (GtkButton*)gtk_builder_get_object(builder, "button_elimina_cliente")) == 0) return 0;

	if((image_resoconto_mese = (GtkImage*)gtk_builder_get_object(builder, "image_resoconto_mese")) == 0) return 0;

	if((button_copia_dati = (GtkButton*)gtk_builder_get_object(builder, "button_copia_dati")) == 0) return 0;

	if((combo_box_text_anno = (GtkComboBoxText*)gtk_builder_get_object(builder, "combo_box_text_anno")) == 0) return 0;

	if((button_aggiorna = (GtkButton*)gtk_builder_get_object(builder, "button_aggiorna")) == 0) return 0;

	if((image_stat_annuali = (GtkImage*)gtk_builder_get_object(builder, "image_stat_annuali")) == 0) return 0;

	if((entry_vecchia_pw = (GtkEntry*)gtk_builder_get_object(builder, "entry_vecchia_pw")) == 0) return 0;

	if((entry_nuova_pw = (GtkEntry*)gtk_builder_get_object(builder, "entry_nuova_pw")) == 0) return 0;

	if((entry_conf_nuova_pw = (GtkEntry*)gtk_builder_get_object(builder, "entry_conf_nuova_pw")) == 0) return 0;

	if((button_ok = (GtkButton*)gtk_builder_get_object(builder, "button_ok")) == 0) return 0;

/****
 ***    widget che non vengono caricati direttamente dal builder, ma che vengono   
 ***    creati direttamente nel programma                                                                  
 ****/

	/* ottiene puntatore all'entry inglobato nel comboboxtext per l'inserimento del cliente nella schermata 'registra chiamata' */
	entry_cliente = (GtkEntry*)gtk_bin_get_child((GtkBin*)combo_box_text_cliente);
	gtk_entry_set_max_length(entry_cliente, MAX_CHAR_CLIENTE);

	/* ottiene puntatore all'entry inglobato nel comboboxtext per l'inserimento dell' apparecchiatura nella schermata 'registra chiamata' */
	entry_appar = (GtkEntry*)gtk_bin_get_child((GtkBin*)combo_box_text_appar);
	gtk_entry_set_max_length(entry_appar, MAX_CHAR_APPAR);

	/* ottiene puntatore all'entry inglobato nel comboboxtext per l'inserimento del cliente nel dialog 'modifica chiamata' */
	entry_cliente1 = (GtkEntry*)gtk_bin_get_child((GtkBin*)combo_box_text_cliente1);
	gtk_entry_set_max_length(entry_cliente1, MAX_CHAR_CLIENTE);

	/* ottiene puntatore all'entry inglobato nel comboboxtext per l'inserimento dell' apparecchiatura nel dialog 'modifica chiamata' */
	entry_appar1 = (GtkEntry*)gtk_bin_get_child((GtkBin*)combo_box_text_appar1);
	gtk_entry_set_max_length(entry_appar1, MAX_CHAR_APPAR);

	/* ottiene puntatore all'entry inglobato nel comboboxtext per l'inserimento dell' anno da visualizzare nella schermata 'statistiche annuali */
	entry_anno_scelto = (GtkEntry*)gtk_bin_get_child((GtkBin*)combo_box_text_anno);
	gtk_entry_set_max_length(entry_anno_scelto, 0); /* zero caratteri perchè non voglio che sia scrivibile, ma si devono solo sciegliere i 
							 * valori proposti nella lista */

	return 1;
}




/****
 ***
 ***		CONNESSIONE SIGNAL HANDLERS
 ***
 ****/
void connessione_signal_handlers()
{
	g_signal_connect(window_principale, "destroy", G_CALLBACK(uscita_normale), NULL);

	g_signal_connect(window_modifica_chiamata, "delete_event", G_CALLBACK(delete_event), NULL);

	g_signal_connect(window_principale, "delete-event", G_CALLBACK(delete_event), NULL);

	g_signal_connect(menu_aiuto_informazioni, "activate", G_CALLBACK(pressed_menu_aiuto_informazioni), NULL);

	g_signal_connect(menu_file_apri, "activate", G_CALLBACK(pressed_menu_file_apri), NULL);

	g_signal_connect(menu_file_esci, "activate", G_CALLBACK(pressed_menu_file_esci), NULL);

	g_signal_connect(entry_password, "key_press_event", G_CALLBACK(enter_key_pressed), NULL);

	g_signal_connect(button_accedi, "clicked", G_CALLBACK(pressed_button_accedi), NULL);

	g_signal_connect(button_indietro, "clicked", G_CALLBACK(pressed_button_indietro), NULL);

	g_signal_connect(button_esci, "clicked", G_CALLBACK(pressed_button_esci), NULL);

	g_signal_connect(button_reg_chiamata, "clicked", G_CALLBACK(pressed_button_reg_chiamata), NULL);

	g_signal_connect(button_lista_ch_mese, "clicked", G_CALLBACK(pressed_button_lista_ch_mese), NULL);

	g_signal_connect(button_carica_mese, "clicked", G_CALLBACK(pressed_button_carica_mese), NULL);

	g_signal_connect(button_database, "clicked", G_CALLBACK(pressed_button_database), NULL);

	g_signal_connect(button_resoc_mese, "clicked", G_CALLBACK(pressed_button_resoc_mese), NULL);

	g_signal_connect(button_stat_annuali, "clicked", G_CALLBACK(pressed_button_stat_annuali), NULL);

	g_signal_connect(button_cambio_pw, "clicked", G_CALLBACK(pressed_button_cambio_pw), NULL);

	g_signal_connect(button_mod_comp_att, "clicked", G_CALLBACK(pressed_button_mod_comp_att), NULL);

	g_signal_connect(button_modifica_ch, "clicked", G_CALLBACK(pressed_button_modifica_ch), NULL);

	g_signal_connect(button_elimina_ch, "clicked", G_CALLBACK(pressed_button_elimina_ch), NULL);

	g_signal_connect(button_salva_chiamata1, "clicked", G_CALLBACK(pressed_button_salva_chiamata1), NULL);

	g_signal_connect(button_annulla1, "clicked", G_CALLBACK(pressed_button_annulla1), NULL);

	g_signal_connect(button_salva_chiamata, "clicked", G_CALLBACK(pressed_button_salva_chiamata), NULL);

	g_signal_connect(button_chiudi_informazioni, "clicked", G_CALLBACK(pressed_button_chiudi_informazioni), NULL);

	g_signal_connect(entry_gg, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_mm, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_aaaa, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_ora_in_spost, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_ora_in_lav, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_ora_fine_lav, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_ore_lav, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_compenso, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_compenso_cent, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(button_salva_chiamata1, "clicked", G_CALLBACK(pressed_button_salva_chiamata1), NULL);

	g_signal_connect(entry_gg1, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_mm1, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_aaaa1, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_ora_in_spost1, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_ora_in_lav1, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_ora_fine_lav1, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_ore_lav1, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_compenso1, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_compenso_cent1, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(button_agg_appar, "clicked", G_CALLBACK(pressed_button_agg_appar), NULL);

	g_signal_connect(button_agg_cliente, "clicked", G_CALLBACK(pressed_button_agg_cliente), NULL);

	g_signal_connect(button_elimina_appar, "clicked", G_CALLBACK(pressed_button_elimina_appar), NULL);

	g_signal_connect(button_elimina_cliente, "clicked", G_CALLBACK(pressed_button_elimina_cliente), NULL);

	g_signal_connect(button_salva_comp_att, "clicked", G_CALLBACK(pressed_button_salva_comp_att), NULL);

	g_signal_connect(entry_comp_ch_t, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_comp_ch_t_dec, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_comp_ch_tl, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_comp_ch_tl_dec, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_comp_uvt, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_comp_uvt_dec, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_comp_ch_p, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_comp_ch_p_dec, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_comp_dis_p, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_comp_dis_p_dec, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_comp_uvp, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_comp_uvp_dec, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_comp_zb, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_comp_zb_dec, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_comp_zc, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_comp_zc_dec, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_comp_lav_ore, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_comp_lav_ore_dec, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(entry_comp_iva, "insert-text", G_CALLBACK(inserisci_solo_numeri), NULL);

	g_signal_connect(button_copia_dati, "clicked", G_CALLBACK(pressed_button_copia_dati), NULL);  

	g_signal_connect(button_aggiorna, "clicked", G_CALLBACK(pressed_button_aggiorna), NULL);

	g_signal_connect(button_ok, "clicked", G_CALLBACK(pressed_button_ok), NULL);

	return;
}



/****
 ***
 ***		MAIN
 ***
 *****/
int main(int argc, char** argv) {

	char controllo;
	FILE *stream_file_config;
	GError *error = NULL;
	gchar label_descrizione_accesso_testo[256];

	/* setta alcune variabili globali usate in seguito */
	database_server_montato = FALSE;
	database_locale = FALSE;
	modalita_ripristino = FALSE;

	esci_subito = FALSE;

	/* percorso della directoy HOME */
	home_dir = g_getenv("HOME");

	/* definisce le directory e i percorsi dei file principali */
	g_sprintf(gui_dir, "%s/gechilx_client/gui", home_dir);
	g_sprintf(program_dir, "%s/gechilx_client", home_dir);
	g_sprintf(database_dir, "%s/gechilx_client/database", home_dir);
	g_sprintf(file_appar_path, "%s/database/appar.gec", program_dir);
	g_sprintf(file_clienti_path, "%s/database/clienti.gec", program_dir);
	g_sprintf(file_config_path, "%s/gechilx_client/system.gec", home_dir);
	g_sprintf(file_gui_path, "%s/gechilx.glade", gui_dir);

	/* inizializza GTK+ */
	gtk_init(&argc, &argv);

	/* crea un nuovo oggetto GtkBuilder */
	builder = gtk_builder_new();

	/* carica l'interfaccia grafica dal file gechilx.glade */
	if(!gtk_builder_add_from_file(builder, file_gui_path, &error))
	{
		g_warning("%s", error->message);
		g_free(error);
		return(1);
	}

	/* inizializza i puntatori di tutti i widget che useremo, se c'è un errore esce dal programma e ritorna il codice 2 */
	if( !(puntatori_widget(builder)) ) return(2);

	/* connessione segnali standard*/
	gtk_builder_connect_signals(builder, NULL);

	/* connessione segnali  impostati  da noi */
	connessione_signal_handlers();

	g_sprintf(label_descrizione_accesso_testo, "\n Gestione Chiamate for Linux\n Versione %s", VERSION); 

	gtk_label_set_text(label_descrizione_accesso, label_descrizione_accesso_testo); 

	/* apre il file di configurazione 'system.gec' in lettura e ne ottiene il puntatore allo stream */
	stream_file_config = fopen(file_config_path, "r");

	/* se non riesce ad aprire il file 'system.gec' segnala l'errore ed esce */
	if(stream_file_config == 0)
	{
		crea_dialog_box_un_pulsante(dialog_fceom, "ATTENZIONE!", 
		"\n File di configurazione errato o mancante. \n Non è possibile proseguire. \n", window_principale, "Chiudi"); 
		return 1;
	}

	/* controlla se il primo carattere del file 'system.gec è '1' , in caso affermativo significa che la volta precedente si era
	* lavorato su una copia del database locale, altrimenti accede al programma normalmente */
	controllo = fgetc(stream_file_config);
	if(controllo == '1')
	{
		/* mostra la finestra di ripristino, tutti gli altri widget sono mostrati automaticamente da GtkBuilder */
		gtk_widget_show((GtkWidget*)box_ripristino);
		
		/* nasconde la barra menu */
		gtk_widget_hide((GtkWidget*)barra_menu);
		
		gtk_label_set_text(label_titolo, "ATTENZIONE!");
		
		/* settando modalita_ripristino a TRUE , chiudendo il programma, nella funzione delete_event ,verrà nuovamente impostato 
		 * il primo byte del file 'system.gec' col carattere '0'. Cosi facendo al riavvio  seguente il programma partirà in modalita normale */
		modalita_ripristino = TRUE;
		
		schermata_corrente = SCHERMATA_RIPRISTINO;
		
		gtk_widget_grab_focus((GtkWidget*)button_esci);	//focus sul pulsante Esci
	}
	else // avvio normale del programma
	{
		/* collega il database montando la condivisione col server, se l'operazione è andata a buon fine setta la variabile 
		 * globale 'database_server_montato' a TRUE, perchè dopo servirà */
		g_sprintf(comando, "mount %s", database_dir);
		if(!(system(comando))) database_server_montato = TRUE;	/* la funzione system esegue il comando di sistema passato come argomento e 
									 * ritorna zero se è andato a buon fine e se è andato a buon fine la 
									 * variabile 'database_server_montato' diventa TRUE */

		gtk_window_set_default_size(window_principale, 800, 600);

		/* mostra la finestra di accesso. tutti gli altri widget sono mostrati automaticamente da GtkBuilder */
		gtk_widget_show_all((GtkWidget*)box_accedi);
		schermata_corrente = SCHERMATA_ACCESSO;
	}

	gtk_window_set_default_size(window_principale, 800, 600);
	
	gtk_widget_show((GtkWidget*)window_principale);

	/* inizia main loop */
	gtk_main();

	return 0;
}
