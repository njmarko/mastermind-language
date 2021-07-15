# bash je potreban da bi radila redirekcija stderr i stdout, kao i zbog PIPESTATUS promenljive
SHELL = /bin/bash
# naziv kompajlera i osnovnih source fajlova
COMP = $(wildcard *.l)
SRC = $(basename $(COMP))
# da li je kompajler sa generisanjem koda?
ifeq ($(SRC),скочко)
    ASM = true
    CGENC = codegen.c
    CGENH = codegen.h
endif
# fajlovi od kojih se sastoji kompajler
COMPILER_BUILD = lex.yy.c $(SRC).tab.c скочко_логика.c $(CGENC)
# fajlovi od kojih zavisi ponovno prevođenje
COMPILER_DEPENDS = $(COMPILER_BUILD) defs.h скочко_логика.h $(CGENH)
# fajlovi koje treba pobrisati da bi ostao samo izvorni kod
COMPILER_CLEAN = lex.yy.c $(SRC).tab.c $(SRC).tab.h $(SRC).output $(SRC) *.?~ *.mc~ .make.out* *.asm *.S Makefile~ *.txt~
# ako treba pokrenuti replay uneti ime samo ime fajl bez .S ekstenzije
REPLAY = ""

# pravila koja ne generišu nove fajlove
.PHONY: clean archive help

# osnovno pravilo za prevođenje kompajlera
$(SRC): $(COMPILER_DEPENDS)
	@echo -e "\e[01;32mGCC...\e[00m"
	@-rm -f $(SRC) .make.out2 2>/dev/null
	@g++ main.cpp ZeroBytesConversion.cpp -m32 -lstdc++fs -o zeroBytes
	@gcc -o $@ $(COMPILER_BUILD) 2>&1 | tee .make.outg; pstat=$${PIPESTATUS[0]}; \
	cat .make.outf .make.outb .make.outg > .make.out 2>/dev/null; \
	out=`grep -Ei "conflict|warning|error" .make.out 2>/dev/null`; \
	if [ "$$out" != "" ]; then \
		cat .make.outf .make.outb; \
		echo -e "\e[01;31m\nThere are errors/warnings in grammar/actions!\n\e[00m" ; \
		rm -f $(SRC) 2>/dev/null ; \
		exit 1; \
	fi; \
	exit $$pstat

lex.yy.c: $(SRC).l $(SRC).tab.c
	@echo -e "\e[01;32mFLEX...\e[00m"
	@flex $< 2>&1 | tee .make.outf; exit $${PIPESTATUS[0]}

$(SRC).tab.c: $(SRC).y
	@echo -e "\e[01;32mBISON...\e[00m"
	@bison -d -v $< 2>&1 | tee .make.outb; exit $${PIPESTATUS[0]}

clean:
	@echo -e "\e[01;32mDeleting temporary files...\e[00m"
	@rm -f $(COMPILER_CLEAN)
	@rm -f hipsim
	@if [ -f ./hipsim-src/Makefile ]; then \
	    make --silent -C ./hipsim-src/ clean ; \
	fi

archive: clean
	@arc=`pwd`; \
	arc=`basename "$$arc"`; \
	echo -e "\e[01;32mCreating archive ../$$arc.tar.gz\e[00m"; \
	tar --exclude=*.gz -czf ../$$arc.tar.gz ../$$arc

replay: $(SRC)
	@if [ ! -f $(SRC) ]; then exit 1; fi
	@./zeroBytes $$REPLAY.S decode;
	@gcc -m32 recreatedFile/$$REPLAY.S -o recreatedFile/$$REPLAY.out;
	@./recreatedFile/$$REPLAY.out;

help:
	@echo -e "\n\e[01;36mKompajliranje programa\e[00m"
	@echo -e "    \e[01;32mmake\e[00m"
	@echo -e "\n\e[01;36mBrisanje automatski generisanih fajlova\e[00m"
	@echo -e "    \e[01;32mmake clean\e[00m"
	@echo -e "\n\e[01;36mPokretanje replay-a. Treba uneti ime fajla bez .S ekstenzije.\nKada se ova komanda unese generisaće se izvršni fajl koji će biti i pokrenut\nTako će biti prikazan ponovljeni snimak te partije\e[00m"
	@echo -e "    \e[01;32mmake replay REPLAY=igra1\e[00m"

