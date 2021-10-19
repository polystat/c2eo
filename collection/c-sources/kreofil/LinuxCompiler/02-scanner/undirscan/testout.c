

/* tesout.c */

#include <stdio.h>
#include "main_c.h"
#include "main_t.h"
#include "main_e.h"

void fprintlex(f) FILE* f; {
  char* lcs;
  char* lvs;
  switch(lc) {
    case NO_LC: lcs="NO_LC"; lvs=lv; break;
    case ADD: lcs="ADD"; lvs="+"; break;
    case ASS: lcs="ASS"; lvs=":="; break;
    case CL: lcs="CL"; lvs=","; break;
    case COM: lcs="COM"; lvs=lv; break;
    case DIV: lcs="DIV"; lvs="/"; break;
    case DVT: lcs="DVT"; lvs=":"; break;
    case EQ: lcs="EQ"; lvs="="; break;
    case ENF: lcs="ENF"; lvs="-1"; break;
    case ERL: lcs="ERL"; lvs=lv; break;
    case FLT: lcs="FLT"; lvs=lv; break;
    case GE: lcs="GE"; lvs=">="; break;
    case GT: lcs="GT"; lvs=">"; break;
    case ID: lcs="ID"; lvs=lv; break;
    case IGNORE: lcs="IGNORE"; lvs=lv; break;
    case INT: lcs="INT"; lvs=lv; break;
    case LE: lcs="LE"; lvs="<="; break;
    case LRB: lcs="LRB"; lvs="("; break;
    case LSB: lcs="LSB"; lvs="["; break;
    case LT: lcs="LT"; lvs="<"; break;
    case MOD: lcs="MOD"; lvs="%"; break;
    case MUL: lcs="MUL"; lvs="*"; break;
    case NE: lcs="NE"; lvs="!="; break;
    case PCL: lcs="PCL"; lvs=";"; break;
    case PNT: lcs="PNT"; lvs="."; break;
    case RRB: lcs="RRB"; lvs=")"; break;
    case RSB: lcs="RSB"; lvs="]"; break;
    case SPACE: lcs="SPACE"; lvs=" "; break;
    case STRELA: lcs="STRELA"; lvs="->"; break;
    case SUB: lcs="SUB"; lvs="-"; break;
    case KWABORT: lcs="KWABORT"; lvs=lv; break;
    case KWBEGIN: lcs="KWBEGIN"; lvs=lv; break;
    case KWCASE: lcs="KWCASE"; lvs=lv; break;
    case KWEND: lcs="KWEND"; lvs=lv; break;
    case KWFLOAT: lcs="KWFLOAT"; lvs=lv; break;
    case KWINT: lcs="KWINT"; lvs=lv; break;
    case KWLOOP: lcs="KWLOOP"; lvs=lv; break;
    case KWOR: lcs="KWOR"; lvs=lv; break;
    case KWREAD: lcs="KWREAD"; lvs=lv; break;
    case KWSKIP: lcs="KWSKIP"; lvs=lv; break;
    case KWSPACE: lcs="KWSPACE"; lvs=lv; break;
    case KWTAB: lcs="KWTAB"; lvs=lv; break;
    case KWVAR: lcs="KWVAR"; lvs=lv; break;
    case KWWRITE: lcs="KWWRITE"; lvs=lv; break;
    default: lcs="no printed lex class"; lvs=lv;
  }
  fprintf(f, "lc = %s\nlv =%s\n----------------------------\n",
              lcs,     lvs);  
}
