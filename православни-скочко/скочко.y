%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h> 
    #include "defs.h"
    #include "symtab.h"
    #include "codegen.h"

    int yyparse(void);
    int yylex(void);
    int yyerror(char *s);
    void warning(char *s);

    extern int yylineno;
    int out_lin = 0;
    char char_buffer[CHAR_BUFFER_LENGTH];
    int error_count = 0;
    int warning_count = 0;

    FILE *output;



    char * znakovi_za_ispis[9] = {znak_skocko, znak_tref, znak_pik, znak_herc, znak_karo, znak_zvezda};

    enum Znak trazena_kombinacija[VELICINA_KOMBINACIJE] = {0};

    enum Znak unesena_kombinacija[VELICINA_KOMBINACIJE] = {0};

    enum Znak histogram_unesene_kombinacije[6];
    enum Znak histogram_trazene_kombinacije[6];

    char* znakovi_za_tablu[BROJ_ZNAKOVA_TABLA];
    char* znakovi_trazena_kombinacija[VELICINA_KOMBINACIJE];
    
    int tacna_pozicija = 0;
    int netacna_pozicija = 0 ;

    int broj_unetih_znakova = 0;

    int trenutni_znak = 0;

    bool igra_zavrsena = TRUE;

    void isprazni_tablu(){
      int i;
      for(i = 0; i< BROJ_ZNAKOVA_TABLA; ++i){
        znakovi_za_tablu[i] = znak_prazan;
      }
    }

    void ocisti_terminal(){
    // printf("%s", ocisti_ekran);
      system("clear");
    }

    void napravi_histogram(enum Znak * kombinacija, enum Znak * histogram){
      int i;
      // postavim sve vrednosti na nula u histogramu
      for(i = 0; i < BROJ_ZNAKOVA; ++i){
        histogram[i] = 0;
      }
      // napravim histogram na osnovu prosledjene kombinacije
      for(i = 0; i < VELICINA_KOMBINACIJE; ++i){
        histogram[kombinacija[i]]++;
      }
    }


    void evaluiraj_poziciju(enum Znak * kombinacija1, enum Znak * kombinacija2){
      napravi_histogram(kombinacija1, histogram_unesene_kombinacije);
      tacna_pozicija = 0;
      netacna_pozicija = 0;
      int i;
      // prebroj znakove koji su na netacnim pozicijama
      for(int i = 0; i < BROJ_ZNAKOVA; ++i){
        if(histogram_unesene_kombinacije[i] <= histogram_trazene_kombinacije[i]){
          netacna_pozicija += histogram_unesene_kombinacije[i];
        } else {
          netacna_pozicija += histogram_trazene_kombinacije[i];
        }
      }

      // prebroj znakove na tacnim pozicijama
      for (int i = 0; i < VELICINA_KOMBINACIJE; ++i){
        if(kombinacija1[i] == trazena_kombinacija[i]){
          tacna_pozicija++;
        }
      }
      netacna_pozicija -= tacna_pozicija;
    }

    void odigraj_kombinaciju(){
      evaluiraj_poziciju(unesena_kombinacija, trazena_kombinacija);
      int i;
      for(i = 0; i < VELICINA_KOMBINACIJE; ++i){
        znakovi_za_tablu[broj_unetih_znakova++] = znakovi_za_ispis[unesena_kombinacija[i]];
      }
      for(i = 0; i < tacna_pozicija; ++i){
        znakovi_za_tablu[broj_unetih_znakova++] = znak_crveni;
      }
      for(i = 0; i < netacna_pozicija; ++i){
        znakovi_za_tablu[broj_unetih_znakova++] = znak_zuti;
      }
      for(i = 0; i < VELICINA_KOMBINACIJE - tacna_pozicija - netacna_pozicija; ++i){
        znakovi_za_tablu[broj_unetih_znakova++] = znak_prazan;
      }

      printf(tabla, tabla_args(znakovi_za_tablu));
    }

    void napravi_random_trazenu_kombinaciju(){
      int i;
      for(i = 0; i < VELICINA_KOMBINACIJE; ++i){
        trazena_kombinacija[i] = rand() % 6;
        znakovi_trazena_kombinacija[i] = znakovi_za_ispis[trazena_kombinacija[i]];
      }
    }

    void zavrsi_upis_u_fajl(){
        if (!igra_zavrsena)
        {
          generisi_kraj(broj_unetih_znakova/(2*VELICINA_KOMBINACIJE));
          fclose(output);
        }
        igra_zavrsena = FALSE;
    }

    void nova_igra(){
        zavrsi_upis_u_fajl();
        output = fopen("output.S", "w+");
        
        generisi_data_sekciju();
        generisi_pocetak_text_sekcije();
        generisi_pomocne_funkcije();
        // generisi_interaktivnu_igru();

        ocisti_terminal();
        trenutni_znak = 0;
        broj_unetih_znakova = 0;
        tacna_pozicija = 0;
        netacna_pozicija = 0;
        igra_zavrsena = FALSE;
        isprazni_tablu();
        napravi_random_trazenu_kombinaciju();

        generisi_trazenu_kombinaciju(trazena_kombinacija);

        napravi_histogram(trazena_kombinacija, histogram_trazene_kombinacije);
        printf(tabla, tabla_args(znakovi_za_tablu));
        printf("%s", poruka_unos);
    }

    void ispis_nakon_unete_kombinacije(){
        generisi_unetu_kombinaciju(unesena_kombinacija, broj_unetih_znakova/(2*VELICINA_KOMBINACIJE));
        if(tacna_pozicija == VELICINA_KOMBINACIJE){
          printf("%s", poruka_kombinacija_pogodjena); printf("%s", poruka_kraj_partije);
          igra_zavrsena = TRUE;
        } else if(broj_unetih_znakova >= BROJ_ZNAKOVA_TABLA){
          printf(ispravna_kombinacija_ispis, komb_trazena_args(znakovi_trazena_kombinacija));
          printf("%s", poruka_kraj_partije);
          igra_zavrsena = TRUE;
        } else {
          printf("%s", poruka_unos);
        }
        if (igra_zavrsena)
        {
          generisi_kraj(broj_unetih_znakova/(2*VELICINA_KOMBINACIJE));
          fclose(output);
        }
        
    }

    void unesi_znak(enum Znak znak){
      if(znak == NEISPRAVAN_ZNAK){
        return;
      }
      // provera da li je trenutna igra zavrsena
      else if(broj_unetih_znakova >= BROJ_ZNAKOVA_TABLA || tacna_pozicija == VELICINA_KOMBINACIJE){
        ocisti_terminal();
        printf("%s", poruka_kraj_partije);
      }
      else
      {
        unesena_kombinacija[trenutni_znak++] = znak;
        if ((trenutni_znak + 1) % 5 == 0)
        {
          ocisti_terminal();
          trenutni_znak = 0;
          odigraj_kombinaciju();
          ispis_nakon_unete_kombinacije();
        }
      }
    }

    bool provera_broja_za_znak(int broj){
      if (broj < 1 || broj > BROJ_ZNAKOVA)
      {
        printf("%s", poruka_broj_van_opsega_znaka);
        return FALSE;
      }
      return TRUE;

    }


