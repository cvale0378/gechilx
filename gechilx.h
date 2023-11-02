/* 
 * File:   gechilx.h
 * Author: crvubu
 *
 * Created on 6 ottobre 2010, 21.55
 */

#ifndef	_GECHILX_H
#define	_GECHILX_H

#define	SCHERMATA_ACCESSO		0
#define	SCHERMATA_MENU_PRINC		1
#define	SCHERMATA_REG_CH		2
#define	SCHERMATA_MOD_CH		3
#define	SCHERMATA_DATABASE_CL_APP	4
#define	SCHERMATA_MOD_COMP_ATT		5
#define	SCHERMATA_LISTA_CH_MESE		6
#define	SCHERMATA_CAMBIO_PW		7
#define	SCHERMATA_RESOCONTO		8
#define	SCHERMATA_STAT_ANN		9
#define	SCHERMATA_RIPRISTINO		10

#define	MAX_CHAR_PW	11
#define	MAXPATHLEN	1024
#define	MAXSTRLEN	256      /* deve essere multiplo di 4  */

/* massimo numero di chiamate che si possono salvare in un mese */
#define	MAX_NUM_CH_MESE	1000

/* definizione dimensione buffer dati per una chiamata */
#define	LENGHT_CHIAMATA_WORD 	780
#define	LENGHT_CHIAMATA_BYTE	1560

/* offset da dove sono salvati i compensi delle attività del mese nel file chiamate mese */
#define	OFFSET_COMPENSI	192

/* definizione massimo numero di caratteri per i vari entry */
#define	MAX_CHAR_N_CHIAMATA	3
#define	MAX_CHAR_DATA		10
#define	MAX_CHAR_GG		2
#define	MAX_CHAR_MM		2
#define	MAX_CHAR_AAAA		4
#define	MAX_CHAR_ORA_IN_SPOST	4
#define	MAX_CHAR_ORA_IN_LAV	4
#define	MAX_CHAR_ORA_FINE_LAV	4
#define	MAX_CHAR_CLIENTE	40
#define	MAX_CHAR_LUOGO		40
#define	MAX_CHAR_APPAR		30
#define	MAX_CHAR_GUASTO		30
#define	MAX_CHAR_LAV_ESEG	495
#define	MAX_CHAR_FLAGS		9
#define	MAX_CHAR_ORE		2
#define MAX_CHAR_COMPENSO	5

/* offset dei vari campi di una chiamata quando è salvata */
#define	OFFSET_N_CHIAMATA	4
#define	OFFSET_DATA		26
#define	OFFSET_ORA_IN_SPOST	58
#define	OFFSET_ORA_IN_LAV	70
#define	OFFSET_ORA_FINE_LAV	90
#define	OFFSET_CLIENTE		106
#define	OFFSET_LUOGO		208
#define	OFFSET_APPAR		306
#define	OFFSET_GUASTO		388
#define	OFFSET_LAV_ESEGUITO	466
#define	OFFSET_CATNS		1472
#define	OFFSET_CATNS_CENT	1488
#define	OFFSET_FLAGS		1510
#define	OFFSET_ORE		1546

/* numero totale dei flags per ogni chiamata */
#define	NUM_FLAGS	9

/* coordinate per tabella resconto mese */
#define	Y_INIZIO		60
#define	LARG_CELLA		74
#define	LARG_CELLA_CATNS	120     /* larghezza cella compenso attività a tariffa non standard */
#define	ALT_CELLA		17
#define	X_OFFSET_TESTO		20
#define	Y_OFFSET_TESTO		18

/* coordinate per grafico statistiche annuali */
#define	LARG_TOT_IMMAGINE	900
#define	ALT_TOT_IMMAGINE	675
#define	PROFONDITA_COLORE	24
#define	X0_GRAFICO_FATTURATO	50	/* pixel corrispondenti alla coordinata 0,y del grafico fatturato */
#define	Y0_GRAFICO_FATTURATO	310	/* pixel corrispondenti alla coordinata x,0 del grafico fatturato */ 
#define	X0_GRAFICO_ATTIVITA	50	/* pixel corrispondenti alla coordinata 0,y del grafico attivita */
#define	Y0_GRAFICO_ATTIVITA	650	/* pixel corrispondenti alla coordinata x,0 del grafico attivita */ 
#define	DELTA_MESE_X		50	/* spostamento di un mese nel grafico in pixel in entrambi i grafici */
#define	DELTA_1000_EURO_Y	60	/* spostamento di 1000 euro in pixel nel grafico fatturato */
#define	DELTA_1_ATTIVITA_Y	3	/* spostamento di 1 attivita in pixel nel grafico attivita */

/* database clienti e apparrecchiature */
#define	COLONNA_CLIENTE	0
#define	COLONNA_APPAR	0

/* larghezza delle colonne della lista chiamate mese */
#define LARGHEZZA_NUM_CH_COLUMN		60
#define LARGHEZZA_DATA_COLUMN		100
#define LARGHEZZA_CLIENTE_COLUMN	400
#define LARGHEZZA_LUOGO_COLUMN		400
#define LARGHEZZA_APPAR_COLUMN		300
#define LARGHEZZA_GUASTO_COLUMN		300
#define LARGHEZZA_FLAGS_COLUMN		110
#define LARGHEZZA_ORE_COLUMN		50
#define LARGHEZZA_COMPENSO_COLUMN	60

/* definisce tipo di dato per descrivere una chiamata */
typedef struct 
{
	int num_chiamata;

	const gchar 	*data_gg,
			*data_mm,
			*data_aaaa,
			*ora_spost,
			*ora_in_lav,
			*ora_fine_lav,
			*cliente,
			*luogo,
			*appar,
			*guasto,
			*comp_att_tariffa_non_stand,
			*comp_att_tariffa_non_stand_cent,
			*lav_eseguito,
			*ore_lav;

	gchar buffer_flags[MAX_CHAR_FLAGS + 1];

} chiamata;

/* tipo di dato per descrivere quali e quante attivita' eseguite nel giorno */
typedef struct 
{
	int ch_t;
	int ch_tl;
	int uvt;
	int ch_p;
	int dis_p;
	int uvp;
	int za;
	int zb;
	int zc;
	int lav_ore;
	float catns;    // compenso per attività a tariffa non standard
} attivita;

/* tipo di dato per descrivere i compensi per le attività e l'iva del mese */
typedef struct 
{
	float ch_t;
	float ch_tl;
	float uvt;
	float ch_p;
	float dis_p;
	float uvp;
	float za;
	float zb;
	float zc;
	float lav_ore;
	float iva;    
} compenso;

#endif	/* _GECHILX_H */
