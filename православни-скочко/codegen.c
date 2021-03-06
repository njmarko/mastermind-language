#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"
#include "defs.h"
#include "math.h"

extern FILE *output;

int enkodiraj_kombinaciju(enum Znak *kombinacija)
{

    int i;
    int enkodirana_kombinacija = 0;
    for (i = 0; i < VELICINA_KOMBINACIJE; ++i)
    {
        enkodirana_kombinacija = enkodirana_kombinacija << 8 ^ (0x80 >> kombinacija[i]);
    }
    return enkodirana_kombinacija;
}

void generisi_data_sekciju()
{
    code(".section .data");
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
    code("\nmulti_line_display: .ascii \"%s\"", "\\n| %-1s  | %-1s  | %-1s  | %-1s  |   |%-1s |%-1s |%-1s |%-1s |\\n| %-1s  | %-1s  | %-1s  | %-1s  |   |%-1s |%-1s |%-1s |%-1s |\\n| %-1s  | %-1s  | %-1s  | %-1s  |   |%-1s |%-1s |%-1s |%-1s |\\n| %-1s  | %-1s  | %-1s  | %-1s  |   |%-1s |%-1s |%-1s |%-1s |\\n| %-1s  | %-1s  | %-1s  | %-1s  |   |%-1s |%-1s |%-1s |%-1s |\\n| %-1s  | %-1s  | %-1s  | %-1s  |   |%-1s |%-1s |%-1s |%-1s |\\n\\0");
    code("\none_line_display: .ascii \"%s\"", "\\n| %-1s  | %-1s  | %-1s  | %-1s  |   |%-1s |%-1s |%-1s |%-1s |\\n\\0");

    code("\nbrojac_stack: .long 48");
    code("\nznak_skocko: .ascii \"%s\"", "\\033[1;33m\\xE2\\x98\\xBA\\033[0m\\0");
    code("\nznak_tref: .ascii \"%s\"", "\\033[1;34m\\xE2\\x99\\xA3\\033[0m\\0");
    code("\nznak_pik: .ascii \"%s\"", "\\033[1;34m\\xE2\\x99\\xA0\\033[0m\\0");
    code("\nznak_herc: .ascii \"%s\"", "\\033[1;31m\\xE2\\x99\\xA5\\033[0m\\0");
    code("\nznak_karo: .ascii \"%s\"", "\\033[1;31m\\xE2\\x99\\xA6\\033[0m\\0");
    code("\nznak_zvezda: .ascii \"%s\"", "\\033[1;33m\\xE2\\x98\\x85\\033[0m\\0");
    code("\nznak_crveni: .ascii \"%s\"", "\\033[1;31m\\xE2\\x97\\x8F\\033[0m\\0");
    code("\nznak_zuti: .ascii \"%s\"", "\\033[1;33m\\xE2\\x97\\x8F\\033[0m\\0");
    code("\nznak_prazan: .ascii \"%s\"", "\\0\\033[0m\\0");
    code("\nporuka_prazan: .ascii \"%s\"", "\\n\\033[1;32mОдустали сте од игре. Довиђења!\\033[0m\\n\\0");
    code("\nporuka_prazan_len = .-poruka_prazan");
    code("\nporuka_pogresan_input: .ascii \"%s\"", "Погрешно унета комбинација! (унесите 4 броја без размака, нпр. 1122)\\n\\0");
    code("\nporuka_pogresan_input_len = .-poruka_pogresan_input");
    code("\nporuka_pogodjena_ispis: .ascii \"%s\"", "\\n\\033[1;32mЧЕСТИТАМО! Пронашли сте тражену комбинацију!\\033[0m\\n\\0");
    code("\nporuka_pogodjena_len = .-poruka_pogodjena_ispis");
    code("\nporuka_promasena_ispis: .ascii \"%s\"", "\\n\\033[1;32mНажалост нисте успели да пронађете тражену комбинацију.\\n\\033[0m| %-1s  | %-1s  | %-1s  | %-1s  |\\033[1;32m је тражена комбинација.\\033[0m\\n\\0");
    code("\nporuka_promasena_len = .-poruka_promasena_ispis");
    code("\nclear: .ascii \"%s\"", "\\033[2J\\0");
    code("\nclear_len = .-clear");
    code("\nclr: .ascii \"clear\"");
}

void generisi_pocetak_text_sekcije()
{
    code("\n.section .text");
    code("\n.globl main");
}

