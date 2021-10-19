

/******************************************************************/
/* main.c */
/******************************************************************/

#include     <string.h>
#include     <stdio.h>

#include     "main_c.h"
#include     "main_t.h"
#include     "main_d.h"

void fprintlex(FILE*);
void nxsi(void);
void nxl(void);
int program(struct INSTR_LIST**);
void scan_types_tmp_var(struct INSTR_LIST*);
void output(FILE*);
// Вывод списка команд
void INSTR_LIST_out(struct INSTR_LIST*, FILE*);

// Вывод переменных в файл порождаемой программы на c++
void cppOutput(FILE*);
// генерация операндов и операторов на c++
void cpp_OPD_LIST_out(struct OPD_LIST*, FILE*);
void cpp_INSTR_LIST_out(struct INSTR_LIST*, FILE*);

extern int erFlag;

void main(argc, argv) int argc; char *argv[];
{
    FILE* cppfil;   // Файл, используемый для генерации кода на c++
    
    if(argc != 4) {
        printf("\nIncorrect command format!\n"); return;
    }
    if((infil = fopen(argv[1], "r"))==NULL) {
        printf("\nInput file is absent!\n"); return;
    }
    outfil = fopen(argv[2], "w");
  line = 1; column = 0;
  fprintf(outfil, "Analiz is begining!\n");
  // Создание пустого списка команд, к которому будут дабавляться
  // команды, сгенерированные в дальнейшем (INSTRUCTION_LIST_cat)

  last = NULL;
  nxsi();
  nxl();
  if(program(&InstrListPtr) && erFlag==0)
    printf("O\'key!\n");
  else
    printf("Invalid program!\n");

  // Формирование контекста временных переменных для создания
  // соответствующих описаний при генерации кода
  scan_types_tmp_var(InstrListPtr);
  //printf("scan_types_tmp_var\n");

  // Вывод сформированной таблицы имен
  output(outfil);
  // Вывод сформированного списка операндов
  OPD_LIST_out(&opdList, outfil);
  // Вывод сформированного списка команд
  INSTR_LIST_out(InstrListPtr ,outfil);
  fprintf(outfil, "\nEnd of program!\n");
  fclose(infil); fclose(outfil);
  
  // Генерация кода в c++ по построенному промежуточному представлению
  cppfil = fopen(argv[3], "w");
  cppOutput(cppfil);
  
  //scan_types_tmp_var(InstrListPtr);
  //printf("scan_types_tmp_var\n");
  cpp_OPD_LIST_out(&opdList, cppfil);
  //printf("cpp_OPD_LIST_out\n");
  cpp_INSTR_LIST_out(InstrListPtr, cppfil);
  //printf("cpp_INSTR_LIST_out\n");
  fclose(cppfil);
}


