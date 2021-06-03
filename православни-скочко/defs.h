#ifndef DEFS_H
#define DEFS_H

#define bool int
#define TRUE  1
#define FALSE 0

#define SYMBOL_TABLE_LENGTH   64
#define NO_INDEX              -1
#define NO_ATR                 0
#define LAST_WORKING_REG      12
#define FUN_REG               13
#define CHAR_BUFFER_LENGTH   128
extern char char_buffer[CHAR_BUFFER_LENGTH];
enum Znak {SKOCKO, TREF, PIK, HERC, KARO, ZVEZDA};

extern int out_lin;
//pomocni makroi za ispis
extern void warning(char *s);
extern int yyerror(char *s);
#define err(args...)  sprintf(char_buffer, args), yyerror(char_buffer)
#define warn(args...) sprintf(char_buffer, args), warning(char_buffer)
#define code(args...) ({fprintf(output, args); \
          if (++out_lin > 2000) err("Too many output lines"), exit(1); })


//tipovi podataka
enum types { NO_TYPE, INT, UINT, VOID};

//vrste simbola (moze ih biti maksimalno 32)
enum kinds { NO_KIND = 0x1, REG = 0x2, LIT = 0x4, 
             FUN = 0x8, VAR = 0x10, PAR = 0x20, GVAR = 0x40, PVAR = 0x80};
             
//konstante arithmetickih operatora
enum arops { ADD, SUB, MUL, DIV, AROP_NUMBER };
//stringovi za generisanje aritmetickih naredbi
static char *ar_instructions[] = { "ADDS", "SUBS", "MULS", "DIVS",
                                   "ADDU", "SUBU", "MULU", "DIVU" };

//konstante relacionih operatora
enum relops { LT, GT, LE, GE, EQ, NE, RELOP_NUMBER };
//stringovi za JMP narebu
static char* jumps[]={"JLTS", "JGTS", "JLES", "JGES", "JEQ ", "JNE ",
                      "JLTU", "JGTU", "JLEU", "JGEU", "JEQ ", "JNE " };

static char* opp_jumps[]={"JGES", "JLES", "JGTS", "JLTS", "JNE ", "JEQ ",
                          "JGEU", "JLEU", "JGTU", "JLTU", "JNE ", "JEQ "};

// za igru skocko
static char * tabla = "\n| %-1s  | %-1s  | %-1s  | %-1s  |   |%-1s |%-1s |%-1s |%-1s |\n| %-1s  | %-1s  | %-1s  | %-1s  |   |%-1s |%-1s |%-1s |%-1s |\n| %-1s  | %-1s  | %-1s  | %-1s  |   |%-1s |%-1s |%-1s |%-1s |\n| %-1s  | %-1s  | %-1s  | %-1s  |   |%-1s |%-1s |%-1s |%-1s |\n| %-1s  | %-1s  | %-1s  | %-1s  |   |%-1s |%-1s |%-1s |%-1s |\n| %-1s  | %-1s  | %-1s  | %-1s  |   |%-1s |%-1s |%-1s |%-1s |\n\0";
static char * red = "\n| %-1s  | %-1s  | %-1s  | %-1s  |   |%-1s |%-1s |%-1s |%-1s |\n\0";
static char * ispravna_kombinacija_ispis = "\n\033[1;32mНажалост нисте успели да пронађете тражену комбинацију.\n\033[0m| %-1s  | %-1s  | %-1s  | %-1s  |\033[1;32m је тражена комбинација.\033[0m\n\0";
static char * poruka_unos = "\n\033[1;32mСКОЧКО[1] ТРЕФ[2] ПИК[3] ХЕРЦ[4] КАРО[5] ЗВЕЗДА[6] (КРАЈ за излазак)\033[0m\n\0";
static char * poruka_kombinacija_pogodjena = "\n\033[1;32mЧЕСТИТАМО! Пронашли сте тражену комбинацију!\033[0m\n\0";
static char * poruka_kraj_partije = "\n\033[1;32mИгра је завршена. Укуцајте команду \"НОВА ИГРА\" или \"КРАЈ\" за излазак.\033[0m\n\0";
static char * poruka_pocetak_partije = "\n\033[1;32mЗапочните нову игру командом \"ЗАПОЧНИ\" или \"НОВА ИГРА\".\033[0m\n\0";
static char * poruka_kraj_programa = "\n\033[1;32mОдустали сте од игре. Довиђења!\033[0m\n\0";
static char * poruka_broj_van_opsega_znaka = "\n\033[1;32mПодржани су бројеви знакова од 1 до 6!\033[0m\n\0";

#define znak_pik  "\033[1;34m\u2660\033[0m\0"
#define znak_karo  "\033[1;31m\u2666\033[0m\0"
#define znak_skocko  "\033[1;33m\u263A\033[0m\0"
#define znak_zvezda  "\033[1;33m\u2605\033[0m\0"
#define znak_tref  "\033[1;34m\u2663\033[0m\0"
#define znak_herc  "\033[1;31m\u2665\033[0m\0"
#define znak_crveni  "\033[1;31m\u25CF\033[0m\0"
#define znak_zuti  "\033[1;33m\u25CF\033[0m\0"
#define znak_prazan  "\0\033[0m\0"
#define ocisti_ekran "\033[2J\0"


#define BROJ_ZNAKOVA 6
#define VELICINA_KOMBINACIJE 4
#define BROJ_ZNAKOVA_TABLA 48
#define tabla_args(a) a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15], a[16], a[17], a[18], a[19], a[20], a[21], a[22], a[23], a[24], a[25], a[26], a[27], a[28], a[29], a[30], a[31], a[32], a[33], a[34], a[35], a[36], a[37], a[38], a[39], a[40], a[41], a[42], a[43], a[44], a[45], a[46], a[47]
#define red_args(a) a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]
#define komb_trazena_args(a) a[0], a[1], a[2], a[3]
#define NEISPRAVAN_ZNAK -1
                          

#endif

