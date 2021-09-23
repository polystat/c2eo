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

//--------------------------------------------------------------------------------------------------
void GlobalSpaceGen::Generate(std::string &str) {
    str =
        "+package c2eo\n\n"
        "+alias varint c2eo.varInt\n"
        "+alias varint c2eo.varFloat\n\n"

        "[arg] > global\n";

    // Формирование списка глобальных объектов
    std::string strObj = "";
    for(auto globalObject: globalObjects) {
        globalObject->Generate(strObj);
        str += "  ";
        str += strObj;
        str += "\n";
    }
    str += "\n";
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
