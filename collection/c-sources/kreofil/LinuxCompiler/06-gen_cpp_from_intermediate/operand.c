

/******************************************************************
 operand.c - определния функций обработки операнда
******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_c.h"
#include "main_t.h"
#include "main_e.h"

void er(int);

static int opdNum = 0; // Номер операнда (в порядке создания

// Создание и заполнение структуры константы
struct CONST *CONST_constructor(typ, val)
  scalType typ;
  void *val;
{
  struct CONST *constPtr;
  if((constPtr = (struct CONST*)malloc(sizeof(struct CONST)))==NULL)
  {
    er(23); exit(1);
  }
  constPtr->typ = typ;
  if(typ == INTTYP) constPtr->val.unum = *((int *)val);
  else if(typ == FLOATTYP) constPtr->val.fnum = *((float *)val);
  return constPtr;
}

// Создание и заполнение структуры операнда
struct OPERAND *OPERAND_constructor(typ, val)
  opdType typ;
  void *val;
{
  struct OPERAND *opdPtr;

  int OPERAND_size = sizeof(struct OPERAND);
  if((opdPtr = (struct OPERAND*)malloc(OPERAND_size))==NULL)
  {
    er(23); exit(1);
  }
  opdPtr->ident = ++opdNum;
  opdPtr->typ = typ;
  opdPtr->next = NULL;
  switch(typ)
  {
    case constOpd:
      opdPtr->val.cons = (struct CONST *)val;
      break;
    case nameVarOpd:
      opdPtr->val.var = (struct application *)val;
      break;
    case tmpVarOpd:
      opdPtr->val.var = NULL; // организация и содержимое определятся позднее
      break;
    case pointerOpd:
      opdPtr->val.var = NULL; // организация и содержимое определятся позднее
      break;
    default:
      opdPtr->val.var = NULL; //
  }
  OPD_LIST_append(&opdList, opdPtr);
  return opdPtr;
}

// Добавление операнда к списку операндов
void OPD_LIST_append(list, opdPtr)
  struct OPD_LIST* list;
  struct OPERAND*  opdPtr;
{
  if(list->last == NULL)
    opdPtr->next = opdPtr;
  else {
    opdPtr->next = list->last->next;
    list->last->next = opdPtr;
  }
    list->last = opdPtr;
}

// Создание и начальная установка операнда метки
struct OPERAND* CreateLabel()
{
  struct INSTRUCTION* tmpInstrPtr; // указатель на команду - метку
  // создается операнд - метка
  struct OPERAND* tmpOpdPtr =
    OPERAND_constructor(labelOpd, NULL);
  // команда - метка с обратной связью на операнд
  tmpInstrPtr = INSTRUCTION_constructor(labelOpc, tmpOpdPtr, NULL, NULL);
  tmpOpdPtr->val.label = tmpInstrPtr;
  return tmpOpdPtr;
}

// Вывод списка операндов в файл
void OPD_LIST_out(list, outfil)
  struct OPD_LIST* list;
  FILE* outfil;
{
  struct OPERAND* p_tmp;
  fprintf(outfil, "\n    Список операндов:\n");
  if(list->last == NULL) {
    fprintf(outfil, "Операнды отсутствуют!\n");
    return;
  }
  p_tmp = list->last;
  do {
      char* typ;
      char* vec_scal;
      switch(p_tmp->next->typ) {
          case nameVarOpd:
              if(p_tmp->next->val.var->typ == INTTYP) typ = "int";
              else typ = "float";
              if(p_tmp->next->val.var->len == 0) vec_scal = "скаляр";
              else vec_scal = "вектор";
              fprintf(outfil, 
                  //"%d: Объявленная переменная имя = %s, тип = %s, %s, адрес = %d\n",
                  //p_tmp->next->ident, p_tmp->next->val.var->name, ---- 
                  "%d: Объявленная переменная %s, тип = %s, %s, адрес = %d\n",
                  p_tmp->next->ident, p_tmp->next->val.var->name,
                  typ, vec_scal, p_tmp->next->val.var->addr
              );
              break;
          case tmpVarOpd:
              fprintf(outfil, "%d: Промежуточная переменная, тип = %s\n",
                      p_tmp->next->ident,
                      p_tmp->next->val.tmpVarType == INTTYP ? (char*)"int": (char*)"float"
                     );
              break;
          case pointerOpd:
              fprintf(outfil, "%d: Временный указатель, тип = %s\n",
                      p_tmp->next->ident,
                      p_tmp->next->val.pointerType == INTTYP ? (char*)"int*": (char*)"float*"
                     );
              break;
          case constOpd:
              if(p_tmp->next->val.cons->typ == INTTYP) {
                  typ = "int";
                  fprintf(outfil, "%d: Константа, %s, %d\n",
                     p_tmp->next->ident, typ, p_tmp->next->val.cons->val.unum
                );
            } else {
                typ = "float";
                fprintf(outfil, "%d: Константа, %s, %e\n",
                        p_tmp->next->ident, typ, p_tmp->next->val.cons->val.fnum
                );
            }
            break;
          case labelOpd:
              fprintf(outfil, "%d: Метка\n", p_tmp->next->ident);
              break;
          default:
              fprintf(outfil, "Структура операнда непонятна\n");
    }
    p_tmp = p_tmp->next;
  } while(p_tmp != list->last);
}


// Генерация описаний промежуточных операндов в код на c++
void cpp_OPD_LIST_out(list, outfil)
    struct OPD_LIST* list;
    FILE* outfil;
{
    int vec_len = 0;
    struct OPERAND* p_tmp;
    if(list->last == NULL) {
        fprintf(outfil, "// Промежуточные операнды отсутствуют!\n");
        return;
    }
    p_tmp = list->last;
    fprintf(outfil, "// Список промежуточных операндов:\n");
    do {
        char* typ;
        char* vec_scal;
        switch(p_tmp->next->typ) {
            case tmpVarOpd:
                if(p_tmp->next->val.tmpVarType == INTTYP) {
                    fprintf(outfil, "    int _V%d_;\n", p_tmp->next->ident);
                } else {
                    fprintf(outfil, "    float _V%d_;\n", p_tmp->next->ident);
                }
                break;
            case pointerOpd:
                if(p_tmp->next->val.pointerType == INTTYP) {
                    fprintf(outfil, "    int* _pV%d_;\n", p_tmp->next->ident);
                } else {
                    fprintf(outfil, "    float* _pV%d_;\n", p_tmp->next->ident);
                }
                break;
        }
        p_tmp = p_tmp->next;
    } while(p_tmp != list->last);
}

