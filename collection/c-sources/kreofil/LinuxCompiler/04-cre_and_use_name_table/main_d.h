

/*------------------------------------------------------------*/
/* main_d.h */
/*------------------------------------------------------------*/

int   si;        
sic_type  sic;    

char  lv[BFSIZ]; 

lc_type     lc;
unsigned  unum;
double    dnum;

int i_lv;  

int line,
    column;  

FILE    *infil, 
        *outfil;

int memAddr = 0; // текущий адрес в выделяемой статической памяти

// Переменные, используемые для построения таблиц имен
struct node *last = NULL; // указатель на последний элемент списка

struct node* p_find = NULL;     // указатель на найденный элемент таблицы
struct node* pred_find = NULL;  // указатель на элемент таблицы,
				// предшествующий найденному
