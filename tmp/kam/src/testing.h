#ifndef C2EO_TESTING_H
#define C2EO_TESTING_H

#include <string>
#include <vector>

#include "generator.h"

//----------------------------------------------------------------------------------------------
// Печать всех глобальных переменных в программе.
struct GlobalVarTest {
    void Print(std::string &str, std::vector<AbstractGen*> *globalObjects);
};

#endif //C2EO_TESTING_H