%}


%union {
    int i;
    char *s;
}


%token _NOVA_IGRA
%token _KRAJ
%token <i> _ZNAK
%token <s> _BROJ

%type <i> unos_za_znak

%%

program
    : znakovi_pre_pocetka igra _KRAJ
    {
      zavrsi_upis_u_fajl();
      YYACCEPT;
    }
    ; 

znakovi_pre_pocetka
    : /* prazna */
    | znakovi_pre_pocetka _KRAJ
    {
      zavrsi_upis_u_fajl();
      YYACCEPT;
    }
    | znakovi_pre_pocetka unos_za_znak
    {
      printf("%s", poruka_pocetak_partije);
    }
    ;

igra
    : _NOVA_IGRA 
    {
      nova_igra();
    } 
    lista_kombinacija
    | igra _NOVA_IGRA
    {
      nova_igra();
    }
    lista_kombinacija
    ;

lista_kombinacija
    : /* prazna */
    | kombinacija
    ;

kombinacija
    : unos_za_znak 
    {
        unesi_znak($1);
    }
    | kombinacija unos_za_znak
    {
        unesi_znak($2);
    }
    ;

unos_za_znak
    : _ZNAK
    {
      $$ = $1;
    }
    | _BROJ
    {
      if(provera_broja_za_znak(atoi($1))){
        $$ = atoi($1) - 1;
      } else {
        $$ = NEISPRAVAN_ZNAK;
      }
    }
    ;

%%

int yyerror(char *s) {
  fprintf(stderr, "\nline %d: ERROR: %s", yylineno, s);
  error_count++;
  return 0;
}

void warning(char *s) {
  fprintf(stderr, "\nline %d: WARNING: %s", yylineno, s);
  warning_count++;
}

int main(){
    // extern int yydebug;
    // yydebug = 1;

    ocisti_terminal();
    srand(time(0));

    int synerr;
    init_symtab();
    // output = fopen("output.S", "w+");

    printf("%s", poruka_pocetak_partije);
    synerr = yyparse();
    printf("%s", poruka_kraj_programa);


    clear_symtab();
    // fclose(output);


    return 0;
}