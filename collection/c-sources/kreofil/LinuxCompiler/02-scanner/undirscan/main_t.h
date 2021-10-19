

/*-------------------------------------------------------------*/
/*  main_t.h */
/*-------------------------------------------------------------*/

typedef enum {
    LETTER = 1, DIG, ERS, BTFN, IGN, NO_SIC
} sic_type;

typedef enum {
  NO_LC = 0,
  ADD, ASS, CL,  COM, DIV, DVT, EQ,  ENF, ERL, FLT, 
  GE,  GT,  ID,  IGNORE,   INT,
  LE,  LRB, LSB, LT,  MOD, MUL, NE,  PCL, PNT, RRB, RSB,
  SPACE,    STRELA,   SUB,
  KWABORT, KWBEGIN, KWCASE,  KWEND, KWFLOAT, KWINT,   KWLOOP,
  KWOR,    KWREAD,  KWSKIP,  KWSPACE, KWTAB, KWVAR,   KWWRITE
} lc_type;
