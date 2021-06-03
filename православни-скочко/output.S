
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
pogodjena_kombinacija: .long 0multi_line_display: .ascii "\n| %-1s  | %-1s  | %-1s  | %-1s  |   |%-1s |%-1s |%-1s |%-1s |\n| %-1s  | %-1s  | %-1s  | %-1s  |   |%-1s |%-1s |%-1s |%-1s |\n| %-1s  | %-1s  | %-1s  | %-1s  |   |%-1s |%-1s |%-1s |%-1s |\n| %-1s  | %-1s  | %-1s  | %-1s  |   |%-1s |%-1s |%-1s |%-1s |\n| %-1s  | %-1s  | %-1s  | %-1s  |   |%-1s |%-1s |%-1s |%-1s |\n| %-1s  | %-1s  | %-1s  | %-1s  |   |%-1s |%-1s |%-1s |%-1s |\n\0"
#popunjava niz sa svim kombinacijama
#koristi: %eax, %ebx, %ecx, %edx, %esi, %edi
napravi_kombinacije:
	xorl %eax, %eax
	xorl %ebx, %ebx
	xorl %ecx, %ecx	
	xorl %edx, %edx
	movb $0b00000100, %bh #maska za brojanje do 6
	movb $0b10000000, %ah #pocetna maska
	movb $1, %cl #broj za shiftovanje
	movb %ah, %al 
	movb %ah, %bl
	movb %ah, %ch
	movb %ah, %dl
	leal sve_kombinacije, %esi
	xorl %edi, %edi
komb_petlja1:
	cmpb %bh, %al
	jb komb_povratak
	movb %ah, %bl
komb_petlja2:
	cmpb %bh, %bl
	jb komb_brojac1
	movb %ah, %ch
komb_petlja3:
	cmpb %bh, %ch
	jb komb_brojac2
	movb %ah, %dl
komb_petlja4:
	cmpb %bh, %dl
	jb komb_brojac3
	movb %al, (%esi,%edi,4)
	movb %bl, 1(%esi,%edi,4)
	movb %ch, 2(%esi,%edi,4)
	movb %dl, 3(%esi,%edi,4)
	incl %edi
	shrb %cl, %dl
	jmp komb_petlja4
komb_brojac3:
	shrb %cl, %ch
	jmp komb_petlja3
komb_brojac2:
	shrb %cl, %bl
	jmp komb_petlja2
komb_brojac1:
	shrb %cl, %al
	jmp komb_petlja1
komb_povratak:
	ret

#racuna koliko ima znakova na tacnim mestima(crvenih) i koliko znakova nije na pravom mestu
#koristi: %eax(prva komb), %ebx(druga komb), %ecx, %edx, %esi(ret:crveni), %edi(ret:zuti)
histogram:
	pushl %eax
	pushl %ebx
	pushl %ecx
	pushl %edx
	xorl %esi, %esi
	movl %ebx, %edx
	andl %eax, %edx
	movl %edx, %ecx
	jz proveri_zute
petlja:
	cmpb $0, %dl
	jne brojac
	cmpl $0, %edx
	je proveri_zute
	shrl $8, %edx
	jmp petlja
brojac:
	shrl $8, %edx
	incl %esi
	jmp petlja
proveri_zute:
	xorl %ecx, %ebx
	xorl %ecx, %eax
	xorl %edi, %edi
petlja2:
	cmpb $0, %bl
	je brojac2
	movl $4, %edx
petlja3:
	decl %edx
	js brojac2
	testb %bl, %al
	jz brojac3
	xorb %bl, %al
	incl %edi
	jmp brojac2
brojac3:
	rorl $8, %eax
	jmp petlja3
brojac2:
	shrl $8, %ebx
	jnz petlja2
povratak:
	popl %edx
	popl %ecx
	popl %ebx
	popl %eax
	ret
#zapisuje preostale moguce kombinacije u niz preostale_kombinacije, i pamti njihov broj u %ebx
#koristi ciljanu kombinaciju u %eax, registre %ebx(trenutna kombinacija iz niza),%ecx(brojac),  %edx(broj pronadjenih kombinacija)
izracunaj_preostale_1:
	movl $0b10000000100000000100000001000000, %eax
	xorl %ebx, %ebx
	xorl %ecx, %ecx
	xorl %edx, %edx
