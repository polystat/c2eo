#include <sstream>
#include <iostream>
#include <string>
#include <fstream>

#include "generator.h"


//--------------------------------------------------------------------------------------------------
GlobalSpaceGen* AbstractGen::globalSpaceGenPtr = nullptr;

//--------------------------------------------------------------------------------------------------
/*void Debug::PrintAll(std::string &str, std::vector<AbstractGen*>* objects) {
    for(auto obj : *objects){
        GlobalVarGen *g = static_cast<GlobalVarGen*>(obj);
        str += "    stdout (g.";
        str += g->name;
        str += ".toString)\n";
    }
}*/

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
void GlobalFuncGen::Generate(std::string &str) {
    // Первоначально осуществляется генерация списка атрибутов
    str = "[";
    if(name == "main") {
        str += "arg] > main\n";
    } else {
        str = "";
        return;
        /*Duplicate names inside ""*/
        /*
        int i = 0;
        for(auto paramName: paramNames) {
            str += "param_";
            str += paramName;
            {
                // for pointers:
                str += "_";
                str += std::to_string(i);
            }
            str += " ";
            i += 1;
        }
        str += "ret_param] > ";
        str += name;
        str += "\n";*/
    }
    // Далее идет формирование тела функции, которое пока не прописано
    str += "    seq > @\n";
    //str += "      ...\n";
}

void GlobalFuncGen::GenValue(std::string &str) {
    if(name == "main") {
        str += "main arg\n";
    }
}

//--------------------------------------------------------------------------------------------------
void GlobalSpaceGen::Generate(std::string &str) {
    str = "";
    // Формирование списка глобальных объектов
    std::string strObj = "";
    for(auto globalObject: globalObjects) {
        globalObject->Generate(strObj);
        if (!strObj.empty()) {
            str += "  ";
            str += strObj;
            str += "\n";
        }
        //if (globalObject->)
    }
}

void GlobalSpaceGen::GenValue(std::string &str) {
    // Формирование списка инициализаций
    for(auto globalObject: globalObjects) {
        std::string strInit = "";
        globalObject->GenValue(strInit);
        if (!strInit.empty()) {
            str += "    ";
            str += strInit;
            str += "\n";
        }
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

//-------------------------------------------------------------------------------------------
//#ifdef _DEBUG
void GlobalVarGen::ShowValue(std::string &str) {
    str = "    stdout (^.";
    str += name;
    str += ".toString)\n";
}
void GlobalFuncGen::ShowValue(std::string &str) {
    str = "";
}
void GlobalSpaceGen::ShowValue(std::string &str) {
    // Формирование списка инициализаций
    for(auto globalObject: globalObjects) {
        std::string strInit = "";
        globalObject->ShowValue(strInit);
        if (!strInit.empty()) {
            str += "  ";
            str += strInit;
            str += "\n";
            str += "      stdout \"\\n\"\n";
        }
    }
    //str += "\n";
}
//#endif