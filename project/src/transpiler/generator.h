#ifndef __GENERATOR__
#define __GENERATOR__

#include <string>
#include <vector>
#include <map>

//-------------------------------------------------------------------------------------------------
// Абстрактный генератор кода на всякий случай
// Возможно придется связывать динамически разные генераторы в общий список или массив
// Сюда же, возможно, добавятся общие статические объекты
struct SpaceGen;

struct AbstractGen {
    static SpaceGen* globalSpaceGenPtr;
    static SpaceGen* staticSpaceGenPtr;
    static std::string filename;
    static std::map<int64_t ,std::string> identifiers;

    virtual void Generate(std::string &str) = 0;
    virtual void GenValue(std::string &str) {}
    virtual ~AbstractGen() = default;
};

//-------------------------------------------------------------------------------------------------
// Генератор кода для глобального пространства
// Наряду с константной оберткой обеспечивает запись глобальных объектов
struct SpaceGen: AbstractGen {
    std::vector<AbstractGen*> objects;
    std::string space;      // строка с собранным глобальным пространством

    // Добавление очередного объекта к глобальному пространству
    void Add(AbstractGen* obj);
    void Generate(std::string &str);
    void GenValue(std::string &str);

    ~SpaceGen();
};



//-------------------------------------------------------------------------------------------------
// Генератор кода для глобальных переменных.
// Накапливает необходимые значения в соответствующих строках.
struct VarGen: AbstractGen {
    std::string name;       // идентификатор переменной
    std::string type;       // тип переменной
    std::string value;      // значение переменной
    virtual void Generate(std::string &str);
//    virtual void GenValue(std::string &str);
};


//-------------------------------------------------------------------------------------------------
// Генератор кода для абстрактных инструкций.
// Накапливает необходимые значения в соответствующих строках.
struct StmtGen: AbstractGen {
    std::string value;
    std::string postfix = "";
    int shift;
    std::string getIndentSpaces(int shift);

};

//-------------------------------------------------------------------------------------------------
// Генератор кода для набора инструкций.
// Накапливает необходимые значения в соответствующих строках.
struct CompoundStmtGen: StmtGen {
    std::vector<StmtGen*> statements;

    void Add(StmtGen* stmt);
    virtual void Generate(std::string &str);
    ~CompoundStmtGen();


};

//-------------------------------------------------------------------------------------------------
// Генератор кода для бинарных операций.
// Накапливает необходимые значения в соответствующих строках.
struct BinaryStmtGen: StmtGen {
    StmtGen* left = nullptr;
    StmtGen* right = nullptr;

    virtual void Generate(std::string &str);
    ~BinaryStmtGen();
};

//-------------------------------------------------------------------------------------------------
// Генератор кода для бинарных операций.
// Накапливает необходимые значения в соответствующих строках.
struct UnaryStmtGen: StmtGen {
    StmtGen* nestedStmt = nullptr;
    std::string op;

    virtual void Generate(std::string &str);
    ~UnaryStmtGen();
};
//-------------------------------------------------------------------------------------------------
// Генератор кода для глобальных функций.
// Накапливает необходимые значения в соответствующих строках.
struct GlobalFuncGen: StmtGen {
    std::string name;       // имя объекта-функции
    std::vector<std::string> paramNames;    // список имен параметров (типы не нужны).
    CompoundStmtGen* body;
    // Возращаемый параметры передается как дополнительный атрибут с некоторым именем,
    // которое не должно нигде встречаться в другом контексте.
    virtual void Generate(std::string &str);
    virtual void GenValue(std::string &str);
};
//-------------------------------------------------------------------------------------------------
// Генератор кода для абстрактных инструкций.
// Накапливает необходимые значения в соответствующих строках.

//-------------------------------------------------------------------------------------------------
// Класс Generator. Собирает все воедино для единицы компиляции
struct FullGen: AbstractGen {
    void Generate(std::string &str);
};

//-------------------------------------------------------------------------------------------------
// Генератор кода приложения
// Используется для формирования кода, запускающего программу
struct ApplicationGen: AbstractGen {
    std::string appCode;       // строка с порождаемым кодом
    void Generate(std::string &str);
};

#endif // __GENERATOR__
