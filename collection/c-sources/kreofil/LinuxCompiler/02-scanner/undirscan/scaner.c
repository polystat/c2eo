

/******************************************************************
 saner.c 
******************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "main_c.h"
#include "main_t.h"
#include "main_e.h"
#include "scaner_d.h"

void find_idkw(void);
int id_etc(void);
int decimal(void);
int pdecimal(void);
int binary(void);
int octal(void);
int hex(void);
int float1(void);
int float2(void);
int float3(void);
int float4(void);
int float5(void);
int com(void);
int  siis(char*);
void nxsi(void);
sic_type translt(void);
void er(int);
void unset(void);

void nxl() {
    i_lv = -1;   
    lv[0] = '\0';
    oldpoz=ftell(infil)-1;
    oldline=line; oldcolumn=column;
        if(si == EOF) {lc = ENF; return;}
        if(sic == BTFN) {nxsi(); lc = SPACE; return;}
        if(id_etc()){return;}  unset();
        if(float1()) {return;} unset();
        if(float2()) {return;} unset();
        if(float3()) {return;} unset();
        if(float4()) {return;} unset();
        if(float5()) {return;} unset();
        if(binary()) {return;} unset();
        if(octal()) {return;}  unset();
        if(hex()) {return;}    unset();
        if(pdecimal()) {return;} unset();
        if(decimal()) {return;} unset();
        if(com()) {return;}    unset();
        if(sic == IGN) {nxsi(); lc = IGNORE; return;}
        if(si=='/') {nxsi(); lc=DIV;return;}
        if(si == ';') {nxsi(); lc = PCL; return;}
        if(si == ',') {nxsi(); lc = CL; return;}
        if(si == ':') {
          nxsi(); 
          if(si == '=') {nxsi(); lc = ASS; return;}
        } unset();
        if(si==':') {nxsi(); lc = DVT; return;}
        if(si == '(') {nxsi(); lc = LRB; return;}
        if(si == ')') {nxsi(); lc = RRB; return;}
	if(si == '[') {nxsi(); lc = LSB; return;}
	if(si == ']') {nxsi(); lc = RSB; return;}
	if(si == '*') {nxsi(); lc = MUL; return;}
        if(si == '%') {nxsi(); lc = MOD; return;}
        if(si == '+') {nxsi(); lc = ADD; return;}
        if(si == '.') {nxsi(); lc = PNT; return;}
        if(si == '-') {
          nxsi(); 
          if(si == '>') {nxsi(); lc = STRELA; return;}  
        } unset();
        if(si=='-') {nxsi(); lc=SUB; return;}
        if(si == '=') {nxsi(); lc = EQ; return;}
	if(si == '!') {
          nxsi(); 
	  if(si == '=') {nxsi(); lc = NE; return;}
        } unset();
	if(si == '>') {
          nxsi(); 
          if(si == '=') {nxsi(); lc = GE; return;}
        } unset();
        if(si=='>') {nxsi(); lc=GT; return;}
	if(si == '<') {
          nxsi(); 
          if(si == '=') {nxsi(); lc = LE; return;}
        } unset();
        if(si=='<') {nxsi(); lc=LT; return;}
	lc = ERL; er(0); nxsi();
}

void unset() {
  fseek(infil, oldpoz, 0);  nxsi();
  i_lv=-1; lv[0]='\0'; poz = oldpoz;
  line=oldline; column=oldcolumn;
}

int id_etc()
{
    if(sic==LETTER||sic=='_') {lv[++i_lv]=si; nxsi();}
    else return 0;
    while(sic==LETTER||sic==DIG||si=='_') {
        lv[++i_lv] = si;
        nxsi();
    }
    lv[++i_lv] = '\0';
    find_idkw(); 
    return 1;
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

int com()
{
    if(si=='/') {nxsi();}
    else return 0;
    if(si != '*') {return 0;}
    nxsi();
    loop:if(si == EOF) {lc = ERL; lv[++i_lv]='\0'; er(2); return 1;} 
    while(si != '*') {
        lv[++i_lv] = si;
        if(si == EOF) {lc = ERL; lv[++i_lv]='\0'; er(2); return 1;} 
        nxsi();
    }
    nxsi(); if(si != '/') {lv[++i_lv] = si; goto loop;}
    lv[++i_lv] = '\0';
    lc = COM;
    nxsi();
    return 1;   
}

void nxsi()
{
    if((si = getc(infil)) == '\n') {++line; column = 0;}
    else ++column;
    sic = translt();
    ++poz;
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

int decimal() {
  if(sic==DIG){lv[++i_lv]=si; nxsi();} else return 0;
  while (sic == DIG){
    lv[++i_lv] = si;
    nxsi();
  }
  if(sic==LETTER) {lc=ERL; lv[++i_lv]='\0'; er(3); return 1;}
  lc = INT; lv[++i_lv] = '\0'; return 1; 
}

int pdecimal() {
  if(si=='{') nxsi(); else return 0;
  if(si=='1'){lv[++i_lv]='1'; nxsi();} else return 0;
  if(si=='0'){lv[++i_lv]='0'; nxsi();} else return 0;
  if(si=='}'){lv[++i_lv]='#'; nxsi();} else return 0;
  if(sic==DIG){lv[++i_lv]=si; nxsi();} else return 0;
  while (sic == DIG){
    lv[++i_lv] = si;
    nxsi();
  }
  if(sic==LETTER) {lc=ERL; lv[++i_lv]='\0'; er(3); return 1;}
  lc = INT; lv[++i_lv] = '\0'; return 1; 
}

int binary() {
  if(si=='{') nxsi(); else return 0;
  if(si=='2'){lv[++i_lv]='1'; nxsi();} else return 0;
  if(si=='}'){lv[++i_lv]='#'; nxsi();} else return 0;
  if(si=='0'||si=='1'){lv[++i_lv]=si; nxsi();} else return 0;
  while (si=='0'||si=='1'){
    lv[++i_lv] = si;
    nxsi();
  }
  if(sic==LETTER) {lc=ERL; lv[++i_lv]='\0'; er(3); return 1;}
  lc = INT; lv[++i_lv] = '\0'; return 1; 
}

int octal() {
  if(si=='{') nxsi(); else return 0;
  if(si=='8'){lv[++i_lv]='8'; nxsi();} else return 0;
  if(si=='}'){lv[++i_lv]='#'; nxsi();} else return 0;
  if(si>='0' && si<='7'){lv[++i_lv]=si; nxsi();} else return 0;
  while (si>='0' && si<='7') {
    lv[++i_lv] = si;
    nxsi();
  }
  if(sic==LETTER) {lc=ERL; lv[++i_lv]='\0'; er(3); return 1;}
  lc = INT; lv[++i_lv] = '\0'; return 1; 
}

int hex() {
  if(si=='{') nxsi(); else return 0;
  if(si=='1'){lv[++i_lv]='1'; nxsi();} else return 0;
  if(si=='6'){lv[++i_lv]='0'; nxsi();} else return 0;
  if(si=='}'){lv[++i_lv]='#'; nxsi();} else return 0;
  if(siis("0123456789ABCDEFabcdef"))
    {lv[++i_lv]=si; nxsi();} else return 0;
  while (siis("0123456789ABCDEFabcdef")) {
    lv[++i_lv] = si;
    nxsi();
  }
  if(sic==LETTER) {lc=ERL; lv[++i_lv]='\0'; er(3); return 1;}
  lc = INT; lv[++i_lv] = '\0'; return 1; 
}

int float1() {
  if(sic==DIG){lv[++i_lv]=si; nxsi();} else return 0;
  while (sic == DIG){
    lv[++i_lv] = si;
    nxsi();
  }
  if(si=='e'||si=='E') {lv[++i_lv]=si; nxsi();} else return 0;
  if(si=='+'||si=='-') {lv[++i_lv]=si; nxsi();} 
  if(sic==DIG){lv[++i_lv]=si; nxsi();} else return 0;
  while (sic == DIG){
    lv[++i_lv] = si;
    nxsi();
  }
  if(sic==LETTER) {lc=ERL; lv[++i_lv]='\0'; er(3); return 1;}
  lc = FLT; lv[++i_lv] = '\0'; return 1; 
}

int float2() {
  if(sic==DIG){lv[++i_lv]=si; nxsi();} else return 0;
  while (sic == DIG){
    lv[++i_lv] = si;
    nxsi();
  }
  if(si=='.') {lv[++i_lv]=si; nxsi();} else return 0;
  while(sic==DIG) {lv[++i_lv]=si; nxsi();}
  if(si=='e'||si=='E') {lv[++i_lv]=si; nxsi();} else return 0;
  if(si=='+'||si=='-') {lv[++i_lv]=si; nxsi();} 
  if(sic==DIG){lv[++i_lv]=si; nxsi();} else return 0;
  while (sic == DIG){
    lv[++i_lv] = si;
    nxsi();
  }
  if(sic==LETTER) {lc=ERL; lv[++i_lv]='\0'; er(3); return 1;}
  lc = FLT; lv[++i_lv] = '\0'; return 1; 
}

int float3() {
  if(sic==DIG){lv[++i_lv]=si; nxsi();} else return 0;
  while (sic == DIG){
    lv[++i_lv] = si;
    nxsi();
  }
  if(si=='.') {lv[++i_lv]=si; nxsi();} else return 0;
  while(sic==DIG) {lv[++i_lv]=si; nxsi();}
  if(sic==LETTER) {lc=ERL; lv[++i_lv]='\0'; er(3); return 1;}
  lc = FLT; lv[++i_lv] = '\0'; return 1; 
}

int float4() {
  if(si=='.') {lv[++i_lv]=si; nxsi();} else return 0;
  if(sic==DIG){lv[++i_lv]=si; nxsi();} else return 0;
  while (sic == DIG){
    lv[++i_lv] = si;
    nxsi();
  }
  if(si=='e'||si=='E') {lv[++i_lv]=si; nxsi();} else return 0;
  if(si=='+'||si=='-') {lv[++i_lv]=si; nxsi();} 
  if(sic==DIG){lv[++i_lv]=si; nxsi();} else return 0;
  while (sic == DIG){
    lv[++i_lv] = si;
    nxsi();
  }
  if(sic==LETTER) {lc=ERL; lv[++i_lv]='\0'; er(3); return 1;}
  lc = FLT; lv[++i_lv] = '\0'; return 1; 
}

int float5() {
  if(si=='.') {lv[++i_lv]=si; nxsi();} else return 0;
  if(sic==DIG){lv[++i_lv]=si; nxsi();} else return 0;
  while (sic == DIG){
    lv[++i_lv] = si;
    nxsi();
  }
  if(sic==LETTER) {lc=ERL; lv[++i_lv]='\0'; er(3); return 1;}
  lc = FLT; lv[++i_lv] = '\0'; return 1; 
}
