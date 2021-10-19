#ifndef __GENERATOR__
#define __GENERATOR__

#include <string>
#include <vector>

//-------------------------------------------------------------------------------------------------
// Абстрактный генератор кода на всякий случай
// Возможно придется связывать динамически разные генераторы в общий список или массив
// Сюда же, возможно, добавятся общие статические объекты
struct GlobalSpaceGen;

struct AbstractGen {
    static GlobalSpaceGen* globalSpaceGenPtr;
    virtual void Generate(std::string &str) = 0;
    virtual void GenValue(std::string &str) {}

#ifdef _DEBUG
    virtual void ShowValue(std::string &str) {}
#endif
};

//-------------------------------------------------------------------------------------------------
// Генератор кода для глобального пространства
// Наряду с константной оберкой обеспечивает запись глобальных объектов
struct GlobalSpaceGen: AbstractGen {
    std::vector<AbstractGen*> globalObjects;
    std::string space;      // строка с собранным глобальным пространством

    // Добавление очередного объекта к глобальному пространству
    void Add(AbstractGen* obj);
    void Generate(std::string &str);
    void GenValue(std::string &str);
#ifdef _DEBUG
    void ShowValue(std::string &str);
#endif
};

//-------------------------------------------------------------------------------------------------
// Генератор кода для глобальных переменных
// Накапливает необходимые значения в соответствующих строках
struct GlobalVarGen: AbstractGen {
    std::string name;       // идентификатор переменной
    std::string type;       // тип переменной
    std::string value;      // значение переменной
    virtual void Generate(std::string &str);
    virtual void GenValue(std::string &str);
#ifdef _DEBUG
    virtual void ShowValue(std::string &str);
#endif
};

/*//-------------------------------------------------------------------------------------------------
// Генератор вывода всех переменных коллекции
// Накапливает необходимые значения в соответствующих строках
struct Debug {
    void PrintAll(std::string &str, std::vector<AbstractGen*> *objects);
};*/

//-------------------------------------------------------------------------------------------------
// Генератор кода для глобальных функций
// Накапливает необходимые значения в соответствующих строках
struct GlobalFuncGen: AbstractGen {
    std::string name;       // имя объекта-функции
    std::vector<std::string> paramNames;    // список имен параметров (типы не нужны)
    // Возращаемый параметры передается как дополнительный атрибут с некоторым именем,
    // которое не должно нигде встречаться в другом контексте.
    virtual void Generate(std::string &str);
    virtual void GenValue(std::string &str);
#ifdef _DEBUG
    virtual void ShowValue(std::string &str);
#endif
};

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
