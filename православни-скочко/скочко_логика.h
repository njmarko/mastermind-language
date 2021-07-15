
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "defs.h"
#include "codegen.h"

FILE *output;

static char *znakovi_za_ispis[9] = {znak_skocko, znak_tref, znak_pik, znak_herc, znak_karo, znak_zvezda};

static enum Znak trazena_kombinacija[VELICINA_KOMBINACIJE] = {0};

static enum Znak unesena_kombinacija[VELICINA_KOMBINACIJE] = {0};

static enum Znak histogram_unesene_kombinacije[6];
static enum Znak histogram_trazene_kombinacije[6];

static char *znakovi_za_tablu[BROJ_ZNAKOVA_TABLA];
static char *znakovi_trazena_kombinacija[VELICINA_KOMBINACIJE];

static int tacna_pozicija = 0;
static int netacna_pozicija = 0;

static int broj_unetih_znakova = 0;

static int trenutni_znak = 0;

static bool igra_zavrsena = TRUE;

static int broj_partije = 0;

void isprazni_tablu();
void ocisti_terminal();
void napravi_histogram(enum Znak *kombinacija, enum Znak *histogram);
void evaluiraj_poziciju(enum Znak *kombinacija1, enum Znak *kombinacija2);
void odigraj_kombinaciju();
void napravi_random_trazenu_kombinaciju();
void kompajliraj_izlaz();
void napravi_zerobyte_fajl();
void zavrsi_upis_u_fajl();
void nova_igra();
void ispis_nakon_unete_kombinacije();
void unesi_znak(enum Znak znak);
bool provera_broja_za_znak(int broj);