#include <sstream>
#include <iostream>
#include <string>
#include <fstream>

#include "generator.h"


//--------------------------------------------------------------------------------------------------
GlobalSpaceGen* GlobalVarGen::globalSpaceGenPtr = nullptr;

//--------------------------------------------------------------------------------------------------
void GlobalVarGen::Generate(std::string &str) {
    str = type;
    str += " > ";
    str += name;
}

void GlobalVarGen::GenValue(std::string &str) {
    str = name;
    str += ".set ";
    str += value;
}

//--------------------------------------------------------------------------------------------------
void GlobalSpaceGen::Generate(std::string &str) {
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

void GlobalSpaceGen::GenValue(std::string &str) {
    // Формирование списка инициализаций
    std::string strInit = "";
    for(auto globalObject: globalObjects) {
        globalObject->GenValue(strInit);
        str += "    ";
        str += strInit;
        str += "\n";
    }
    //str += "\n";
}

//........................................................................
// Добавление очередного объекта к глобальному пространству
void GlobalSpaceGen::Add(AbstractGen* obj) {
    globalObjects.push_back(obj);
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
