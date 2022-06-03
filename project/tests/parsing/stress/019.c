/*
 * This is a RANDOMLY GENERATED PROGRAM.
 *
 * Generator: csmith 2.4.0
 * Git version: deddca6
 * Options:   (none)
 * Seed:      6434655404688560440
 */

#include "csmith.h"


static long __undefined;

/* --- Struct/Union Declarations --- */
union U0 {
   uint16_t  f0;
   uint8_t  f1;
   signed f2 : 15;
};

/* --- GLOBAL VARIABLES --- */
static union U0 g_3 = {65530UL};
static union U0 *g_5 = &g_3;
static union U0 ** volatile g_4 = &g_5;/* VOLATILE GLOBAL g_4 */


/* --- FORWARD DECLARATIONS --- */
static int8_t  func_1(void);


/* --- FUNCTIONS --- */
/* ------------------------------------------ */
/* 
 * reads : g_4 g_3.f0
 * writes: g_5
 */
static int8_t  func_1(void)
{ /* block id: 0 */
    union U0 *l_2 = &g_3;
    (*g_4) = l_2;
    return g_3.f0;
}




/* ---------------------------------------- */
int main (int argc, char* argv[])
{
    int print_hash_value = 0;
    if (argc == 2 && strcmp(argv[1], "1") == 0) print_hash_value = 1;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3.f0, "g_3.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}

/************************ statistics *************************
XXX max struct depth: 0
breakdown:
   depth: 0, occurrence: 1
XXX total union variables: 0

XXX non-zero bitfields defined in structs: 1
XXX zero bitfields defined in structs: 0
XXX const bitfields defined in structs: 0
XXX volatile bitfields defined in structs: 0
XXX structs with bitfields in the program: 1
breakdown:
   indirect level: 0, occurrence: 0
   indirect level: 1, occurrence: 1
XXX full-bitfields structs in the program: 0
breakdown:
XXX times a bitfields struct's address is taken: 2
XXX times a bitfields struct on LHS: 0
XXX times a bitfields struct on RHS: 0
XXX times a single bitfield on LHS: 0
XXX times a single bitfield on RHS: 0

XXX max expression depth: 1
breakdown:
   depth: 1, occurrence: 3

XXX total number of pointers: 3

XXX times a variable address is taken: 3
XXX times a pointer is dereferenced on RHS: 0
breakdown:
XXX times a pointer is dereferenced on LHS: 1
breakdown:
   depth: 1, occurrence: 1
XXX times a pointer is compared with null: 0
XXX times a pointer is compared with address of another variable: 0
XXX times a pointer is compared with another pointer: 0
XXX times a pointer is qualified to be dereferenced: 0

XXX max dereference level: 1
breakdown:
   level: 0, occurrence: 0
   level: 1, occurrence: 1
XXX number of pointers point to pointers: 1
XXX number of pointers point to scalars: 0
XXX number of pointers point to structs: 0
XXX percent of pointers has null in alias set: 0
XXX average alias set size: 1

XXX times a non-volatile is read: 2
XXX times a non-volatile is write: 1
XXX times a volatile is read: 0
XXX    times read thru a pointer: 0
XXX times a volatile is write: 1
XXX    times written thru a pointer: 0
XXX times a volatile is available for access: 0
XXX percentage of non-volatile access: 75

XXX forward jumps: 0
XXX backward jumps: 0

XXX stmts: 2
XXX max block depth: 0
breakdown:
   depth: 0, occurrence: 2

XXX percentage a fresh-made variable is used: 50
XXX percentage an existing variable is used: 50
FYI: the random generator makes assumptions about the integer size. See platform.info for more details.
XXX total OOB instances added: 0
********************* end of statistics **********************/

