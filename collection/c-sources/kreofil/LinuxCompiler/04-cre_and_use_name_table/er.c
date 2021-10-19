

/* er.c */

#include <stdio.h>

#include "er_d.h"

extern int line, column;

void er(i) int i; {
  fprintf(stderr, "(%d, %d) ERROR %d: %s\n", line, column, i, ermes[i]);
  ++erFlag;
}
