
/*******************************************************************
   scaner_d.h
********************************************************************/

struct find_string {
    char* wrd;   
    lc_type lc;
};

//#define rw_size   14 

//struct find_string rwtab[rw_size] = {
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

int const rw_size = sizeof(rwtab)/sizeof(struct find_string);
