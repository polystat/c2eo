

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
int program(void);

extern int erFlag;

void main(argc, argv) int argc; char *argv[];
{
  if(argc != 3) {
    printf("\nIncorrect command format!\n"); return;
  }
  if((infil = fopen(argv[1], "r"))==NULL) {
    printf("\nInput file is absent!\n"); return;
  }
  outfil = fopen(argv[2], "w");
  line = 1; column = 0;
  fprintf(outfil, "Analiz is begining!\n");
  nxsi();
  nxl();
  erFlag = 0;
  if(program() && erFlag==0)
    printf("O\'key!\n");
  else
    printf("Invalid program!\n");
  fprintf(outfil, "\nEnd of program!\n");
  fclose(infil); fclose(outfil);
}
