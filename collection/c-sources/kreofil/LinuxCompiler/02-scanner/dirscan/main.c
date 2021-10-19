

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
  nxsi(); 
  do {nxl(); fprintlex(outfil);} while(lc != ENF);
  printf("\nEnd of program!\n");
  fclose(infil); fclose(outfil);
} 