void generisi_pomocne_funkcije()
{

    code("\n%s", "#popunjava niz sa svim kombinacijama");
    code("\n%s", "#koristi: %eax, %ebx, %ecx, %edx, %esi, %edi");
    code("\n%s", "napravi_kombinacije:");
    code("\n%s", "	xorl %eax, %eax");
    code("\n%s", "	xorl %ebx, %ebx");
    code("\n%s", "	xorl %ecx, %ecx	");
    code("\n%s", "	xorl %edx, %edx");
    code("\n%s", "	movb $0b00000100, %bh #maska za brojanje do 6");
    code("\n%s", "	movb $0b10000000, %ah #pocetna maska");
    code("\n%s", "	movb $1, %cl #broj za shiftovanje");
    code("\n%s", "	movb %ah, %al ");
    code("\n%s", "	movb %ah, %bl");
    code("\n%s", "	movb %ah, %ch");
    code("\n%s", "	movb %ah, %dl");
    code("\n%s", "	leal sve_kombinacije, %esi");
    code("\n%s", "	xorl %edi, %edi");
    code("\n%s", "komb_petlja1:");
    code("\n%s", "	cmpb %bh, %al");
    code("\n%s", "	jb komb_povratak");
    code("\n%s", "	movb %ah, %bl");
    code("\n%s", "komb_petlja2:");
    code("\n%s", "	cmpb %bh, %bl");
    code("\n%s", "	jb komb_brojac1");
    code("\n%s", "	movb %ah, %ch");
    code("\n%s", "komb_petlja3:");
    code("\n%s", "	cmpb %bh, %ch");
    code("\n%s", "	jb komb_brojac2");
    code("\n%s", "	movb %ah, %dl");
    code("\n%s", "komb_petlja4:");
    code("\n%s", "	cmpb %bh, %dl");
    code("\n%s", "	jb komb_brojac3");
    code("\n%s", "	movb %al, (%esi,%edi,4)");
    code("\n%s", "	movb %bl, 1(%esi,%edi,4)");
    code("\n%s", "	movb %ch, 2(%esi,%edi,4)");
    code("\n%s", "	movb %dl, 3(%esi,%edi,4)");
    code("\n%s", "	incl %edi");
    code("\n%s", "	shrb %cl, %dl");
    code("\n%s", "	jmp komb_petlja4");
    code("\n%s", "komb_brojac3:");
    code("\n%s", "	shrb %cl, %ch");
    code("\n%s", "	jmp komb_petlja3");
    code("\n%s", "komb_brojac2:");
    code("\n%s", "	shrb %cl, %bl");
    code("\n%s", "	jmp komb_petlja2");
    code("\n%s", "komb_brojac1:");
    code("\n%s", "	shrb %cl, %al");
    code("\n%s", "	jmp komb_petlja1");
    code("\n%s", "komb_povratak:");
    code("\n%s", "	ret");
    code("\n%s", "");
    code("\n%s", "#racuna koliko ima znakova na tacnim mestima(crvenih) i koliko znakova nije na pravom mestu");
    code("\n%s", "#koristi: %eax(prva komb), %ebx(druga komb), %ecx, %edx, %esi(ret:crveni), %edi(ret:zuti)");
    code("\n%s", "histogram:");
    code("\n%s", "	pushl %eax");
    code("\n%s", "	pushl %ebx");
    code("\n%s", "	pushl %ecx");
    code("\n%s", "	pushl %edx");
    code("\n%s", "	xorl %esi, %esi");
    code("\n%s", "	movl %ebx, %edx");
    code("\n%s", "	andl %eax, %edx");
    code("\n%s", "	movl %edx, %ecx");
    code("\n%s", "	jz proveri_zute");
    code("\n%s", "petlja:");
    code("\n%s", "	cmpb $0, %dl");
    code("\n%s", "	jne brojac");
    code("\n%s", "	cmpl $0, %edx");
    code("\n%s", "	je proveri_zute");
    code("\n%s", "	shrl $8, %edx");
    code("\n%s", "	jmp petlja");
    code("\n%s", "brojac:");
    code("\n%s", "	shrl $8, %edx");
    code("\n%s", "	incl %esi");
    code("\n%s", "	jmp petlja");
    code("\n%s", "proveri_zute:");
    code("\n%s", "	xorl %ecx, %ebx");
    code("\n%s", "	xorl %ecx, %eax");
    code("\n%s", "	xorl %edi, %edi");
    code("\n%s", "petlja2:");
    code("\n%s", "	cmpb $0, %bl");
    code("\n%s", "	je brojac2");
    code("\n%s", "	movl $4, %edx");
    code("\n%s", "petlja3:");
    code("\n%s", "	decl %edx");
    code("\n%s", "	js brojac2");
    code("\n%s", "	testb %bl, %al");
    code("\n%s", "	jz brojac3");
    code("\n%s", "	xorb %bl, %al");
    code("\n%s", "	incl %edi");
    code("\n%s", "	jmp brojac2");
    code("\n%s", "brojac3:");
    code("\n%s", "	rorl $8, %eax");
    code("\n%s", "	jmp petlja3");
    code("\n%s", "brojac2:");
    code("\n%s", "	shrl $8, %ebx");
    code("\n%s", "	jnz petlja2");
    code("\n%s", "povratak:");
    code("\n%s", "	popl %edx");
    code("\n%s", "	popl %ecx");
    code("\n%s", "	popl %ebx");
    code("\n%s", "	popl %eax");
    code("\n%s", "	ret");
    code("\n%s", "#zapisuje preostale moguce kombinacije u niz preostale_kombinacije, i pamti njihov broj u %ebx");
    code("\n%s", "#koristi ciljanu kombinaciju u %eax, registre %ebx(trenutna kombinacija iz niza),%ecx(brojac),  %edx(broj pronadjenih kombinacija)");
    code("\n%s", "izracunaj_preostale_1:");
    code("\n%s", "	movl $0b10000000100000000100000001000000, %eax");
    code("\n%s", "	xorl %ebx, %ebx");
    code("\n%s", "	xorl %ecx, %ecx");
    code("\n%s", "	xorl %edx, %edx");
    code("\n%s", "preostali_petlja1:");
    code("\n%s", "	cmpl broj_preostalih, %ecx");
    code("\n%s", "	je preostali_povratak1");
    code("\n%s", "	movl sve_kombinacije(,%ecx,4), %ebx");
    code("\n%s", "	cmpl $0, %ebx");
    code("\n%s", "	je preostali_povratak1");
    code("\n%s", "	call histogram");
    code("\n%s", "	cmpl crveni, %esi");
    code("\n%s", "	jne preostali_brojac1");
    code("\n%s", "	cmpl zuti, %edi");
    code("\n%s", "	jne preostali_brojac1");
    code("\n%s", "	movl %ebx, preostale_kombinacije(,%edx,4)");
    code("\n%s", "	incl %edx");
    code("\n%s", "preostali_brojac1:");
    code("\n%s", "	incl %ecx");
    code("\n%s", "	jmp preostali_petlja1");
    code("\n%s", "preostali_povratak1:");
    code("\n%s", "	movl %edx, broj_preostalih");
    code("\n%s", "	ret");
    code("\n%s", "");
    code("\n%s", "#zapisuje preostale moguce kombinacije u niz preostale_kombinacije, i pamti njihov broj u %ebx");
    code("\n%s", "#koristi ciljanu kombinaciju u %eax, registre %ebx(trenutna kombinacija iz niza),%ecx(brojac),  %edx(broj pronadjenih kombinacija)");
    code("\n%s", "izracunaj_preostale:");
    code("\n%s", "	movl $0b10000000100000000100000001000000, %eax");
    code("\n%s", "	xorl %ebx, %ebx");
    code("\n%s", "	xorl %ecx, %ecx");
    code("\n%s", "	xorl %edx, %edx");
    code("\n%s", "preostali_petlja:");
    code("\n%s", "	cmpl broj_preostalih, %ecx");
    code("\n%s", "	je preostali_povratak");
    code("\n%s", "	movl preostale_kombinacije(,%ecx,4), %ebx #jedina razlika");
    code("\n%s", "	cmpl $0, %ebx");
    code("\n%s", "	je preostali_povratak");
    code("\n%s", "	call histogram");
    code("\n%s", "	cmpl crveni, %esi");
    code("\n%s", "	jne preostali_brojac");
    code("\n%s", "	cmpl zuti, %edi");
    code("\n%s", "	jne preostali_brojac");
    code("\n%s", "	movl %ebx, preostale_kombinacije(,%edx,4)");
    code("\n%s", "	incl %edx");
    code("\n%s", "preostali_brojac:");
    code("\n%s", "	incl %ecx");
    code("\n%s", "	jmp preostali_petlja");
    code("\n%s", "preostali_povratak:");
    code("\n%s", "	movl %edx, broj_preostalih");
    code("\n%s", "	ret");
    code("\n%s", "");
    code("\n%s", "generisi_trazenu_kombinaciju:");
    code("\n%s", "	pushl $0");
    code("\n%s", "	call time #time(null)");
    code("\n%s", "	addl $4, %esp");
    code("\n%s", "	pushl %eax");
    code("\n%s", "	call srand #srand(time(null)) postavlja seed koji ce rand() da koristi");
    code("\n%s", "	addl $4, %esp");
    code("\n%s", "	movl $0b10000000100000001000000010000000, %ebx");
    code("\n%s", "	movl $5, %edi");
    code("\n%s", "	movl $6, %esi");
    code("\n%s", "petlja_random:");
    code("\n%s", "	decl %edi");
    code("\n%s", "	jz povratak_random");
    code("\n%s", "	call rand #generise pseudorandom broj");
    code("\n%s", "	xorl %edx, %edx");
    code("\n%s", "	divl delioc #ostatak u %edx");
    code("\n%s", "	movb %dl, %cl");
    code("\n%s", "	rorb %cl,%bl");
    code("\n%s", "	rorl $8, %ebx");
    code("\n%s", "	jmp petlja_random");
    code("\n%s", "povratak_random:");
    code("\n%s", "	movl %ebx, trazena_kombinacija");
    code("\n%s", "	ret");
    code("\n%s", "");
    code("\n%s", "pp_pronadji_sledeceg:");
    code("\n%s", "	cmpb $' ', (%edx)");
    code("\n%s", "	jne povratak_pronadji_sledeceg");
    code("\n%s", "	incl %edx");
    code("\n%s", "	jmp pp_pronadji_sledeceg");
    code("\n%s", "povratak_pronadji_sledeceg:ret");
    code("\n%s", "");
    code("\n%s", "prazan_string: #kod unosa praznog stringa se igra prekida");
    code("\n%s", "	movl $4, %eax");
    code("\n%s", "	movl $1, %ebx");
    code("\n%s", "	leal poruka_prazan, %ecx");
    code("\n%s", "	movl $poruka_prazan_len, %edx");
    code("\n%s", "	int $0x80");
    code("\n%s", "	jmp kraj");
    code("\n%s", "pogresan_input:");
    code("\n%s", "	movl $4, %eax");
    code("\n%s", "	movl $1, %ebx");
    code("\n%s", "	leal poruka_pogresan_input, %ecx");
    code("\n%s", "	movl $poruka_pogresan_input_len, %edx");
    code("\n%s", "	int $0x80");
    code("\n%s", "	jmp input_kombinacija");
    code("\n%s", "	");
    code("\n%s", "input_kombinacija:");
    code("\n%s", "	movl $4, %eax");
    code("\n%s", "	movl $1, %ebx");
    code("\n%s", "	leal znakovi, %ecx");
    code("\n%s", "	movl $znakovi_len, %edx");
    code("\n%s", "	int $0x80");
    code("\n%s", "");
    code("\n%s", "	movl $3, %eax");
    code("\n%s", "	movl $0, %ebx");
    code("\n%s", "	leal unesena_komb, %ecx");
    code("\n%s", "	movl $100, %edx");
    code("\n%s", "	int $0x80");
    code("\n%s", "");
    code("\n%s", "	cmpl $1, %eax");
    code("\n%s", "	je prazan_string #prekid igre");
    code("\n%s", "	leal unesena_komb, %edx");
    code("\n%s", "	call pp_pronadji_sledeceg");
    code("\n%s", "	xorl %ebx, %ebx");
    code("\n%s", "petlja_input:");
    code("\n%s", "	cmpl $4, %ebx");
    code("\n%s", "	je povratak_input");
    code("\n%s", "	shll $8, pokusaj");
    code("\n%s", "	movb (%edx,%ebx,1), %cl");
    code("\n%s", "	cmpb $'1', %cl");
    code("\n%s", "	jb pogresan_input");
    code("\n%s", "	cmpb $'6', %cl");
    code("\n%s", "	ja pogresan_input");
    code("\n%s", "	subb $'1', %cl #oduzimam 1 posto se u slucaju jedinice ne rotira maska");
    code("\n%s", "	movb $0b10000000, %ah #pocetna maska");
    code("\n%s", "	rorb %cl, %ah");
    code("\n%s", "	xorb %ah, pokusaj");
    code("\n%s", "brojac_input:");
    code("\n%s", "	incl %ebx");
    code("\n%s", "	jmp petlja_input");
    code("\n%s", "povratak_input:	ret");
}

