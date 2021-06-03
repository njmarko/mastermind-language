#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"
#include "symtab.h"

extern FILE *output;

void generisi_data_sekciju(){
    code("\n.section .data"); 
    code("\ncrveni: .long 0");
    code("\nzuti: .long 0  ");
    code("\nsve_kombinacije: .fill 1296,4,0");
    code("\npreostale_kombinacije: .fill 1296,4,0");
    code("\nbroj_preostalih: .long 1296");
    code("\ntrazena_kombinacija: .long 0");
    code("\ndelioc: .long 6");
    code("\nznakovi: .ascii \"\\n\\033[1;32mСКОЧКО[1] ТРЕФ[2] ПИК[3] ХЕРЦ[4] КАРО[5] ЗВЕЗДА[6] (КРАЈ за излазак)\\033[0m\\n\\0\"");
    code("\nznakovi_len = .-znakovi");
    code("\nreset: .ascii \"\\033[0m\\n\"");
    code("\ngreen: .ascii \"\\033[1;32m\"");
    code("\nunesena_komb: .fill 100,1,0");
    code("\npokusaj: .long 0");
    code("\npogodjena_kombinacija: .long 0");
}