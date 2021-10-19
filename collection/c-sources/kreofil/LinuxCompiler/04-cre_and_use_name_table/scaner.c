

/******************************************************************
 saner.c 
******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "main_c.h"
#include "main_t.h"
#include "main_e.h"
#include "scaner_d.h"

void find_idkw(void);
void id_etc(void);
void number(void);
void divcom(void);
void prenumber(void);
void fltnumber(void);
void fltnumber2(void);
void expon(void);
int  siis(char*);
void nxsi(void);
sic_type translt(void);
void er(int);

void nxl() {
  do {
    i_lv = -1;
    lv[0] = '\0';
        if(si == EOF) {lc = ENF;}
        else if(sic == BTFN) {nxsi(); lc = SPACE;}
        else if(sic==LETTER||si=='_'){lv[++i_lv]=si; nxsi(); id_etc();}
        else if(sic==DIG) {number();}
        else if(sic == IGN) {nxsi(); lc = IGNORE;}
        else if(si=='/') {nxsi(); divcom();}
        else if(si == ';') {nxsi(); lc = PCL;}
        else if(si == ',') {nxsi(); lc = CL;}
        else if(si == ':') {
          nxsi(); 
          if(si == '=') {nxsi(); lc = ASS;}
          else lc = DVT;
        }
        else if(si == '(') {nxsi(); lc = LRB;}
        else if(si == ')') {nxsi(); lc = RRB;}
	else if(si == '[') {nxsi(); lc = LSB;}
	else if(si == ']') {nxsi(); lc = RSB;}
	else if(si == '*') {nxsi(); lc = MUL;}
	else if(si == '%') {nxsi(); lc = MOD;}
        else if(si == '+') {nxsi(); lc = ADD;}
        else if(si == '-') {
          nxsi(); 
          if(si == '>') {nxsi(); lc = STRELA;}  
          else lc = SUB;
        }
        else if(si == '=') {nxsi(); lc = EQ;}
	else if(si == '!') {
          nxsi(); 
          if(si == '=') {nxsi(); lc = NE;}
	  else {lc = ERL; er(1);}
	}
	else if(si == '>') {
	  nxsi();
	  if(si == '=') {nxsi(); lc = GE;}
	  lc = GT;
        }
	else if(si == '<') {
          nxsi(); 
	  if(si == '=') {nxsi(); lc = LE;}
	  lc = LT;
	}
	else if(si == '{') {nxsi(); prenumber();}
	else if(si == '.') {lv[++i_lv]=si; nxsi(); fltnumber2();}
	else {lc = ERL; er(0); nxsi();}
  } while(lc==COM || lc==SPACE || lc==IGNORE); /* игнорируемые лексеммы */
}

void id_etc()
{
    while(sic==LETTER||sic==DIG||si=='_') {
        lv[++i_lv] = si;
        nxsi();
    }
    lv[++i_lv] = '\0';
    find_idkw(); 
}

void find_idkw()
{
    int i;
    for(i = 0; i < rw_size; ++i)
        if(!strcmp(rwtab[i].wrd, lv)) {
            lc = rwtab[i].lc;
            return;
        }
    lc = ID;
}

void divcom()
{
    if(si != '*') {lc = DIV; return;}
    nxsi();
    loop:if(si == EOF) {lc = ERL; lv[++i_lv]='\0'; er(2); return;} 
    while(si != '*') {
        lv[++i_lv] = si;
        if(si == EOF) {lc = ERL; lv[++i_lv]='\0'; er(2); return;} 
        nxsi();
    }
    nxsi(); if(si != '/') {lv[++i_lv] = si; goto loop;}
    lv[++i_lv] = '\0';
    lc = COM;
    nxsi();
}

void nxsi()
{
    if((si = getc(infil)) == '\n') {++line; column = 0;}
    else ++column;
    sic = translt();
}

sic_type translt()
{
   if(isalpha(si)) return LETTER;
   else if(isdigit(si)) return DIG;
   else if(si==' '||si=='\f'||si=='\t'||si=='\n') return BTFN;
   else if(si < ' ') return IGN;
   else return NO_SIC;
}

int siis(s) char* s;
{
    int i;
    for(i = 0; s[i] != '\0'; ++i)
        if(s[i] == si) return 1;
    return 0;
}

void number() {
  do {
    lv[++i_lv] = si;
    nxsi();
  } while (sic == DIG);
  if(si == '.') {fltnumber(); return;}
  if(si=='e'||si=='E') {expon();return;}
  if(sic==LETTER) {lc=ERL; lv[++i_lv]='\0'; er(3); return;}
  lc = INT; lv[++i_lv] = '\0'; unum = atoi(lv);
}

