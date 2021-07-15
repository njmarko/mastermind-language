    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h> 
    #include "defs.h"
    #include "codegen.h"
    #include "скочко_логика.h"

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

    void kompajliraj_izlaz(){
          // char * komanda = "gcc igra.S -m32 -o igra";
          char * komanda = " -m32 -o igra";
          char buff[1000];
          snprintf(buff, sizeof(buff), "gcc igra%d.S%s%d", broj_partije, komanda, broj_partije);
          system(buff);
    }

    void napravi_zerobyte_fajl(){
          char * komanda = "./zeroBytes igra";
          char * mod = " encode";
          char buff[1000];
          snprintf(buff, sizeof(buff), "%s%d.S%s", komanda, broj_partije, mod);
          system(buff);
          // obrisi fajl na osnovu koga je napravljen zerobytes fajl
          char rm_command_buff[1000];
          snprintf(rm_command_buff, sizeof(rm_command_buff), "rm igra%d.S", broj_partije);
          system(rm_command_buff);
    }

    void zavrsi_upis_u_fajl(){
        if (!igra_zavrsena)
        {
          generisi_kraj(broj_unetih_znakova/(2*VELICINA_KOMBINACIJE));
          fclose(output);
        //   kompajliraj_izlaz();
          napravi_zerobyte_fajl();
        }
        igra_zavrsena = FALSE;
    }

    void nova_igra(){
        zavrsi_upis_u_fajl();
        broj_partije++;
        char buff[1000];
        char * ime_fajla = "igra";
        snprintf(buff, sizeof(buff), "%s%d.S", ime_fajla, broj_partije);
        output = fopen(buff, "w+");
        
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
        //   kompajliraj_izlaz();
          napravi_zerobyte_fajl();
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

