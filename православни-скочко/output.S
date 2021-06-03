
.section .data
crveni: .long 0
zuti: .long 0  
sve_kombinacije: .fill 1296,4,0
preostale_kombinacije: .fill 1296,4,0
broj_preostalih: .long 1296
trazena_kombinacija: .long 0
delioc: .long 6
znakovi: .ascii "\n\033[1;32mСКОЧКО[1] ТРЕФ[2] ПИК[3] ХЕРЦ[4] КАРО[5] ЗВЕЗДА[6] (КРАЈ за излазак)\033[0m\n\0"
znakovi_len = .-znakovi
reset: .ascii "\033[0m\n"
green: .ascii "\033[1;32m"
unesena_komb: .fill 100,1,0
pokusaj: .long 0
pogodjena_kombinacija: .long 0