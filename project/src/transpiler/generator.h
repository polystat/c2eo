#pragma once
#ifndef __GENERATOR__
#define __GENERATOR__

#include <string>
#include <sstream>
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
    static std::map<uint64_t ,std::string> identifiers;
    static int shift;

    enum GenKind{
        GK_VarGen,
        GK_StmtGen,
        GK_UnaryStmtGen,
        GK_EmptyStmtGen,
        GK_LiteralStmtGen,
        GK_LastUnaryStmtGen,
        GK_BinaryStmtGen,
        GK_ObjectGen,
        GK_MultilineStmtGen,
        GK_FuncGen,
        GK_CmpStmtGen,
        GK_IfStmtGen,
        GK_WhileStmtGen,
        GK_DoWhileStmtGen,
        GK_LastMultilineStmtGen,
        GK_LastStmtGen,
        GK_SpaceGen,
        GK_SourceGen,
        GK_RecordDecl,
        //GK_FieldGen
    };

    const GenKind Kind;

    GenKind getKind() const {return  Kind;}

    friend std::ostream& operator<<(std::ostream & out, AbstractGen* generator)
    {
        if (generator)
            generator->Generate(out);
        return out;
    }

    explicit AbstractGen(GenKind K) : Kind(K) {}
    virtual void Generate(std::ostream &out) = 0;
    virtual ~AbstractGen() = default;
};


//-------------------------------------------------------------------------------------------------
// Генератор кода для объявления переменных.
// Накапливает необходимые значения в соответствующих строках.
struct VarGen: AbstractGen {
    std::string name;       // идентификатор переменной
    std::string type;       // тип переменной
    std::string value;      // значение переменной
    void Generate(std::ostream &out) override;

    VarGen(): AbstractGen(GenKind::GK_VarGen) {}

    static bool classof(const AbstractGen *S) {
        return S->getKind() == GK_VarGen;
    }

};


/*//-------------------------------------------------------------------------------------------------
// Генератор кода для полей объектов.
// Накапливает необходимые значения в соответствующих строках.
struct FieldGen: VarGen {
    size_t count = 0;
    static bool classof(const AbstractGen *S) {
        return S->getKind() == GK_FieldGen;
    }
};*/

//-------------------------------------------------------------------------------------------------
// Генератор кода для структур и объединений.
// Накапливает необходимые значения в соответствующих строках.
struct RecordGen: AbstractGen {
    std::string name;
    std::string type;
    std::vector<RecordGen*> fields;
    size_t count = 0;
    virtual void Generate(std::ostream &out);
//    virtual void GenValue(std::string &str);
    RecordGen(): AbstractGen(GenKind::GK_RecordDecl) {}
    static bool classof(const AbstractGen *S) {
        return S->getKind() == GK_RecordDecl;
    }
};


//-------------------------------------------------------------------------------------------------
// Генератор кода для абстрактных инструкций.
// Накапливает необходимые значения в соответствующих строках.
struct StmtGen : AbstractGen {
    std::string value;
    std::string postfix;
    static std::string getIndentSpaces();

    static bool classof(const AbstractGen *S) {
        return S->getKind() >= GK_StmtGen &&
               S->getKind() <= GK_LastStmtGen;
    }

protected:
    explicit StmtGen(GenKind kind): AbstractGen(kind) { }

};

struct MultiLineStmtGen: StmtGen {
    std::vector<StmtGen*> statements;

    void Add(StmtGen* stmt);
    virtual void Generate(std::ostream &out);


    MultiLineStmtGen(): StmtGen(GenKind::GK_MultilineStmtGen){};
    static bool classof(const AbstractGen *S) {
        return S->getKind() >= GK_MultilineStmtGen &&
               S->getKind() <= GK_LastMultilineStmtGen;
    }

    ~MultiLineStmtGen();

protected:
    MultiLineStmtGen(GenKind kind): StmtGen(kind){};

};

//-------------------------------------------------------------------------------------------------
// Генератор кода для набора инструкций.
// Накапливает необходимые значения в MultiLineStmt.
struct CompoundStmtGen : public MultiLineStmtGen {
    bool is_decorator = false;

    virtual void Generate(std::ostream &out);
    CompoundStmtGen() : MultiLineStmtGen(GK_CmpStmtGen){};

    static bool classof(const AbstractGen *S) {
        return S->getKind() == GK_CmpStmtGen;
    }
};

//-------------------------------------------------------------------------------------------------
// Генератор кода для функций.
// Накапливает необходимые значения в соответствующих строках.
struct FuncGen: MultiLineStmtGen {
    std::string name;       // имя объекта-функции
    std::vector<std::string> paramNames;    // список имен параметров (типы не нужны).
    // TODO нужен ли body?
    CompoundStmtGen* body = nullptr;
    // Возращаемый параметры передается как дополнительный атрибут с некоторым именем,
    // которое не должно нигде встречаться в другом контексте.

    FuncGen() : MultiLineStmtGen(GK_FuncGen){};
    static bool classof(const AbstractGen *S) {
        return S->getKind() == GK_FuncGen;
    }

