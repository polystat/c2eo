/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021-2022 c2eo team
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
#pragma ide diagnostic ignored "fuchsia-statically-constructed-objects"
#ifndef PROJECT_SRC_TRANSPILER_ALIASES_H_
#define PROJECT_SRC_TRANSPILER_ALIASES_H_

#include "map"
#include "set"
#include "string"

std::set<std::string> known_types = {
    "memory",
    "seq",
    "$",
    "do-while-loop-label-1.forward TRUE",
    "do-while-loop-label-2.forward TRUE",
    "end.forward TRUE",
    "for-loop-label-1.forward TRUE",
    "for-loop-label-2.forward TRUE",
    "while-loop-label.backward",
    "while-loop-label.forward TRUE",
};

std::map<std::string, std::string> known_aliases = {
    {"addr-of", "+alias c2eo.coperators.addr-of"},
    {"addrcpy", "+alias c2eo.coperators.addrcpy"},
    {"address", "+alias c2eo.coperators.address"},
    {"addrmemcpy", "+alias c2eo.coperators.addrmemcpy"},
    {"and", "+alias c2eo.coperators.and"},
    {"as-bool", "+alias c2eo.coperators.as-bool"},
    {"as-float32", "+alias c2eo.coperators.as-float32"},
    {"as-float64", "+alias c2eo.coperators.as-float64"},
    {"as-int8", "+alias c2eo.coperators.as-int8"},
    {"as-int16", "+alias c2eo.coperators.as-int16"},
    {"as-int32", "+alias c2eo.coperators.as-int32"},
    {"as-int64", "+alias c2eo.coperators.as-int64"},
    {"as-uint8", "+alias c2eo.coperators.as-uint8"},
    {"as-uint16", "+alias c2eo.coperators.as-uint16"},
    {"as-uint32", "+alias c2eo.coperators.as-uint32"},
    {"as-uint64", "+alias c2eo.coperators.as-uint64"},
    {"bit-and", "+alias c2eo.coperators.bit-and"},
    {"bit-not", "+alias c2eo.coperators.bit-not"},
    {"bit-or", "+alias c2eo.coperators.bit-or"},
    {"bit-xor", "+alias c2eo.coperators.bit-xor"},
    {"div", "+alias c2eo.coperators.div"},
    {"eq", "+alias c2eo.coperators.eq"},
    {"goto", "+alias org.eolang.goto"},
    {"gt", "+alias c2eo.coperators.gt"},
    {"gte", "+alias c2eo.coperators.gte"},
    {"if", "+alias c2eo.coperators.if"},
    {"if-else", "+alias c2eo.coperators.if-else"},
    {"lt", "+alias c2eo.coperators.lt"},
    {"lte", "+alias c2eo.coperators.lte"},
    {"memadrcpy", "+alias c2eo.coperators.memadrcpy"},
    {"meta", "+alias c2eo.coperators.meta"},
    {"minus", "+alias c2eo.coperators.minus"},
    {"mod", "+alias c2eo.coperators.mod"},
    {"neg", "+alias c2eo.coperators.neg"},
    {"neq", "+alias c2eo.coperators.neq"},
    {"not", "+alias c2eo.coperators.not"},
    {"or", "+alias c2eo.coperators.or"},
    {"plug", "+alias c2eo.coperators.plug"},
    {"plus", "+alias c2eo.coperators.plus"},
    {"pos", "+alias c2eo.coperators.pos"},
    {"post-dec-float32", "+alias c2eo.coperators.post-dec-float32"},
    {"post-dec-float64", "+alias c2eo.coperators.post-dec-float64"},
    {"post-dec-int8", "+alias c2eo.coperators.post-dec-int8"},
    {"post-dec-int16", "+alias c2eo.coperators.post-dec-int16"},
    {"post-dec-int32", "+alias c2eo.coperators.post-dec-int32"},
    {"post-dec-int64", "+alias c2eo.coperators.post-dec-int64"},
    {"post-dec-ptr", "+alias c2eo.coperators.post-dec-ptr"},
    {"post-dec-uint8", "+alias c2eo.coperators.post-dec-uint8"},
    {"post-dec-uint16", "+alias c2eo.coperators.post-dec-uint16"},
    {"post-dec-uint32", "+alias c2eo.coperators.post-dec-uint32"},
    {"post-dec-uint64", "+alias c2eo.coperators.post-dec-uint64"},
    {"post-inc-float32", "+alias c2eo.coperators.post-inc-float32"},
    {"post-inc-float64", "+alias c2eo.coperators.post-inc-float64"},
    {"post-inc-int8", "+alias c2eo.coperators.post-inc-int8"},
    {"post-inc-int16", "+alias c2eo.coperators.post-inc-int16"},
    {"post-inc-int32", "+alias c2eo.coperators.post-inc-int32"},
    {"post-inc-int64", "+alias c2eo.coperators.post-inc-int64"},
    {"post-inc-ptr", "+alias c2eo.coperators.post-inc-ptr"},
    {"post-inc-uint8", "+alias c2eo.coperators.post-inc-uint8"},
    {"post-inc-uint16", "+alias c2eo.coperators.post-inc-uint16"},
    {"post-inc-uint32", "+alias c2eo.coperators.post-inc-uint32"},
    {"post-inc-uint64", "+alias c2eo.coperators.post-inc-uint64"},
    {"pre-dec-float32", "+alias c2eo.coperators.pre-dec-float32"},
    {"pre-dec-float64", "+alias c2eo.coperators.pre-dec-float64"},
    {"pre-dec-int8", "+alias c2eo.coperators.pre-dec-int8"},
    {"pre-dec-int16", "+alias c2eo.coperators.pre-dec-int16"},
    {"pre-dec-int32", "+alias c2eo.coperators.pre-dec-int32"},
    {"pre-dec-int64", "+alias c2eo.coperators.pre-dec-int64"},
    {"pre-dec-ptr", "+alias c2eo.coperators.pre-dec-ptr"},
    {"pre-dec-uint8", "+alias c2eo.coperators.pre-dec-uint8"},
    {"pre-dec-uint16", "+alias c2eo.coperators.pre-dec-uint16"},
    {"pre-dec-uint32", "+alias c2eo.coperators.pre-dec-uint32"},
    {"pre-dec-uint64", "+alias c2eo.coperators.pre-dec-uint64"},
    {"pre-inc-float32", "+alias c2eo.coperators.pre-inc-float32"},
    {"pre-inc-float64", "+alias c2eo.coperators.pre-inc-float64"},
    {"pre-inc-int8", "+alias c2eo.coperators.pre-inc-int8"},
    {"pre-inc-int16", "+alias c2eo.coperators.pre-inc-int16"},
    {"pre-inc-int32", "+alias c2eo.coperators.pre-inc-int32"},
    {"pre-inc-int64", "+alias c2eo.coperators.pre-inc-int64"},
    {"pre-inc-ptr", "+alias c2eo.coperators.pre-inc-ptr"},
    {"pre-inc-uint8", "+alias c2eo.coperators.pre-inc-uint8"},
    {"pre-inc-uint16", "+alias c2eo.coperators.pre-inc-uint16"},
    {"pre-inc-uint32", "+alias c2eo.coperators.pre-inc-uint32"},
    {"pre-inc-uint64", "+alias c2eo.coperators.pre-inc-uint64"},
    {"printf", "+alias c2eo.coperators.printf"},
    {"ram", "+alias c2eo.coperators.ram"},
    {"read", "+alias c2eo.coperators.read"},
    {"read-as-address", "+alias c2eo.coperators.read-as-address"},
    {"read-as-bool", "+alias c2eo.coperators.read-as-bool"},
    {"read-as-float32", "+alias c2eo.coperators.read-as-float32"},
    {"read-as-float64", "+alias c2eo.coperators.read-as-float64"},
    {"read-as-int8", "+alias c2eo.coperators.read-as-int8"},
    {"read-as-int16", "+alias c2eo.coperators.read-as-int16"},
    {"read-as-int32", "+alias c2eo.coperators.read-as-int32"},
    {"read-as-int64", "+alias c2eo.coperators.read-as-int64"},
    {"read-as-ptr", "+alias c2eo.coperators.read-as-ptr"},
    {"read-as-string", "+alias c2eo.coperators.read-as-string"},
    {"read-as-uint8", "+alias c2eo.coperators.read-as-uint8"},
    {"read-as-uint16", "+alias c2eo.coperators.read-as-uint16"},
    {"read-as-uint32", "+alias c2eo.coperators.read-as-uint32"},
    {"read-as-uint64", "+alias c2eo.coperators.read-as-uint64"},
    {"shift-left", "+alias c2eo.coperators.shift-left"},
    {"shift-right", "+alias c2eo.coperators.shift-right"},
    {"times", "+alias c2eo.coperators.times"},
    {"write", "+alias c2eo.coperators.write"},
    {"write-as-bool", "+alias c2eo.coperators.write-as-bool"},
    {"write-as-float32", "+alias c2eo.coperators.write-as-float32"},
    {"write-as-float64", "+alias c2eo.coperators.write-as-float64"},
    {"write-as-int8", "+alias c2eo.coperators.write-as-int8"},
    {"write-as-int16", "+alias c2eo.coperators.write-as-int16"},
    {"write-as-int32", "+alias c2eo.coperators.write-as-int32"},
    {"write-as-int64", "+alias c2eo.coperators.write-as-int64"},
    {"write-as-ptr", "+alias c2eo.coperators.write-as-ptr"},
    {"write-as-string", "+alias c2eo.coperators.write-as-string"},
    {"write-as-uint8", "+alias c2eo.coperators.write-as-uint8"},
    {"write-as-uint16", "+alias c2eo.coperators.write-as-uint16"},
    {"write-as-uint32", "+alias c2eo.coperators.write-as-uint32"},
    {"write-as-uint64", "+alias c2eo.coperators.write-as-uint64"},
};

#endif  // PROJECT_SRC_TRANSPILER_ALIASES_H_

#pragma clang diagnostic pop
