%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h> 
    #include "defs.h"
    #include "symtab.h"

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

    #define BROJ_ZNAKOVA 6
    #define VELICINA_KOMBINACIJE 4
    #define BROJ_ZNAKOVA_TABLA 48
    #define tabla_args(a) a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15], a[16], a[17], a[18], a[19], a[20], a[21], a[22], a[23], a[24], a[25], a[26], a[27], a[28], a[29], a[30], a[31], a[32], a[33], a[34], a[35], a[36], a[37], a[38], a[39], a[40], a[41], a[42], a[43], a[44], a[45], a[46], a[47]
    #define red_args(a) a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]

    char * tabla = "\n| %-1s | %-1s | %-1s | %-1s |   |%1s|%1s|%1s|%1s|\n| %-1s | %-1s | %-1s | %-1s |   |%1s|%1s|%1s|%1s|\n| %-1s | %-1s | %-1s | %-1s |   |%1s|%1s|%1s|%1s|\n| %-1s | %-1s | %-1s | %-1s |   |%1s|%1s|%1s|%1s|\n| %-1s | %-1s | %-1s | %-1s |   |%1s|%1s|%1s|%1s|\n| %-1s | %-1s | %-1s | %-1s |   |%1s|%1s|%1s|%1s|\n\0";
    char * red = "\n|%-1s |%-1s |%-1s |%-1s |   |%1s|%1s|%1s|%1s|\n\0";

    #define znak_pik  "\033[1;34m\u2660\033[0m\0"
    #define znak_karo  "\033[1;31m\u2666\033[0m\0"
    #define znak_skocko  "\033[1;33m\u263A\033[0m\0"
    #define znak_zvezda  "\033[1;33m\u2605\033[0m\0"
    #define znak_tref  "\033[1;34m\u2663\033[0m\0"
    #define znak_herc  "\033[1;31m\u2665\033[0m\0"
    #define znak_crveni  "\033[1;31mO\033[0m\0"
    #define znak_zuti  "\033[1;33mO\033[0m\0"
    #define znak_prazan  "\0\033[0m\0"

    char * znakovi_za_ispis[9] = {znak_skocko, znak_tref, znak_pik, znak_herc, znak_karo, znak_zvezda};

    enum Znak trazena_kombinacija[VELICINA_KOMBINACIJE] = {0};

    enum Znak unesena_kombinacija[VELICINA_KOMBINACIJE] = {0};

    enum Znak histogram_unesene_kombinacije[6];
    enum Znak histogram_trazene_kombinacije[6];

    char* znakovi_za_tablu[BROJ_ZNAKOVA_TABLA];
    
    int tacna_pozicija = 0;
    int netacna_pozicija = 0 ;

    int broj_unetih_znakova = 0;

    int trenutni_znak = 0;

    void isprazni_tablu(){
      int i;
      for(i = 0; i< BROJ_ZNAKOVA_TABLA; ++i){
        znakovi_za_tablu[i] = znak_prazan;
      }
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
      }
    }


    void nova_igra(){
        trenutni_znak = 0;
        broj_unetih_znakova = 0;
        tacna_pozicija = 0;
        netacna_pozicija = 0;
        isprazni_tablu();
        napravi_random_trazenu_kombinaciju();
        napravi_histogram(trazena_kombinacija, histogram_trazene_kombinacije);
        printf(tabla, tabla_args(znakovi_za_tablu));
    }


%}


%union {
    int i;
    char *s;
}


%token _ENTER
%token _NOVA_IGRA
%token _KRAJ
%token <i> _ZNAK

%%

program
    : znakovi_pre_pocetka igra _KRAJ
    {
      YYACCEPT;
    }
    ; 

znakovi_pre_pocetka
    : /* empty */
    | znakovi_pre_pocetka _KRAJ
    {
      YYACCEPT;
    }
    | znakovi_pre_pocetka _ZNAK
    {
      printf("\nЗапочните нову игру командом \"ЗАПОЧНИ\" или \"НОВА ИГРА\".\n");
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
    :
    | kombinacija
    ;

kombinacija
    : _ZNAK 
    {
      unesena_kombinacija[trenutni_znak++] = $1;
      if ((trenutni_znak + 1)%5 == 0)
      {
        trenutni_znak = 0;
        odigraj_kombinaciju();
      }
      
    }
    | kombinacija _ZNAK
    {
      unesena_kombinacija[trenutni_znak++] = $2;
      if ((trenutni_znak + 1)%5 == 0)
      {
        trenutni_znak = 0;
        odigraj_kombinaciju();
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

    srand(time(0));

    int synerr;
    init_symtab();
    output = fopen("output.asm", "w+");

    synerr = yyparse();
    printf("\nДовиђења!\n");


    clear_symtab();
    fclose(output);


    return 0;
}