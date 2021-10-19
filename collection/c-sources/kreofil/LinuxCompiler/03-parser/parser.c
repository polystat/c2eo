

/******************************************************************
 synt.c 
******************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "main_c.h"
#include "main_t.h"
#include "main_e.h"

void nxl(void);
void er(int);

int decl(void);
int oper(void);

int program() {
    //printf("is program?\n");
  _0:
    if(lc==KWBEGIN) {nxl(); goto _1;}
    return 0;
  _1:
    if(decl()) {goto _2;}
    if(oper()) {goto _2;}
    er(4); return 0;
  _2:
    if(lc==PCL) {nxl(); goto _1;}
    if(lc==KWEND) {nxl(); goto _end;}
    er(4); return 0;
  _end:
    //printf("It is program\n");
    return 1;
}

int decl() {
    //printf("is decl?\n");
  _0:
    if(lc==KWVAR) {nxl(); goto _1;}
    return 0;
  _1:
    if(lc==ID) {nxl(); goto _2;}
    er(5); return 0;
  _2:
    if(lc==LSB) {nxl(); goto _3;}
    if(lc==CL) {nxl(); goto _1;}
    if(lc==DVT) {nxl(); goto _6;}
    er(5); return 0;
  _3:
    if(lc==INT) {nxl(); goto _4;}
    er(5); return 0;
  _4:
    if(lc==RSB) {nxl(); goto _5;}
    er(5); return 0;
  _5:
    if(lc==CL) {nxl(); goto _1;}
    if(lc==DVT) {nxl(); goto _6;}
    er(5); return 0;
  _6:
    if(lc==KWINT) {nxl(); goto _end;}
    if(lc==KWFLOAT) {nxl(); goto _end;}
    er(5); return 0;
  _end:
    //printf("It is decl\n");
    return 1;
}

int assign(void);
int condit(void);
int loopop(void);
int readop(void);
int writeop(void);

int oper() {
    //printf("is oper?\n");
  _0:
    if(assign()) {goto _end;}
    if(condit()) {goto _end;}
    if(loopop()) {goto _end;}
    if(lc==KWSKIP) {goto _end;}
    if(lc==KWABORT) {goto _end;}
    if(readop()) {goto _end;}
    if(writeop()) {goto _end;}
    return 0;
  _end:
    //printf("It is oper\n");
    return 1;
}

int defset(void);

int condit() {
  _0:
    if(lc==KWCASE) {nxl(); goto _1;}
    return 0;
  _1:
    if(defset()) {goto _2;}
    if(lc==KWEND) {nxl(); goto _end;}
    er(6); return 0;
  _2:
    if(lc==KWEND) {nxl(); goto _end;}
    er(6); return 0;
  _end:
    return 1;
}

int loopop() {
  _0:
    if(lc==KWLOOP) {nxl(); goto _1;}
    return 0;
  _1:
    if(defset()) {goto _2;}
    if(lc==KWEND) {nxl(); goto _end;}
    er(7); return 0;
  _2:
    if(lc==KWEND) {nxl(); goto _end;}
    er(7); return 0;
  _end:
    return 1;
}

int defens(void);

int defset() {
  _0:
    if(defens()) {goto _1;}
    return 0;
  _1:
    if(lc==KWOR) {nxl(); goto _2;}
    goto _end;
  _2:
    if(defens()) {goto _1;}
    er(8); return 0;
  _end:
    return 1;
}

int expr(void);
int oper(void);

int defens() {
  _0:
    if(expr()) {goto _1;}
    return 0;
  _1:
    if(lc==STRELA) {nxl(); goto _2;}
    er(9); return 0;
  _2:
    if(oper()) {goto _3;}
    er(9); return 0;
  _3:
    if(lc==PCL) {nxl(); goto _2;}
    goto _end;
  _end:
    return 1;
}

int addit(void);

int expr() {
  _0:
    if(addit()) {goto _1;}
    return 0;
  _1:
    if(lc==LT) {nxl(); goto _2;}
    if(lc==GT) {nxl(); goto _2;}
    if(lc==EQ) {nxl(); goto _2;}
    if(lc==NE) {nxl(); goto _2;}
    if(lc==GE) {nxl(); goto _2;}
    if(lc==LE) {nxl(); goto _2;}
    goto _end;
  _2:
    if(addit()) {goto _1;}
    er(10); return 0;
  _end:
    return 1;
}

int slag(void);

int addit() {
  _0:
    if(slag()) {goto _1;}
    return 0;
  _1:
    if(lc==ADD) {nxl(); goto _2;}
    if(lc==SUB) {nxl(); goto _2;}
    goto _end;
  _2:
    if(slag()) {goto _1;}
    er(14); return 0;
  _end:
    return 1;

}

int mnog(void);

int slag() {
  _0:
    if(mnog()) {goto _1;}
    return 0;
  _1:
    if(lc==MUL) {nxl(); goto _2;}
    if(lc==DIV) {nxl(); goto _2;}
    if(lc==MOD) {nxl(); goto _2;}
    goto _end;
  _2:
    if(mnog()) {goto _1;}
    er(11); return 0;
  _end:
    return 1;
}

int term(void);

int mnog() {
  _0:
    if(lc==SUB) {nxl(); goto _1;}
    if(term()) {goto _end;}
    return 0;
  _1:
    if(term()) {goto _end;}
    er(12); return 0;
  _end:
    return 1;
}

int numb(void);
int varying(void);

int term() {
  _0:
    if(numb()) {goto _end;}
    if(varying()) {goto _end;}
    if(lc==LRB) {nxl(); goto _1;}
    return 0;
  _1:
    if(expr()) {goto _2;}
    er(13); return 0;
  _2:
    if(lc==RRB) {nxl(); goto _end;}
    er(13); return 0;
  _end:
    return 1;
}

int numb() {
  _0:
    if(lc==INT) {nxl(); goto _end;}
    if(lc==FLT) {nxl(); goto _end;}
    return 0;
  _end:
    return 1;
}

int varying() {
  _0:
    if(lc==ID) {nxl(); goto _1;}
    return 0;
  _1:
    if(lc==LSB) {nxl(); goto _2;}
    goto _end;
  _2:
    if(expr()) {goto _3;}
    er(15); return 0;
  _3:
    if(lc==RSB) {nxl(); goto _end;}
    er(15); return 0;
  _end:
    return 1;
}

int assign() {
  _0:
    if(varying()) {goto _1;}
    return 0;
  _1:
    if(lc==ASS) {nxl(); goto _2;}
    if(lc==CL) {nxl(); goto _3;}
    er(16); return 0;
  _2:
    if(expr()) {goto _end;}
    er(16); return 0;
  _3:
    if(assign()) {goto _4;}
    er(16); return 0;
  _4:
    if(lc==CL) {nxl(); goto _2;}
    er(16); return 0;
  _end:
    return 1;
}

int readop() {
  _0:
    if(lc==KWREAD) {nxl(); goto _1;}
    return 0;
  _1:
    if(varying()) {goto _2;}
    er(17); return 0;
  _2:
    if(lc==CL) {nxl(); goto _1;}
    goto _end;
  _end:
    return 1;
}

int specif(void);

int writeop() {
  _0:
    if(lc==KWWRITE) {nxl(); goto _1;}
    return 0;
  _1:
    if(expr()) {goto _2;}
    if(specif()) {goto _2;}
    er(17); return 0;
  _2:
    if(lc==CL) {nxl(); goto _1;}
    goto _end;
  _end:
    return 1;
}

int specif() {
  _0:
    if(lc==KWSPACE) {nxl(); goto _1;}
    if(lc==KWSKIP) {nxl(); goto _1;}
    if(lc==KWTAB) {nxl(); goto _1;}
    return 0;
  _1:
    if(expr()) {goto _end;}
    goto _end;
  _end:
    return 1;
}

