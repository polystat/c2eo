// Обработка таблицы имен синтаксического анализатора
//   хранение в элементе указателя на динамически создаваемый ключ
//   таблица на основе однонаправленного кольцевого списка

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
//#include <sys/timeb.h>
//#include <conio.h>

#include "main_t.h"
#include "main_e.h"

int find(n) char* n; { // нахождение в таблице элемента с заданным ключом
  struct node* p_tmp; // временный указатель на предыдущий элемент
  if(last == NULL) {// элементов в списке нет
    return 0;
  }
  p_tmp = last; // временный указатель на предыдущий элемент
  do { // поиск  элемента с заданным ключом n)
    int cmprez = strcmp(n, p_tmp->next->val.name);
    if(cmprez == 0) {// совпадение ключей
      p_find = p_tmp->next;
      pred_find = p_tmp;
      // pred_find указывает на предшествующий узел
      return 1;
    } else if(cmprez > 0) { // ключ в буфере больше ключа в узле
      p_tmp = p_tmp->next; // переход к следующему узлу
    } else { // ключ в буфере меньше ключа в узле
      pred_find = p_tmp; // предшествующий элемент
      p_find = p_tmp->next; // узел, следующий за вновь подключаемым
      return 0; // элемента нет и пройден последний, меньший данного
    }
  } while(p_tmp != last); // завершение при отсутствии узла
  p_find = NULL; // указатель найденного элемента элемента пуст
  pred_find = last; // подключаемся за последним элементом
  return 0; // элемент не найден
}

struct node* incl(n) char* n; { // включение в список нового элемента
    struct node* p_tmp = (struct node *)malloc(sizeof(struct node));
    p_tmp->val.name = (char *)malloc(sizeof(char)*(strlen(n)+1));
    strcpy(p_tmp->val.name, n);
    // включение узла в список:
    if(last == NULL) {// список пуст
      last = p_tmp; // внесли в список
      p_tmp->next = last; // замкнули кольцо
    } else  { // поставить за предшествующим
      p_tmp->next = pred_find->next; // зацепление с последующим
      pred_find->next = p_tmp; // зацепление предшественника
      if(p_find == NULL) // перенос последнего
	// (если при поиске пройден весь список)
	last = p_tmp;
    }
    return p_tmp;
}

void del(void) {
  if(p_find->next == p_find) {// это последний узел в списке
    free(p_find->val.name);
    free(p_find); // удаление последнего узла
    last = NULL; // создается пустой список
  } else {// убирается найденный элемент
    if(p_find == last) // удаляется последний элемент списка
      last = pred_find; // предпоследний узел становится последним
    pred_find->next = p_find->next;
    free(p_find->val.name);
    free(p_find);
  }
}

void output(outfil) FILE* outfil; {
  struct node* p_tmp;
  fprintf(outfil, "\n    Таблица имен программы:\n");
  if(last == NULL) {
    fprintf(outfil, "Имена отсутствуют!\n");
    return;
  }
  p_tmp = last;
  do {
    char* typ;
    if(p_tmp->next->val.appl.typ == INTTYP) typ = "int";
    else typ = "float";
    fprintf(outfil, "%s[%d]:%s{%d}\n",
      p_tmp->next->val.name,
      p_tmp->next->val.appl.len,
      typ,
      p_tmp->next->val.appl.addr
    );
    p_tmp = p_tmp->next;
  } while(p_tmp != last);
}
