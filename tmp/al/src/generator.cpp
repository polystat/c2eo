#include <sstream>
#include <iostream>
#include <string>
#include <fstream>

#include "generator.h"


//--------------------------------------------------------------------------------------------------

SpaceGen* AbstractGen::globalSpaceGenPtr = nullptr;
SpaceGen* AbstractGen::staticSpaceGenPtr = nullptr;

//--------------------------------------------------------------------------------------------------
void VarGen::Generate(std::string &str) {
    str = type + " ";
    str += value;
    str += " > ";
    str += name;
}

// void VarGen::GenValue(std::string &str) {
//     str = name;
//     str += ".write ";
//     str += value;
// }

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
    str = R""""(+package c2eo

+alias c2eo.global
+alias c2eo.testVarDeclInt05

+alias org.eolang.io.stdout
+alias org.eolang.txt.sprintf

[args...] > app
  seq > @
    global args > g!
    testVarDeclInt05 args > m!
    stdout
      sprintf
        "%s %s %s\n"
        g.g_intVar01.toString
        m.gs_intVar00.toString
        m.ls_intVar02.toString
)"""";
}

//--------------------------------------------------------------------------------------------------
void FullGen::Generate(std::string &str) {
    
}



