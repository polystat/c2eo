

/******************************************************************
 synt.c
******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "main_c.h"
#include "main_t.h"
#include "main_e.h"

int find(char*);     // НАХОЖДЕНИЕ ЭЛЕМЕНТА ТАБЛИЦЫ С ЗАДАННЫМ ИМЕНЕМ
struct node* incl(char*);    // ВКЛЮЧЕНИЕ НОВОГО ЭЛЕМЕНТА В ТАБЛИЦУ
void del(void);      // УДАЛЕНИЕ ЭЛЕМЕНТА ИЗ ТАБЛИЦЫ
void output(FILE*);  // ПЕЧАТЬ ТАБЛИЦЫ ИМЕН В ФАЙЛЕ И НА ЭКРАНЕ
void nxl(void);
void er(int);

int decl(void);
int oper(struct INSTR_LIST**);

int program(iListPtrPtr)
  struct INSTR_LIST **iListPtrPtr;
{
  struct INSTR_LIST *tmpInstrListPtr = NULL;
  _0:
    if(lc==KWBEGIN) {nxl(); goto _1;}
    return 0;
  _1:
    if(decl()) {goto _2;}
    if(oper(&tmpInstrListPtr)) {
      // Добавление списка команд оператора к уже накопленному списку
      INSTR_LIST_cat(iListPtrPtr, tmpInstrListPtr);
      goto _2;
    }
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
	varyable[i]->val.appl.name = varyable[i]->val.name;
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

int assign(struct INSTR_LIST**, struct OPERAND**, int*);
int condit(struct INSTR_LIST**);
int loopop(struct INSTR_LIST**);
int readop(struct INSTR_LIST**);
int writeop(struct INSTR_LIST**);

int oper(iListPtrPtr)
  struct INSTR_LIST **iListPtrPtr;
{
  // массив указателей на промежуточные операнды
  struct OPERAND* opdArray[32];
  int iArray = 0;
  *iListPtrPtr = NULL; // Определяем пустую цепочку кодов
  _0:
    if(assign(iListPtrPtr, opdArray, &iArray)) {goto _end;}
    if(condit(iListPtrPtr)) {goto _end;}
    if(loopop(iListPtrPtr)) {goto _end;}
    if(lc==KWSKIP) {
      // Генерируется пустая команда
      struct INSTRUCTION *tmpInstr =
	INSTRUCTION_constructor(emptyOpc, NULL, NULL, NULL);
      // Команда добавляется к уже созданному списку команд
      INSTR_LIST_append(iListPtrPtr, tmpInstr);
      nxl(); goto _end;
    }
    if(lc==KWABORT) {
      // Генерируется команда аварийного выхода
      struct INSTRUCTION *tmpInstr =
	INSTRUCTION_constructor(exitOpc, NULL, NULL, NULL);
      // Команда добавляется к уже созданному списку команд
      INSTR_LIST_append(iListPtrPtr, tmpInstr);
      nxl(); goto _end;
    }
    if(readop(iListPtrPtr)) {goto _end;}
    if(writeop(iListPtrPtr)) {goto _end;}
    return 0;
  _end:
    return 1;
}

int defset(struct INSTR_LIST**, struct OPERAND*);

int condit(iListPtrPtr)
  struct INSTR_LIST **iListPtrPtr;
{
  struct OPERAND *endCaseLabel;
  *iListPtrPtr = NULL; // Определяем пустую цепочку кодов
  _0:
    if(lc==KWCASE) {
      endCaseLabel = CreateLabel();
      nxl(); goto _1;
    }
    return 0;
  _1:
    if(defset(iListPtrPtr, endCaseLabel)) {
      // Формируем EXIT команду
      struct INSTRUCTION* tmpInstr =
	INSTRUCTION_constructor(exitOpc, NULL, NULL, NULL);
	INSTR_LIST_append(iListPtrPtr, tmpInstr);
      // Добавляем к цепочке команд конечную метку
	INSTR_LIST_append(iListPtrPtr, endCaseLabel->val.label);
      goto _2;
    }
    if(lc==KWEND) {
      // Формируем EXIT команду
      struct INSTRUCTION* tmpInstr =
	INSTRUCTION_constructor(exitOpc, NULL, NULL, NULL);
	INSTR_LIST_append(iListPtrPtr, tmpInstr);
      nxl(); goto _end;
    }
    er(6); return 0;
  _2:
    if(lc==KWEND) {nxl(); goto _end;}
    er(6); return 0;
  _end:
    return 1;
}

int loopop(iListPtrPtr)
  struct INSTR_LIST **iListPtrPtr;
{
  struct INSTR_LIST *tmpInstrListPtr;
  struct OPERAND *endLoopLabel;
  *iListPtrPtr = NULL;
  _0:
    if(lc==KWLOOP) {
      endLoopLabel = CreateLabel();
      nxl(); goto _1;
    }
    return 0;
  _1:
    if(defset(&tmpInstrListPtr, endLoopLabel)) {
      // Добавляем к цепочке команд метку начала цикла
      INSTR_LIST_append(iListPtrPtr, endLoopLabel->val.label);
      // Добавляем охраняемые операторы тела цикла
      INSTR_LIST_cat(iListPtrPtr, tmpInstrListPtr);
      goto _2;
    }
    if(lc==KWEND) {
      // Формируем EMPTY команду
      struct INSTRUCTION* tmpInstr =
	INSTRUCTION_constructor(emptyOpc, NULL, NULL, NULL);
	INSTR_LIST_append(iListPtrPtr, tmpInstr);
      nxl(); goto _end;
    }
    er(7); return 0;
  _2:
    if(lc==KWEND) {nxl(); goto _end;}
    er(7); return 0;
  _end:
    return 1;
}

int defens(struct INSTR_LIST**, struct OPERAND*);

int defset(iListPtrPtr, extLabel)
  struct INSTR_LIST **iListPtrPtr;
  struct OPERAND* extLabel;
{
  struct INSTR_LIST *tmpInstrListPtr;
  *iListPtrPtr = NULL;
  _0:
    if(defens(&tmpInstrListPtr, extLabel)) {
      INSTR_LIST_cat(iListPtrPtr, tmpInstrListPtr);
      goto _1;
    }
    return 0;
  _1:
    if(lc==KWOR) {nxl(); goto _2;}
    goto _end;
  _2:
    if(defens(&tmpInstrListPtr, extLabel)) {
      INSTR_LIST_cat(iListPtrPtr, tmpInstrListPtr);
      goto _1;
    }
    er(8); return 0;
  _end:
    return 1;
}

int expr(struct INSTR_LIST**);

int defens(iListPtrPtr, extLabel)
  struct INSTR_LIST **iListPtrPtr;
  struct OPERAND *extLabel;
{
  struct INSTR_LIST* tmpInstrListPtr1;
  struct INSTR_LIST* tmpInstrListPtr2;
  struct OPERAND *tmpOpdPtr1;
  struct OPERAND *tmpOpdPtr2;
  struct INSTRUCTION *tmpInstr;
  *iListPtrPtr = NULL;
  _0:
    if(expr(&tmpInstrListPtr1)) {
      // Фиксация операнда выражения
      tmpOpdPtr1 = tmpInstrListPtr1->lastOpd;
      // Формирование начала списка команд для данного условия
      *iListPtrPtr = tmpInstrListPtr1;
      tmpInstrListPtr1 = NULL; // подготовка для накопления кода операторов
      goto _1;
    }
    return 0;
  _1:
    if(lc==STRELA) {nxl(); goto _2;}
    er(9); return 0;
  _2:
    if(oper(&tmpInstrListPtr2)) {
      // накопление кода операторов условия
      INSTR_LIST_cat(&tmpInstrListPtr1, tmpInstrListPtr2);
      goto _3;
    }
    er(9); return 0;
  _3:
    if(lc==PCL) {nxl(); goto _2;}
    goto _end;
  _end:
    // Формирования обрамления в виде команды условия и метки альтернативы
    // Создание перехода после выполнения условия
    tmpInstr = INSTRUCTION_constructor(gotoOpc, extLabel, NULL, NULL);
    INSTR_LIST_append(&tmpInstrListPtr1, tmpInstr);
    // Создаем операнд - метку
    tmpOpdPtr2 = CreateLabel();
    INSTR_LIST_append(&tmpInstrListPtr1, tmpOpdPtr2->val.label);
    // Создание условия
    tmpInstr = INSTRUCTION_constructor(ifOpc, tmpOpdPtr1, tmpOpdPtr2, NULL);
    INSTR_LIST_append(iListPtrPtr, tmpInstr);
    INSTR_LIST_cat(iListPtrPtr, tmpInstrListPtr1);
    return 1;
}

int addit(struct INSTR_LIST**);

int expr(iListPtrPtr)
  struct INSTR_LIST **iListPtrPtr;
{
  opcType opc; // обрабатываема команда
  struct OPERAND *tmpOpdPtr;
  struct OPERAND *tmpOpdPtr1; // первый операнд
  struct INSTR_LIST *tmpListPtr1;
  struct INSTR_LIST *tmpListPtr2;
  *iListPtrPtr = NULL;
  _0:
    if(addit(&tmpListPtr1)) {
      // Фиксация первого операнда
      tmpOpdPtr1 = tmpListPtr1->lastOpd;
      // Формирование начала списка команд из данной суммы
      *iListPtrPtr = tmpListPtr1;
      goto _1;
    }
    return 0;
  _1:
    if(lc==LT) {
      opc = ltOpc;
      nxl(); goto _2;
    }
    if(lc==GT) {
      opc = gtOpc;
      nxl(); goto _2;
    }
    if(lc==EQ) {
      opc = eqOpc;
      nxl(); goto _2;
    }
    if(lc==NE) {
      opc = neOpc;
      nxl(); goto _2;
    }
    if(lc==GE) {
      opc = geOpc;
      nxl(); goto _2;
    }
    if(lc==LE) {
      opc = leOpc;
      nxl(); goto _2;
    }
    goto _end;
  _2:
    if(addit(&tmpListPtr2)) {
      struct INSTRUCTION *tmpInstr;
      // Формирование корректных связей между операндами
      // Формируется временный операнд без заполнения его применения
      // Применение операнда определится при генерации кода
      tmpOpdPtr = OPERAND_constructor(tmpVarOpd, NULL);
      // Команда из разряда: > | < | = | != | >= | <=
      tmpInstr = INSTRUCTION_constructor(opc,
	    tmpOpdPtr1,
	    tmpListPtr2->lastOpd,
	    tmpOpdPtr
      );
      // Добавление команды к списку команд, определяющих 2-е слагаемое
      INSTR_LIST_append(&tmpListPtr2, tmpInstr);
      // Конкатенация списков команд
      INSTR_LIST_cat(iListPtrPtr, tmpListPtr2);
      (*iListPtrPtr)->lastOpd = tmpOpdPtr;
      tmpOpdPtr1 = (*iListPtrPtr)->lastOpd;
      goto _1;
    }
    er(10); return 0;
  _end:
    return 1;
}

int slag(struct INSTR_LIST**);

int addit(iListPtrPtr)
  struct INSTR_LIST **iListPtrPtr;
{
  opcType opc; // обрабатываема команда
  struct OPERAND *tmpOpdPtr1; // первый операнд
  struct OPERAND *tmpOpdPtr;
  struct INSTR_LIST *tmpListPtr1;
  struct INSTR_LIST *tmpListPtr2;
  *iListPtrPtr = NULL;
  _0:
    if(slag(&tmpListPtr1)) {
      // Фиксация первого операнда
      tmpOpdPtr1 = tmpListPtr1->lastOpd;
      // Формирование начала списка команд из данного слагаемого
      *iListPtrPtr = tmpListPtr1;
      goto _1;
    }
    return 0;
  _1:
    if(lc==ADD) {
      opc = addOpc;
      nxl(); goto _2;
    }
    if(lc==SUB) {
      opc = subOpc;
      nxl(); goto _2;
    }
    goto _end;
  _2:
    if(slag(&tmpListPtr2)) {
      struct INSTRUCTION *tmpInstr;
      // Формирование корректных связей между операндами
      // Формируется временный операнд без заполнения его применения
      // Применение операнда определится при генерации кода
      tmpOpdPtr = OPERAND_constructor(tmpVarOpd, NULL);
      // Команда из разряда: + | -
      tmpInstr = INSTRUCTION_constructor(opc,
	    tmpOpdPtr1,
	    tmpListPtr2->lastOpd,
	    tmpOpdPtr
      );
      // Добавление команды к списку операндов
      INSTR_LIST_append(&tmpListPtr2, tmpInstr);
      // Конкатенация списков команд
      INSTR_LIST_cat(iListPtrPtr, tmpListPtr2);
      (*iListPtrPtr)->lastOpd = tmpOpdPtr;
      tmpOpdPtr1 = (*iListPtrPtr)->lastOpd;
      goto _1;
    }
    er(14); return 0;
  _end:
    return 1;

}

int mnog(struct INSTR_LIST**);

int slag(iListPtrPtr)
  struct INSTR_LIST **iListPtrPtr;
{
  opcType opc; // обрабатываема команда
  struct OPERAND *tmpOpdPtr1; // первый операнд
  struct OPERAND *tmpOpdPtr;
  struct INSTR_LIST *tmpListPtr1;
  struct INSTR_LIST *tmpListPtr2;
  *iListPtrPtr = NULL;
  _0:
    if(mnog(&tmpListPtr1)) {
      // Фиксация первого операнда
      tmpOpdPtr1 = tmpListPtr1->lastOpd;
      // Формирование начала списка команд из данного множителя
      *iListPtrPtr = tmpListPtr1;
      goto _1;
    }
    return 0;
  _1:
    if(lc==MUL) {
      opc = multOpc;
      nxl(); goto _2;
    }
    if(lc==DIV) {
      opc = divOpc;
      nxl(); goto _2;
    }
    if(lc==MOD) {
      opc = modOpc;
      nxl(); goto _2;
    }
    goto _end;
  _2:
    if(mnog(&tmpListPtr2)) {
      struct INSTRUCTION *tmpInstr;
      // Формирование корректных связей между операндами
      // Формируется временный операнд без заполнения его применения
      // Применение операнда определится при генерации кода
      tmpOpdPtr = OPERAND_constructor(tmpVarOpd, NULL);
      // Команда из разряда: * | / | %
      tmpInstr = INSTRUCTION_constructor(opc,
	    tmpOpdPtr1,
	    tmpListPtr2->lastOpd,
	    tmpOpdPtr
      );
      // Добавление команды к списку операндов
      INSTR_LIST_append(&tmpListPtr2, tmpInstr);
      // Конкатенация списков команд
      INSTR_LIST_cat(iListPtrPtr, tmpListPtr2);
      (*iListPtrPtr)->lastOpd = tmpOpdPtr;
      tmpOpdPtr1 = (*iListPtrPtr)->lastOpd;
      goto _1;
    }
    er(11); return 0;
  _end:
    return 1;
}

int term(struct INSTR_LIST**);

int mnog(iListPtrPtr)
  struct INSTR_LIST **iListPtrPtr;
{
  *iListPtrPtr = NULL;
  _0:
    if(lc==SUB) {nxl(); goto _1;}
    if(term(iListPtrPtr)) {goto _end;}
    return 0;
  _1:
    if(term(iListPtrPtr)) {
      struct OPERAND *tmpOpdPtr;
      struct INSTRUCTION *tmpInstr;
      // Формирование корректных связей между операндами
      // Формируется временный операнд без заполнения его применения
      // Применение операнда определится при генерации кода
      tmpOpdPtr = OPERAND_constructor(tmpVarOpd, NULL);
      // Команда "унарный минус"
      tmpInstr = INSTRUCTION_constructor(minOpc,
	    (*iListPtrPtr)->lastOpd, NULL, tmpOpdPtr
      );
      // Добавление команды к списку операндов
      INSTR_LIST_append(iListPtrPtr, tmpInstr);
      // Формирование выходного операнда списка команд
      (*iListPtrPtr)->lastOpd = tmpOpdPtr;
      goto _end;
    }
    er(12); return 0;
  _end:
    return 1;
}

int numb(struct CONST**);
int varying(struct INSTR_LIST**);

int term(iListPtrPtr)
  struct INSTR_LIST **iListPtrPtr;
{
  struct CONST *constPtr = NULL; // указатель на формируемую константу
  struct OPERAND *tmpOpdPtr; // временный фиксатор операнда
  _0:
    if(numb(&constPtr)) {
      // Формирование константного операнда и пустого списка команд
      tmpOpdPtr = OPERAND_constructor(constOpd, constPtr);
      // Формирование структуры списка команд для константы
      *iListPtrPtr = INSTR_LIST_constructor(NULL, NULL, tmpOpdPtr);
      goto _end;
    }
    if(varying(iListPtrPtr)) {goto _end;}
    if(lc==LRB) {nxl(); goto _1;}
    return 0;
  _1:
    if(expr(iListPtrPtr)) { // возврат сформированного выражения
      goto _2;
    }
    er(13); return 0;
  _2:
    if(lc==RRB) {nxl(); goto _end;}
    er(13); return 0;
  _end:
    return 1;
}

int numb(constPtrPtr)
  struct CONST **constPtrPtr; // указатель на формируемую константу
{
  _0:
    if(lc==INT) {
      *constPtrPtr = CONST_constructor(INTTYP, &unum);
      nxl(); goto _end;
    }
    if(lc==FLT) {
      *constPtrPtr = CONST_constructor(FLOATTYP, &dnum);
     nxl(); goto _end;
    }
    return 0;
  _end:
    return 1;
}

int varying(iListPtrPtr)
  struct INSTR_LIST **iListPtrPtr;
{
  struct OPERAND *tmpOpdPtr; // временный фиксатор операнда
  struct application* tmpAppl; // временный фиксатор контекста переменной
  struct INSTR_LIST *tmpInstrListPtr; // временный указатель на список команд
  _0:
    if(lc==ID) { // имя переменной д.б. в таблице имен
      if(find(lv)) { // использование элемента таблицы имен
	tmpAppl= &(p_find->val.appl); // временная фиксация найденного узла
	// Формирование структуры операнда - именованной переменной
	tmpOpdPtr = OPERAND_constructor(nameVarOpd, tmpAppl);
	// Формирование структуры списка команд для переменной
	*iListPtrPtr = INSTR_LIST_constructor(NULL, NULL, tmpOpdPtr);
      } else {er(20); return 0;}
      nxl(); goto _1;
    }
    return 0;
  _1:
    if(lc==LSB) {nxl(); goto _2;}
    goto _end;
  _2:
    if(expr(&tmpInstrListPtr)) {
      struct INSTRUCTION *tmpInstr;
      // Формирование корректных связей между операндами
      // Формируется временный операнд без заполнения его применения
      // Применение операнда определится при генерации кода
      tmpOpdPtr = OPERAND_constructor(pointerOpd, NULL);
      // Команда выделения по индексу из вектора
      tmpInstr = INSTRUCTION_constructor(indexOpc,
	    (*iListPtrPtr)->lastOpd,
	    tmpInstrListPtr->lastOpd,
	    tmpOpdPtr
      );
      // Конкатенация списков команд с именем переменной и
      // индексным выражением
      INSTR_LIST_cat(iListPtrPtr, tmpInstrListPtr);
      // Добавление команды к списку команд
      INSTR_LIST_append(iListPtrPtr, tmpInstr);
      // Формирование передаваемого результата (операнда).
      (*iListPtrPtr)->lastOpd = tmpOpdPtr;
      free(tmpInstrListPtr); // удаление после использования
      goto _3;
    }
    er(15); return 0;
  _3:
    if(lc==RSB) {nxl(); goto _end;}
    er(15); return 0;
  _end:
    return 1;
}

//------------------------------------------------------------------------
// Оператор присваивания
int assign(iListPtrPtr, opdArray, iArrayPtr)
    struct INSTR_LIST **iListPtrPtr;
    struct OPERAND* opdArray[];
    int* iArrayPtr;
{
    // Необходимо иметь массив команд присваивания, которые накапливаются для того,
    // чтобы потом дописаться в конце списка формируемых инструкций.
    static struct INSTRUCTION* assignArray[256]; // Пока без проверок на переполнение
                                                 // просто большого размера
    static int assignNumber = 0; // Количество команд присваивания в одном операторе
    // Счетчик уровня вложенности. Увеличивается при входе в очередную рекурсию
    // и вычитается при выходе из нее
    static int count = 0; 
    struct INSTR_LIST *tmpInstrPtr1;
    struct INSTR_LIST *tmpInstrPtr2;
    struct OPERAND    *tmpOpdPtr;
    *iListPtrPtr = NULL;    // Возвращаемый список команд
_0:
    if(varying(&tmpInstrPtr1)) {
        struct INSTRUCTION *tmpInstr; // Указатель на команду присваивания
        tmpOpdPtr = OPERAND_constructor(tmpVarOpd, NULL);
        opdArray[(*iArrayPtr)++]=tmpOpdPtr; // фиксация промежуточного операнда
        // Формируем команду присваивания
        tmpInstr = INSTRUCTION_constructor(assOpc, tmpOpdPtr, tmpInstrPtr1->lastOpd, NULL);
        // Команда добавляется к уже созданному списку команд
        // Вместо добавления к списку команд идет добавление в буфер окончательных
        // команд присваивания
        assignArray[assignNumber++] = tmpInstr;
        if(assignNumber > 256) {
            printf("So long list of variable of assign statement!!!");
            exit(1);
        }
        count++;    //Счетчик устанавливает уровень вложенности присваиваний
        goto _1;
    }
    return 0;
_1:
    if(lc==ASS) {
      *iArrayPtr = 0; // Перебор промежуточных операндов с начала
      nxl(); goto _2;
    }
    if(lc==CL) {nxl(); goto _3;}
    er(16); return 0;
_2:
    if(expr(&tmpInstrPtr2)) {
        // Формируем команду присваивания
        struct INSTRUCTION *tmpInstr = INSTRUCTION_constructor(
            assOpc, tmpInstrPtr2->lastOpd, opdArray[(*iArrayPtr)++], NULL);
        // Команда промежуточного присваивания добавляется к уже созданному списку команд
        INSTR_LIST_append(&tmpInstrPtr2, tmpInstr);
        // Добавляем к элементу правой части сформированный список
        INSTR_LIST_cat(&tmpInstrPtr2, *iListPtrPtr);
        // Добавляем к сформированному списку левую часть
        INSTR_LIST_cat(&tmpInstrPtr2, tmpInstrPtr1);
        // Окончательный список переприсваиваем параметру
        *iListPtrPtr = tmpInstrPtr2;
        goto _end;
    }
    er(16); return 0;
_3:
    if(assign(iListPtrPtr, opdArray, iArrayPtr)) {goto _4;}
    er(16); return 0;
_4:
    if(lc==CL) {nxl(); goto _2;}
    er(16); return 0;
_end:
    count--;    // Уменьшение своего уровня
    if(count == 0) { 
        // Добрались до стартовой точки. Можно переносить команды присваивания в список команд
        ////printf("assignNumber = %d\n", assignNumber);
        for(int i = 0; i < assignNumber; i++) {
             // Перенос присваиваний из буфера в конец списка команд
             INSTR_LIST_append(iListPtrPtr, assignArray[i]);
        }
        assignNumber = 0;
    }
    return 1;
}

int readop(iListPtrPtr)
  struct INSTR_LIST **iListPtrPtr;
{
  struct INSTR_LIST *tmpListPtr = NULL;
  *iListPtrPtr = NULL;
  _0:
    if(lc==KWREAD) {nxl(); goto _1;}
    return 0;
  _1:
    if(varying(&tmpListPtr)) {
      // Генерируется команда ввода очередной переменной
      struct INSTRUCTION *tmpInstr =
	INSTRUCTION_constructor(inOpc, tmpListPtr->lastOpd, NULL, NULL);
      // Команда добавляется к уже созданному списку команд
      INSTR_LIST_append(&tmpListPtr, tmpInstr);
      INSTR_LIST_cat(iListPtrPtr, tmpListPtr);
      goto _2;
    }
    er(17); return 0;
  _2:
    if(lc==CL) {
      nxl(); goto _1;
    }
    goto _end;
  _end:
    return 1;
}

int specif(struct INSTR_LIST**);

int writeop(iListPtrPtr)
  struct INSTR_LIST **iListPtrPtr;
{
  struct INSTR_LIST *tmpListPtr; // для кода, наполняемого в выражении
  *iListPtrPtr = NULL; // начало наполнения кода оператора write
  _0:
    if(lc==KWWRITE) {nxl(); goto _1;}
    return 0;
  _1:
    if(expr(&tmpListPtr)) {
      // Генерируется команда вывода очередного выражения
      struct INSTRUCTION *tmpInstr =
	INSTRUCTION_constructor(outOpc, tmpListPtr->lastOpd, NULL, NULL);
      // Команда добавляется к уже созданному списку команд
      INSTR_LIST_append(&tmpListPtr, tmpInstr);
      INSTR_LIST_cat(iListPtrPtr, tmpListPtr);
      goto _2;
    }
    if(specif(&tmpListPtr)) {
      // определяет цикл команд вывода заданного управляющего символа
      // команда вывода уже сгенерирована в подтвержденном правиле
      INSTR_LIST_cat(iListPtrPtr, tmpListPtr);
      goto _2;
    }
    er(17); return 0;
  _2:
    if(lc==CL) {nxl(); goto _1;}
    goto _end;
  _end:
    return 1;
}

int specif(iListPtrPtr)
  struct INSTR_LIST **iListPtrPtr;
{
  opcType opc; // обрабатываема команда
  struct INSTR_LIST *tmpListPtr; // для кода, наполняемого в выражении
  struct INSTRUCTION *tmpInstr;
  struct CONST *constPtr;
  struct OPERAND *tmpOpdPtr;
  *iListPtrPtr = NULL;
  _0:
    if(lc==KWSPACE) {opc = spaceOutOpc; nxl(); goto _1;}
    if(lc==KWSKIP) {opc = skipOutOpc; nxl(); goto _1;}
    if(lc==KWTAB) {opc = tabOutOpc; nxl(); goto _1;}
    return 0;
  _1:
    if(expr(&tmpListPtr)) {
      // Генерируется команда вывода очередной управляющей последовательности
      tmpInstr =
	 INSTRUCTION_constructor(opc, tmpListPtr->lastOpd, NULL, NULL);
      goto _end;
    }
    // В качестве операнда необходимо создать константу = 1
    unum = 1;
    constPtr = CONST_constructor(INTTYP, &unum);
    // Формирование константного операнда
    tmpOpdPtr = OPERAND_constructor(constOpd, constPtr);
    // Генерируется команда вывода очередной управляющей последовательности
    tmpInstr =
    INSTRUCTION_constructor(opc, tmpOpdPtr, NULL, NULL);
    goto _end;
  _end:
    // Команда добавляется к уже созданному списку команд
    INSTR_LIST_append(&tmpListPtr, tmpInstr);
    INSTR_LIST_cat(iListPtrPtr, tmpListPtr);
    return 1;
}

