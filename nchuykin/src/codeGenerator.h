#ifndef C2EO_CODE_GENERATOR_H
#define C2EO_CODE_GENERATOR_H

#include <string>
#include <vector>

class  Variable
{
public:
    std::string name;
    std::string type;
    virtual std::string getValue() = 0;
};

class  IntVariable : public Variable
{
public:
    int value;
    std::string getValue() override;
};

class CodeGenerator
{
public:
    static std::vector<Variable*> vars;
    static void getCodeToConsole();
    static void getCodeToFile(const char* filename);
private:
    static std::string getCode();
    ~CodeGenerator();
};


#endif //C2EO_CODE_GENERATOR_H
