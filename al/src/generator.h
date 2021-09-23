#ifndef __GENERATOR__
#define __GENERATOR__

#include <string>
#include <vector>

//-------------------------------------------------------------------------------------------------
// Абстрактный генератор кода на всякий случай
// Возможно придется связывать динамически разные генераторы в общий список или массив
// Сюда же, возможно, добавятся общие статические объекты
struct AbstractGen {
    virtual void Generate(std::string &str) = 0;
    virtual void GenValue(std::string &str) {}
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
};

//-------------------------------------------------------------------------------------------------
// Генератор кода для глобальных переменных
// накапливая необходимые значения в соответствующих строках
struct GlobalVarGen: AbstractGen {
    static GlobalSpaceGen* globalSpaceGenPtr;
    std::string name;       // идентификатор переменной
    std::string type;       // тип переменной
    std::string value;      // значение переменной
    void Generate(std::string &str);
    void GenValue(std::string &str);
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
