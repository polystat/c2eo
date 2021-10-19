

/*------------------------------------------------------------*/
/* main_e.h */
/*------------------------------------------------------------*/

extern int   si;        
extern sic_type  sic;    

extern char  lv[];

extern lc_type     lc;
extern unsigned  unum;
extern double    dnum;

extern int i_lv;  

extern int line,
           column;  

extern FILE  *infil, 
             *outfil;

extern int memAddr; // текущий адрес в выделяемой статической памяти

// Переменные, используемые для построения таблиц имен
extern struct node *last; // указатель на последний элемент списка

extern struct node* p_find;    // указатель на найденный элемент таблицы
extern struct node* pred_find; // указатель на элемент таблицы,
			       // предшествующий найденному
