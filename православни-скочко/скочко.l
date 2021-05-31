%option noyywrap yylineno

%{
    #include <string.h>
    #include "скочко.tab.h"
    #include "defs.h"
    extern YYSTYPE yylval;
%}


%%

[ \t\n] {}


(с|С)(к|К)(о|О)(ч|Ч)(к|К)(о|О)           {yylval.i = SKOCKO; return _ZNAK;}
(к|К)(а|А)(р|Р)(о|О)                     {yylval.i = KARO; return _ZNAK;}
(п|П)(и|И)(к|К)                          {yylval.i = PIK; return _ZNAK;}
(т|Т)(р|Р)(е|Е)(ф|Ф)                     {yylval.i = TREF; return _ZNAK;}
(з|З)(в|В)(е|Е)(з|З)(д|Д)(а|А)           {yylval.i = ZVEZDA; return _ZNAK;}
(х|Х)(е|Е)(р|Р)(ц|Ц)                     {yylval.i = HERC; return _ZNAK;}

(н|Н)(о|О)(в|В)(а|А)                     {return _NOVA_IGRA;}
(и|И)(г|Г)(р|Р)(а|А)                     {return _NOVA_IGRA;}
(к|К)(р|Р)(а|А)(ј|Ј)                     {return _KRAJ;}

.                                       {printf("Непознат унос. Пробајте поново.\n");}
%%