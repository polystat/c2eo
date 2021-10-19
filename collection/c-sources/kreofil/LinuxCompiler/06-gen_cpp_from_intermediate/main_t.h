

/*-------------------------------------------------------------*/
/*  main_t.h */
/*-------------------------------------------------------------*/

typedef enum {
    LETTER = 1, DIG, ERS, BTFN, IGN, NO_SIC
} sic_type;

typedef enum {
  NO_LC = 0,
  ADD, ASS, CL,  COM, DIV, DVT, EQ,  ENF, ERL, FLT, 
  GE,  GT,  ID,  IGNORE,   INT,
  LE,  LRB, LSB, LT,  MOD, MUL, NE,  PCL, PNT, RRB, RSB,
  SPACE,    STRELA,   SUB,
  KWABORT, KWBEGIN, KWCASE,  KWEND, KWFLOAT, KWINT,   KWLOOP,
  KWOR,    KWREAD,  KWSKIP,  KWSPACE, KWTAB, KWVAR,   KWWRITE
} lc_type;

// Типы данных, необходимые для работы с таблицей имен
typedef enum {INTTYP, FLOATTYP} scalType;

struct application {
    char* name;
    scalType typ; // скалярный тип элемента таблицы
    int  len; // длина вектора ( = 0, если скаляр)
    int addr; // адрес в физической памяти (только статической)
};

struct element {
  char* name;
  struct application appl;
};

struct node {
  struct element val;
  struct node *next;
};

// Структуры данных и функции, обеспечивающие хранение и обработку операндов

// Организация константы
struct CONST {
  scalType typ; // тип константы
  union {
    int   unum;  // целая константа
    float fnum;  // действительная константа
  } val; // значение константы
};

// Создание и заполнение структуры константы
struct CONST *CONST_constructor(scalType, void*);

// Разновидности операнда
typedef enum {
  nameVarOpd, // именованная переменная из таблицы имен
  tmpVarOpd,  // промежуточная переменная в выражениях
  constOpd,   // константный операнд
  labelOpd,    // операнд - метка
  pointerOpd  // операнд - указатель (на элемент массива)
} opdType;

// Организация операнда
struct OPERAND {
  opdType typ; // тип операнда
  union {
    // ссылка на именованную или промежуточную переменную
    struct application *var;
    // ссылка на константу
    struct CONST *cons;
    // ссылка на метку
    struct INSTRUCTION *label;
    // Хранение типа для временной переменной
    scalType tmpVarType;
    scalType pointerType;
  } val; // значение операнда
  // Дополнительные параметры, обеспечивающие тестирование
  int ident; // уникальный идентификатор операнда
  struct OPERAND *next; // для организации списка операндов
};

// Создание и заполнение структуры операнда
struct OPERAND* OPERAND_constructor(opdType, void*);

// Организация списка операндов
struct OPD_LIST {
  struct OPERAND *last;
};

// Добавление операнда к списку операндов
void OPD_LIST_append(struct OPD_LIST*, struct OPERAND*);

// Вывод списка операндов в файл
void OPD_LIST_out(struct OPD_LIST*, FILE*);

// Структуры данных и функции, обеспечивающие хранение и обработку
// команд промежуточного представления

// Разновидности команд промежуточного представления
typedef enum {
  addOpc,	assOpc,		divOpc,		eqOpc,
  emptyOpc,     exitOpc,	geOpc,		gotoOpc,
  gtOpc,
  ifOpc,	inOpc,		indexOpc,	labelOpc,
  leOpc,	ltOpc,		minOpc,         modOpc,
  multOpc,	neOpc,          outOpc,		skipOutOpc,
  spaceOutOpc,  subOpc,         tabOutOpc
} opcType;

// Организация промежуточной команды
struct INSTRUCTION {
  opcType opc;
  struct INSTRUCTION *next; // отношение расположения/управления
  // Операндная часть инструкции
  struct OPERAND *arg1;
  struct OPERAND *arg2;
  struct OPERAND *rez;
  // Место встречи в тексте программы
  int line;
  int column;
};

// Создание и заполнение структуры команды
struct INSTRUCTION* INSTRUCTION_constructor
   (opcType, struct OPERAND*, struct OPERAND*, struct OPERAND*);

// Организация списка промежуточных команд
struct INSTR_LIST {
  struct INSTRUCTION *firstInstr;
  struct INSTRUCTION *lastInstr;
  struct OPERAND* lastOpd;
};

// Создание и заполнение структуры списка команд промежуточного представления
struct INSTR_LIST *INSTR_LIST_constructor(struct INSTRUCTION*,
   struct INSTRUCTION*, struct OPERAND*);

// Конкатенация двух списков команд в первый
// Замена или установка операнда списка не производится.
// Она должна осуществляться явно в программе анализатора
void INSTR_LIST_cat(struct INSTR_LIST**, struct INSTR_LIST*);

// Добавление к списку команд отдельной команды
void INSTR_LIST_append(struct INSTR_LIST**, struct INSTRUCTION*);

// Вывод списка команд
void INSTR_LIST_out(struct INSTR_LIST *, FILE*);

// Создание операнда - метки
struct OPERAND* CreateLabel(void);