preostali_petlja1:
	cmpl broj_preostalih, %ecx
	je preostali_povratak1
	movl sve_kombinacije(,%ecx,4), %ebx
	cmpl $0, %ebx
	je preostali_povratak1
	call histogram
	cmpl crveni, %esi
	jne preostali_brojac1
	cmpl zuti, %edi
	jne preostali_brojac1
	movl %ebx, preostale_kombinacije(,%edx,4)
	incl %edx
preostali_brojac1:
	incl %ecx
	jmp preostali_petlja1
preostali_povratak1:
	movl %edx, broj_preostalih
	ret

#zapisuje preostale moguce kombinacije u niz preostale_kombinacije, i pamti njihov broj u %ebx
#koristi ciljanu kombinaciju u %eax, registre %ebx(trenutna kombinacija iz niza),%ecx(brojac),  %edx(broj pronadjenih kombinacija)
izracunaj_preostale:
	movl $0b10000000100000000100000001000000, %eax
	xorl %ebx, %ebx
	xorl %ecx, %ecx
	xorl %edx, %edx
preostali_petlja:
	cmpl broj_preostalih, %ecx
	je preostali_povratak
	movl preostale_kombinacije(,%ecx,4), %ebx #jedina razlika
	cmpl $0, %ebx
	je preostali_povratak
	call histogram
	cmpl crveni, %esi
	jne preostali_brojac
	cmpl zuti, %edi
	jne preostali_brojac
	movl %ebx, preostale_kombinacije(,%edx,4)
	incl %edx
preostali_brojac:
	incl %ecx
	jmp preostali_petlja
preostali_povratak:
	movl %edx, broj_preostalih
	ret

generisi_trazenu_kombinaciju:
	pushl $0
	call time #time(null)
	addl $4, %esp
	pushl %eax
	call srand #srand(time(null)) postavlja seed koji ce rand() da koristi
	addl $4, %esp
	movl $0b10000000100000001000000010000000, %ebx
	movl $5, %edi
	movl $6, %esi
petlja_random:
	decl %edi
	jz povratak_random
	call rand #generise pseudorandom broj
	xorl %edx, %edx
	divl delioc #ostatak u %edx
	movb %dl, %cl
	rorb %cl,%bl
	rorl $8, %ebx
	jmp petlja_random
povratak_random:
	movl %ebx, trazena_kombinacija
	ret

pp_pronadji_sledeceg:
	cmpb $' ', (%edx)
	jne povratak_pronadji_sledeceg
	incl %edx
	jmp pp_pronadji_sledeceg
povratak_pronadji_sledeceg:ret

prazan_string: #kod unosa praznog stringa se igra prekida
	movl $4, %eax
	movl $1, %ebx
	leal poruka_prazan, %ecx
	movl $poruka_prazan_len, %edx
	int $0x80
	jmp kraj
pogresan_input:
	movl $4, %eax
	movl $1, %ebx
	leal poruka_pogresan_input, %ecx
	movl $poruka_pogresan_input_len, %edx
	int $0x80
	jmp input_kombinacija
	
input_kombinacija:
	movl $4, %eax
	movl $1, %ebx
	leal znakovi, %ecx
	movl $znakovi_len, %edx
	int $0x80

	movl $3, %eax
	movl $0, %ebx
	leal unesena_komb, %ecx
	movl $100, %edx
	int $0x80

	cmpl $1, %eax
	je prazan_string #prekid igre
	leal unesena_komb, %edx
	call pp_pronadji_sledeceg
	xorl %ebx, %ebx
petlja_input:
	cmpl $4, %ebx
	je povratak_input
	shll $8, pokusaj
	movb (%edx,%ebx,1), %cl
	cmpb $'1', %cl
	jb pogresan_input
	cmpb $'6', %cl
	ja pogresan_input
	subb $'1', %cl #oduzimam 1 posto se u slucaju jedinice ne rotira maska
	movb $0b10000000, %ah #pocetna maska
	rorb %cl, %ah
	xorb %ah, pokusaj
brojac_input:
	incl %ebx
	jmp petlja_input
povratak_input:	ret