void prenumber() {
    int i_beg; // начало обрабатываемой строки
    unum = 0;
    _0:
	if(sic==DIG) {lv[++i_lv]=si; nxsi(); goto _1;}
	lc = ERL; lv[++i_lv] = '\0'; er(3); return;
    _1:
	if(sic==DIG) {lv[++i_lv]=si; nxsi(); goto _1;}
	if(si=='}') {lv[++i_lv] = '\0'; nxsi();
	  if(!strcmp(lv, "2")) {lv[i_lv]='#'; goto _2;}
	  if(!strcmp(lv, "8")) {lv[i_lv]='#'; goto _3;}
	  if(!strcmp(lv, "10")) {lv[i_lv]='#'; goto _4;}
	  if(!strcmp(lv, "16")) {lv[i_lv]='#'; goto _5;}
	  lc = ERL; lv[++i_lv] = '\0'; er(3); return;
	}
	lc = ERL; lv[++i_lv] = '\0'; er(3); return;
    _2:
	if(siis("01")) {lv[++i_lv]=si;
	  unum = 2 * unum + (si - (int)'\0');
	  nxsi(); goto _2;
	}
	if(sic==LETTER||sic==DIG)
	  {lc = ERL; lv[++i_lv]='\0'; er(3); return;}
	lc = INT; lv[++i_lv] ='\0'; return;
    _3:
	if(siis("01234567")) {
	  lv[++i_lv]=si;
	  unum = 8 * unum + (si - (int)'\0');
	  nxsi(); goto _3;
	}
	if(sic==LETTER||sic==DIG)
	  {lc = ERL; lv[++i_lv]='\0'; er(3); return;}
	lc = INT; lv[++i_lv] ='\0'; return;
    _4:
	if(sic==DIG) {
	  lv[++i_lv]=si;
	  unum = 10 * unum + (si - (int)'\0');
	  nxsi(); goto _4;
	}
	if(sic==LETTER) {lc = ERL; lv[++i_lv]='\0'; er(3); return;}
	lc = INT; lv[++i_lv] ='\0'; return;
    _5:
	if(siis("0123456789")) {
	  unum = 16 * unum + (si - (int)'\0');
	  lv[++i_lv]=si;
	  nxsi(); goto _5;
	}
	if(siis("ABCDEF")) {
	  unum = 16 * unum + (si - (int)'A' + 10);
	  lv[++i_lv]=si;
	  nxsi(); goto _5;
	}
	if(siis("abcdef")) {
	  unum = 16 * unum + (si - (int)'a' + 10);
	  lv[++i_lv]=si;
	  nxsi(); goto _5;
	}
	if(sic==LETTER) {lc = ERL; lv[++i_lv]='\0'; er(3); return;}
	lc = INT; lv[++i_lv]='\0'; return;
}

void fltnumber() {
  lv[++i_lv]=si; nxsi();
  _0:
    if(sic==DIG) {lv[++i_lv]=si; nxsi(); goto _0;}
    if(siis("eE")) {expon(); return;}
    if(sic==LETTER) {lc=ERL; lv[++i_lv]='\0'; er(3); return;}
    lc=FLT; lv[++i_lv]='\0'; dnum = atof(lv); return;
}

void expon() {
  lv[++i_lv]=si; nxsi();
  _0:
    if(siis("+-")) {lv[++i_lv]=si; nxsi(); goto _1;}
    if(sic==DIG) {lv[++i_lv]=si; nxsi(); goto _2;}
    lc=ERL; lv[++i_lv]='\0'; return;
  _1:
    if(sic==DIG) {lv[++i_lv]=si; nxsi(); goto _2;}
    lc=ERL; lv[++i_lv]='\0'; er(3); return;
  _2:
    if(sic==DIG) {lv[++i_lv]=si; nxsi(); goto _2;}
    if(sic==LETTER) {lc=ERL; lv[++i_lv]='\0'; er(3); return;}
    lc=FLT; lv[++i_lv]='\0'; dnum = atof(lv); return;
}

void fltnumber2() {
  lv[++i_lv]=si; nxsi();
  _0:
    if(sic==DIG) {lv[++i_lv]=si; nxsi(); goto _1;}
    lc=PNT; lv[++i_lv]='\0'; return;
  _1:
    if(sic==DIG) {lv[++i_lv]=si; nxsi(); goto _1;}
    if(siis("eE")) {expon(); return;}
    if(sic==LETTER) {lc=ERL; lv[++i_lv]='\0'; er(3); return;}
    lc=FLT; lv[++i_lv]='\0'; dnum = atof(lv); return;
}