    void Generate(std::ostream &out) override;

};

//-------------------------------------------------------------------------------------------------
// Генератор кода для бинарных операций.
// Накапливает необходимые значения в соответствующих строках.
struct BinaryStmtGen: StmtGen {
    StmtGen* left = nullptr;
    StmtGen* right = nullptr;

    void Generate(std::ostream &out) override;

    BinaryStmtGen() : StmtGen(GenKind::GK_BinaryStmtGen){}

    static bool classof(const AbstractGen *S) {
        return S->getKind() == GK_BinaryStmtGen;
    }

    ~BinaryStmtGen() override;

    bool isLeftLinear(StmtGen *pGen);
};

//-------------------------------------------------------------------------------------------------
// Генератор кода для унарных операций.
// Накапливает необходимые значения в соответствующих строках.
struct UnaryStmtGen: StmtGen {
    StmtGen* nestedStmt = nullptr;
    std::string op;

    virtual void Generate(std::ostream &out);

    UnaryStmtGen() : StmtGen(GenKind::GK_UnaryStmtGen){}

    static bool classof(const AbstractGen *S) {
        return S->getKind() >= GK_UnaryStmtGen &&
               S->getKind() <= GK_LastUnaryStmtGen;
    }

    ~UnaryStmtGen() override;

protected:
    explicit UnaryStmtGen(GenKind kind) : StmtGen(kind) {};
};


struct  EmptyStmtGen: UnaryStmtGen{

    void Generate(std::ostream &out) override {}

    EmptyStmtGen() : UnaryStmtGen(AbstractGen::GK_EmptyStmtGen){}

    static bool classof(const AbstractGen *S) {
        return S->getKind() == AbstractGen::GK_EmptyStmtGen;
    }
};

struct  LiteralStmtGen: UnaryStmtGen{

    void Generate(std::ostream &out) override ;

    LiteralStmtGen() : UnaryStmtGen(AbstractGen::GK_LiteralStmtGen){}

    static bool classof(const AbstractGen *S) {
        return S->getKind() == AbstractGen::GK_LiteralStmtGen;
    }
};




struct SourceGen:AbstractGen{
    SpaceGen* glob = nullptr;
    SpaceGen* stat = nullptr;

    ~SourceGen();

};



// Накапливает необходимые значения в соответствующих строках.

//-------------------------------------------------------------------------------------------------
// Генератор кода для глобального пространства
// Наряду с константной оберткой обеспечивает запись глобальных объектов
struct SpaceGen: AbstractGen {
    std::vector<AbstractGen*> objects;
    std::string space;      // строка с собранным глобальным пространством

    // Добавление очередного объекта к глобальному пространству
    void Add(AbstractGen*);
    void Generate(std::ostream &out);

    SpaceGen(): AbstractGen(GK_SpaceGen){}


    static bool classof(const AbstractGen *S) {
        return S->getKind() == GK_SpaceGen;
    }

    ~SpaceGen();
};



//-------------------------------------------------------------------------------------------------
// Генератор кода для условного оператора
// Наряду с константной оберткой обеспечивает запись глобальных объектов
struct IfStmtGen: MultiLineStmtGen {
    // Добавление очередного объекта к глобальному пространству

    void Generate(std::ostream &out) override;

    IfStmtGen(): MultiLineStmtGen(GK_IfStmtGen){}


    static bool classof(const AbstractGen *S) {
        return S->getKind() == GK_IfStmtGen;
    }

    ~IfStmtGen() override;
};

//-------------------------------------------------------------------------------------------------
// Генератор кода для оператора цикла while
// Наряду с константной оберткой обеспечивает запись глобальных объектов
struct WhileStmtGen: MultiLineStmtGen {
    // Добавление очередного объекта к глобальному пространству

    void Generate(std::ostream &out) override;

    WhileStmtGen(): MultiLineStmtGen(GK_WhileStmtGen){}


    static bool classof(const AbstractGen *S) {
        return S->getKind() == GK_WhileStmtGen;
    }

    ~WhileStmtGen() override;
};

//-------------------------------------------------------------------------------------------------
// Генератор кода для оператора цикла do-while
// Наряду с константной оберткой обеспечивает запись глобальных объектов
struct DoWhileStmtGen: MultiLineStmtGen {
    // Добавление очередного объекта к глобальному пространству

    void Generate(std::ostream &out) override;

    DoWhileStmtGen(): MultiLineStmtGen(GK_DoWhileStmtGen){}


    static bool classof(const AbstractGen *S) {
        return S->getKind() == GK_WhileStmtGen;
    }

    ~DoWhileStmtGen() override;
};

struct ObjectStmtGen: StmtGen {

    StmtGen* body;
    // Добавление очередного объекта к глобальному пространству
    void Generate(std::ostream &out) override;

    ObjectStmtGen(): StmtGen(GK_ObjectGen){}


    static bool classof(const AbstractGen *S) {
        return S->getKind() == GK_ObjectGen;
    }

    ~ObjectStmtGen() override;
};

#endif // __GENERATOR__
