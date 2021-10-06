#include <iostream>
#include <string>

#include "testing.h"
#include "generator.h"

void GlobalVarTest::Print(std::string &str, std::vector<AbstractGen*> *globalObjects) {
    str.insert(0,"+alias sprintf org.org.eolang.txt.sprintf\n"
                        "+alias stdout org.org.eolang.io.stdout\n\n");

    for ( auto obj : *globalObjects ) {
        GlobalVarGen *g = static_cast<GlobalVarGen*>(obj);

        str += "    stdout ";
        str += g->name;
        str += ".toString\n";
    }
}