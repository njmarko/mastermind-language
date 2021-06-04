%{
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "defs.h"
#include "codegen.h"
#include "скочко_логика.h"

  int yyparse(void);
  int yylex(void);
  int yyerror(char *s);
  void warning(char *s);

  extern int yylineno;
  int out_lin = 0;
  char char_buffer[CHAR_BUFFER_LENGTH];
  int error_count = 0;
  int warning_count = 0;

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

    printf("%s", poruka_pocetak_partije);
    synerr = yyparse();
    printf("%s", poruka_kraj_programa);

    return 0;
}