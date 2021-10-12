#include <sstream>
#include <iostream>
#include <string>
#include <fstream>

#include "generator.h"


//--------------------------------------------------------------------------------------------------
// Зачем??
SpaceGen* AbstractGen::globalSpaceGenPtr = nullptr;
SpaceGen* AbstractGen::localStaticSpaceGenPtr = nullptr;
SpaceGen* AbstractGen::globalStaticSpaceGenPtr = nullptr;

//--------------------------------------------------------------------------------------------------
void VarGen::Generate(std::string &str) {
    str = type;
    str += " > ";
    str += name;
}

void VarGen::GenValue(std::string &str) {
    str = name;
    str += ".set ";
    str += value;
}

//--------------------------------------------------------------------------------------------------
void GlobalFuncGen::Generate(std::string &str) {
    // Первоначально осуществляется генерация списка атрибутов
    str = "[";
    if(name == "main") {
        str += "arg] > main\n";
    } else {
        for(auto paramName: paramNames) {
            str += paramName;
            str += ", ";
        }
        str += "ret_param_xxxx] > ";
        str += name;
        str += "\n";
    }
    // Далее идет формирование тела функции, которое пока не прописано
    str += "    seq > @\n";
    str += "      ...\n";
}

void GlobalFuncGen::GenValue(std::string &str) {
    if(name == "main") {
        str += "\n    main arg\n";
    }
}

//--------------------------------------------------------------------------------------------------
void SpaceGen::Generate(std::string &str) {
    str = "";
    // Формирование списка глобальных объектов
    std::string strObj = "";
    for(auto globalObject: globalObjects) {
        globalObject->Generate(strObj);
        str += "  ";
        str += strObj;
        str += "\n";
    }
}

void SpaceGen::GenValue(std::string &str) {
    // Формирование списка инициализаций
    for(auto globalObject: globalObjects) {
        std::string strInit = "";
        globalObject->GenValue(strInit);
        str += "    ";
        str += strInit;
        str += "\n";
    }
    //str += "\n";
}

//........................................................................
// Добавление очередного объекта к глобальному пространству
void SpaceGen::Add(AbstractGen* obj) {
    globalObjects.push_back(obj);
}

SpaceGen::~SpaceGen() {
    globalObjects.clear();
}

//--------------------------------------------------------------------------------------------------
void ApplicationGen::Generate(std::string &str) {
    str = 
        "+package c2eo\n\n"
        "+alias global c2eo.global\n\n"

        "[args...] > app\n"
        "  seq > @\n"
        "    global args\n";
}

//--------------------------------------------------------------------------------------------------
void FullGen::Generate(std::string &str) {
    
}



