

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

int find(char*);     // НАХОЖДЕНИЕ ЭЛЕМЕНТА ТАБЛИЦЫ С ЗАДАННЫМ ИМЕНЕМ
struct node* incl(char*);    // ВКЛЮЧЕНИЕ НОВОГО ЭЛЕМЕНТА В ТАБЛИЦУ
void del(void);      // УДАЛЕНИЕ ЭЛЕМЕНТА ИЗ ТАБЛИЦЫ
void output(FILE*);  // ПЕЧАТЬ ТАБЛИЦЫ ИМЕН В ФАЙЛЕ И НА ЭКРАНЕ

int decl(void);
int oper(void);

int program() {
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
    return 1;
}

int decl() {
  struct node* varyable[256];
  int i = -1; // указатель на первый элемент массива varyable
  _0:
    if(lc==KWVAR) {nxl(); goto _1;}
    return 0;
  _1:
    if(lc==ID) { // идентификатор описываемой переменной
      if(!find(lv)) { // при отсутствии описания переменной формируем новое
	varyable[++i] = incl(lv);
      } else {
	er(19); return 0;
      }
      nxl(); goto _2;
    }
    er(5); return 0;
  _2:
    if(lc==LSB) {nxl(); goto _3;}
    if(lc==CL) { // определяется скалярная переменная - размер = 0
      varyable[i]->val.appl.len = 0;
      nxl(); goto _1;
    }
    if(lc==DVT) { // определяется скалярная переменная - размер = 0
      varyable[i]->val.appl.len = 0;
      nxl(); goto _6;
    }
    er(5); return 0;
  _3:
    if(lc==INT) {// длина массива
      varyable[i]->val.appl.len = unum;
      nxl(); goto _4;
    }
    er(5); return 0;
  _4:
    if(lc==RSB) {nxl(); goto _5;}
    er(5); return 0;
  _5:
    if(lc==CL) {nxl(); goto _1;}
    if(lc==DVT) {nxl(); goto _6;}
    er(5); return 0;
  _6:
    if(lc==KWINT) {
      int j; // индекс для перебора всех переменных с данным типом
      for(j = 0; j <= i; ++j) {
	varyable[j]->val.appl.typ = INTTYP;
	varyable[j]->val.appl.addr = memAddr;
	if(varyable[j]->val.appl.len == 0)
	  memAddr += 2;
	else
	  memAddr += 2*varyable[j]->val.appl.len;
      }
      nxl(); goto _end;
    }
    if(lc==KWFLOAT) {
      int j; // индекс для перебора всех переменных с данным типом
      for(j = 0; j <= i; ++j) {
	varyable[j]->val.appl.typ = FLOATTYP;
	varyable[j]->val.appl.addr = memAddr;
	if(varyable[j]->val.appl.len == 0)
	  memAddr += 4;
	else
	  memAddr += 4*varyable[j]->val.appl.len;
      }
      nxl(); goto _end;
    }
    er(5); return 0;
  _end:
    return 1;
}

int assign(void);
int condit(void);
int loopop(void);
int readop(void);
int writeop(void);

int oper() {
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
  //struct application* tmpAppl; // временный фиксатор контекста переменной
  _0:
    if(lc==ID) { // имя переменной д.б. в таблице имен
      if(find(lv)) { // использование элемента таблицы имен
	//tmpAppl= &(p_find->val.appl); // временная фиксация найденного узла
      } else {
	er(20); return 0;
      }
      nxl(); goto _1;}
    return 0;
  _1:
    if(lc==LSB) { // Проверка на допустимость индексов (на массив)
/*
	if(tmpNode->val.appl.len <= 0) {
	er(21); return 0;
      }
*/
      nxl(); goto _2;
    }
    // Проверка на скаляр (допустимость использования без индексов)
/*
    if(tmpNode->val.appl.len > 0) {
      er(22); return 0;
    }
*/
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