void generisi_unetu_kombinaciju(enum Znak *kombinacija, int redni_broj)
{
    code("\n#-------celina za unos i racunanje");

    // unosim kombinaciju direktno u pokusaj
    code("\n\tmovl $%d, %%eax", enkodiraj_kombinaciju(kombinacija));
    code("\n\tmovl %%eax, pokusaj");

    code("\n	movl pokusaj, %%eax");
    code("\n	movl trazena_kombinacija, %%ebx");
    code("\n	call histogram");
    code("\n	movl %%esi, crveni");
    code("\n	movl %%edi, zuti");
    code("\n	call izracunaj_preostale_1");
    code("\n	cmpl $4, crveni");
    code("\n	jne ispis%d", redni_broj);
    code("\n	movl $1, pogodjena_kombinacija");
    code("\n");
    code("\n#-------celina za ispis");
    code("\nispis%d:", redni_broj);

    // za prvi ispis
    if (redni_broj > 1)
    {
        code("\n\tmovl $%d, brojac_stack #novi red pocinje odatle", 68 + 32 * (redni_broj - 2));
    }

    code("\n	addl brojac_stack, %%esp #vracanje stack pointera na prvi sledeci koji nije prazan");
    code("\n	#racunanje praznih znakova");
    code("\n	movl $4, %%eax");
    code("\n	subl zuti, %%eax");
    code("\n	subl crveni, %%eax");
    code("\n	movl $4, %%ebx");
    code("\n	mull %%ebx");
    code("\n	subl %%eax, %%esp #ovako se ostavljaju prazna mesta na stacku za prikaz praznih mesta");
    code("\npetlja_zuti%d:", redni_broj);
    code("\n	decl zuti");
    code("\n	js petlja_crveni%d", redni_broj);
    code("\n	pushl $znak_zuti");
    code("\n	jmp petlja_zuti%d", redni_broj);
    code("\npetlja_crveni%d:", redni_broj);
    code("\n	decl crveni");
    code("\n	js kombinacija%d", redni_broj);
    code("\n	pushl $znak_crveni");
    code("\n	jmp petlja_crveni%d", redni_broj);
    code("\nkombinacija%d:", redni_broj);
    code("\n	movl $4, %%eax #brojac");
    code("\npetlja_komb%d:", redni_broj);
    code("\n	decl %%eax");

    // skok na sledecu kombinaciju
    code("\n	js kombinacija_broj%d", redni_broj + 1);

    code("\n	cmpb $0b01000000, pokusaj");
    code("\n	ja upisi_skocko%d", redni_broj);
    code("\n	je upisi_tref%d", redni_broj);
    code("\n	cmpb $0b00010000, pokusaj");
    code("\n	ja upisi_pik%d", redni_broj);
    code("\n	je upisi_herc%d", redni_broj);
    code("\n	cmpb $0b00000100, pokusaj");
    code("\n	ja upisi_karo%d", redni_broj);
    code("\nupisi_zvezda%d:", redni_broj);
    code("\n	pushl $znak_zvezda");
    code("\n	jmp brojac_petlja_komb%d", redni_broj);
    code("\nupisi_skocko%d:", redni_broj);
    code("\n	pushl $znak_skocko");
    code("\n	jmp brojac_petlja_komb%d", redni_broj);
    code("\nupisi_tref%d:", redni_broj);
    code("\n	pushl $znak_tref");
    code("\n	jmp brojac_petlja_komb%d", redni_broj);
    code("\nupisi_pik%d:", redni_broj);
    code("\n	pushl $znak_pik");
    code("\n	jmp brojac_petlja_komb%d", redni_broj);
    code("\nupisi_herc%d:", redni_broj);
    code("\n	pushl $znak_herc");
    code("\n	jmp brojac_petlja_komb%d", redni_broj);
    code("\nupisi_karo%d:", redni_broj);
    code("\n	pushl $znak_karo");
    code("\nbrojac_petlja_komb%d:", redni_broj);
    code("\n	shrl $8, pokusaj");
    code("\n	jmp petlja_komb%d", redni_broj);
    code("\n\nkombinacija_broj%d:", redni_broj + 1);
    code("\n\tsubl $32, brojac_stack #posto je pushovano 8 argumenata");
    code("\n\tsubl brojac_stack, %%esp #vraca se stack pointer na vrh stack-a");
    // kod za cekanje za ispis sledece kombinacije
    code("\n\tpushl $2");
    code("\n\tcall sleep");
    code("\n\taddl $4, %%esp");

    // kod za ciscenje ekrana
    code("\n\tpushl $clr");
    code("\n\tcall system");
    code("\n\taddl $4, %%esp");

    code("\n\tcall printf");
    code("\n\tcmpl $1, pogodjena_kombinacija");
    code("\n\tje poruka_pogodjena");
}

