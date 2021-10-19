
/*******************************************************************
   scaner_d.h
********************************************************************/

struct find_string {
    char* wrd;   
    lc_type lc;
};

#define rw_size   13 

struct find_string rwtab[] = {
    {"abort",  KWABORT},
    {"begin", KWBEGIN},
    {"case",  KWCASE},
    {"end",  KWEND},
    {"float",  KWFLOAT},
    {"int",  KWINT},
    {"loop",  KWLOOP},
    {"or",  KWOR},
    {"read",  KWREAD},
    {"skip",  KWSKIP},
    {"space",  KWSPACE},
    {"tab",  KWTAB},
    {"var",  KWVAR},
    {"write",  KWWRITE}
};

long int poz=0, oldpoz;
int oldline, oldcolumn;
