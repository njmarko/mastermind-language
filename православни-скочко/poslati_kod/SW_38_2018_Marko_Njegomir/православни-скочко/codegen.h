#ifndef CODEGEN_H
#define CODEGEN_H

#include "defs.h"

int enkodiraj_kombinaciju(enum Znak * kombinacija);

void generisi_data_sekciju();

void generisi_pomocne_funkcije();

void generisi_interaktivnu_igru();

void generisi_pocetak_text_sekcije();

void generisi_unetu_kombinaciju(enum Znak * kombinacija, int redni_broj);

void generisi_trazenu_kombinaciju(enum Znak * kombinacija);

void generisi_kraj(int redni_broj);

#endif