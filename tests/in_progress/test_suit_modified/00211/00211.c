extern int printf(const char *format, ...);

#define ACPI_TYPE_INVALID       0x1E
#define NUM_NS_TYPES            ACPI_TYPE_INVALID+1
int array[NUM_NS_TYPES];

#define n 0xe
int main()
{
    int x;
    x = n+1;
    printf("%d\n", x);
//    printf("n+1 = %d\n", 0xe+1);
}