void generisi_trazenu_kombinaciju(enum Znak *kombinacija)
{
    code("\nmain:");
    code("\n\tpushl $clr");
    code("\n\tcall system");
    code("\n\taddl $4, %%esp");
    code("\n	movl $znak_prazan, %%eax #nula za stack");
    code("\n	movl brojac_stack, %%ebx #brojac za stack br.48");
    code("\npripremi_stack: #gura 48 praznih stringova na stack zbog nacina koriscenja printf funkcije");
    code("\n	pushl %%eax");
    code("\n	decl %%ebx");
    code("\n	jnz pripremi_stack");
    code("\n	pushl $multi_line_display");
    code("\n	call printf");
    code("\n	movl $36, brojac_stack #postavlja se vrednost brojaca stack pomnozena sa 4 (velicina long)");
    code("\n");
    code("\n	call napravi_kombinacije");

    // generisem trazenu kombinaciju direktno umesto random
    code("\n\tmovl $%d, %%ebx", enkodiraj_kombinaciju(kombinacija));
    code("\n\tmovl %%ebx, trazena_kombinacija");
}

void generisi_kraj(int redni_broj)
{
    // max koji dospe ovde je 6 pa ga uvecam da bude 7
    redni_broj++;

    code("\nporuka_promasena:");
    code("\n	movl $4, %%eax #brojac");
    code("\npetlja_komb%d:", redni_broj);
    code("\n	decl %%eax");
    code("\n	js ispisi_pogresnu_kombinaciju");
    code("\n	cmpb $0b01000000, trazena_kombinacija");
    code("\n	ja upisi_skocko%d", redni_broj);
    code("\n	je upisi_tref%d", redni_broj);
    code("\n	cmpb $0b00010000, trazena_kombinacija");
    code("\n	ja upisi_pik%d", redni_broj);
    code("\n	je upisi_herc%d", redni_broj);
    code("\n	cmpb $0b00000100, trazena_kombinacija");
    code("\n	ja upisi_karo%d", redni_broj);
    code("\nupisi_zvezda%d:", redni_broj);
    code("\n	pushl $znak_zvezda");
    code("\n	jmp brojac_petlja_komb%d", redni_broj);
    code("\nupisi_skocko%d:", redni_broj);
    code("\n	pushl $znak_skocko");
    code("\n	jmp brojac_petlja_komb%d", redni_broj);
    code("\nupisi_tref%d:", redni_broj);
    code("\n	pushl $znak_tref");
    code("\n	jmp brojac_petlja_komb%d", redni_broj);
    code("\nupisi_pik%d:", redni_broj);
    code("\n	pushl $znak_pik");
    code("\n	jmp brojac_petlja_komb%d", redni_broj);
    code("\nupisi_herc%d:", redni_broj);
    code("\n	pushl $znak_herc");
    code("\n	jmp brojac_petlja_komb%d", redni_broj);
    code("\nupisi_karo%d:", redni_broj);
    code("\n	pushl $znak_karo");
    code("\nbrojac_petlja_komb%d:", redni_broj);
    code("\n	shrl $8, trazena_kombinacija");
    code("\n	jmp petlja_komb%d", redni_broj);
    code("\nispisi_pogresnu_kombinaciju:");
    code("\n	pushl $poruka_promasena_ispis");
    code("\n	call printf");
    code("\n	jmp kraj");

    code("\nporuka_pogodjena:");
    code("\n	movl $4, %%eax");
    code("\n	movl $1, %%ebx");
    code("\n	leal poruka_pogodjena_ispis, %%ecx");
    code("\n	movl $poruka_pogodjena_len, %%edx ");
    code("\n	int $0x80");
    code("\n	jmp kraj");
    code("\n	");
    code("\nkraj:");
    code("\n	movl $1, %%eax");
    code("\n	movl $0, %%ebx");
    code("\n	int $0x80");
}
