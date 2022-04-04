#ifndef __ALIASES__
#define __ALIASES__
#include "map"
#include "set"
#include "string"

std::set<std::string> known_types = {
    "memory",
    "seq",
};

std::map<std::string,std::string> known_aliases =
    {
        {"abs", "+alias c2eo.coperators.abs"},
        {"add", "+alias c2eo.coperators.add"},
        {"address", "+alias c2eo.system.address"},
        {"and", "+alias c2eo.coperators.and"},
        {"as-bool", "+alias c2eo.coperators.as-bool"},
        {"as-float64", "+alias c2eo.coperators.as-float64"},
        {"as-int64", "+alias c2eo.coperators.as-int64"},
        {"as-string", "+alias c2eo.coperators.as-string"},
        {"bit-and", "+alias c2eo.coperators.bit-and"},
        {"bit-not", "+alias c2eo.coperators.bit-not"},
        {"bit-or", "+alias c2eo.coperators.bit-or"},
        {"bit-xor", "+alias c2eo.coperators.bit-xor"},
        {"div", "+alias c2eo.coperators.div"},
        {"eq", "+alias c2eo.coperators.eq"},
        {"geq", "+alias c2eo.coperators.geq"},
        {"get", "+alias c2eo.coperators.get"},
        {"greater", "+alias c2eo.coperators.greater"},
        {"if", "+alias c2eo.coperators.if"},
        {"leq", "+alias c2eo.coperators.leq"},
        {"less", "+alias c2eo.coperators.less"},
        {"mod", "+alias c2eo.coperators.mod"},
        {"mul", "+alias c2eo.coperators.mul"},
        {"neg", "+alias c2eo.coperators.neg"},
        {"neq", "+alias c2eo.coperators.neq"},
        {"not", "+alias c2eo.coperators.not"},
        {"or", "+alias c2eo.coperators.or"},
        {"plug", "+alias c2eo.coperators.plug"},
        {"pow", "+alias c2eo.coperators.pow"},
        {"printf", "+alias c2eo.stdio.printf"},
        {"ram", "+alias c2eo.system.ram"},

        {"read-as-bool", "+alias c2eo.coperators.read-as-bool"},
        {"read-as-int32", "+alias c2eo.coperators.read-as-int32"},
        {"read-as-int64", "+alias c2eo.coperators.read-as-int64"},
        {"read-as-float64", "+alias c2eo.coperators.read-as-float64"},
        {"read-as-ptr", "+alias c2eo.coperators.read-as-ptr"},

        {"shift-left", "+alias c2eo.coperators.shift-left"},
        {"shift-right", "+alias c2eo.coperators.shift-right"},
        {"sub", "+alias c2eo.coperators.sub"},
        {"while", "+alias c2eo.coperators.while"},
        {"write", "+alias c2eo.coperators.write"},

        {"post-inc", "+alias c2eo.coperators.post-inc"},
        {"post-dec", "+alias c2eo.coperators.post-dec"},
        {"pre-inc", "+alias c2eo.coperators.pre-inc"},
        {"pre-dec", "+alias c2eo.coperators.pre-dec"},
        {"addr-of", "+alias c2eo.coperators.addr-of"},
        //{"deref-int64", "+alias c2eo.coperators.deref-int64"},
        {"read-as-address", "+alias c2eo.coperators.read-as-address"},
        {"plus", "+alias c2eo.coperators.plus"},
        //{"uno-minus", "+alias c2eo.coperators.uno-minus"},
        {"bit-not", "+alias c2eo.coperators.bit-not"},
        //{"log-not", "+alias c2eo.coperators.log-not"},
        {"real", "+alias c2eo.coperators.real"},
        {"imag", "+alias c2eo.coperators.imag"},
        {"extension", "+alias c2eo.coperators.extension"},
        {"coawait", "+alias c2eo.coperators.coawait"},

        {"pre-inc-int64", "+alias c2eo.coperators.pre-inc-int64"},
        {"post-inc-int64", "+alias c2eo.coperators.post-inc-int64"},
        {"pre-dec-int64", "+alias c2eo.coperators.pre-dec-int64"},
        {"post-dec-int64", "+alias c2eo.coperators.post-dec-int64"},

        {"write-as-int64", "+alias c2eo.coperators.write-as-int64"},
        {"write-as-float64", "+alias c2eo.coperators.write-as-float64"},
        {"write-as-ptr", "+alias c2eo.coperators.write-as-ptr"},
    };

#endif
