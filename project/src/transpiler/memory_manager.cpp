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

#include "src/transpiler/memory_manager.h"

#include <algorithm>
#include <exception>
#include <iomanip>
#include <stdexcept>
#include <utility>

#include "src/transpiler/transpile_helper.h"
#include "src/transpiler/unit_transpiler.h"
extern UnitTranspiler transpiler;
Variable MemoryManager::Add(const clang::VarDecl *id, int64_t typeInfoID,
                            const std::string &alias, EOObject value,
                            std::string local_name, size_t shift,
                            bool is_initialized) {
  auto res = find_if(variables_.begin(), variables_.end(),
                     [id](const Variable &x) { return x.id == id; });
  if (res != variables_.end()) {
    return *res;
  }
  std::string unique_alias;
  if (duplicates[alias] > 0) {
    unique_alias = alias + "-" + std::to_string(duplicates[alias]);
  } else {
    unique_alias = alias;
  }
  duplicates[alias]++;
  Variable var = {id,
                  pointer_,
                  typeInfoID,
                  std::move(unique_alias),
                  std::move(value),
                  std::move(local_name),
                  shift,
                  is_initialized};
  // TODO(nkchuykin) fix this plug (rework for check value == EoObject::PLUG)
  if (var.value.name.empty()) {
    var.value.name = "plug";
  }
  variables_.push_back(var);
  pointer_ += transpiler.type_manger_.GetById(typeInfoID).GetSizeOfType();
  return var;
}

Variable MemoryManager::AddExternal(
    const clang::VarDecl *id, int64_t typeInfoID, std::string alias,
    EOObject value, std::string local_name, size_t shift,
    __attribute__((unused)) bool is_initialized) {
  Variable var = {id,
                  some_non_zero_position,
                  typeInfoID,
                  std::move(alias),
                  std::move(value),
                  std::move(local_name),
                  shift,
                  false};
  // TODO(nkchuykin) fix this plug (rework for check value == EoObject::PLUG)
  if (var.value.name.empty()) {
    var.value.name = "plug";
  }
  auto place =
      std::find_if(variables_.begin(), variables_.end(),
                   [var](const Variable &x) { return x.alias == var.alias; });
  if (place == variables_.end()) {
    variables_.push_back(var);
  } else {
    *place = var;
  }

  // pointer_ += size;
  return var;
}

bool MemoryManager::Empty() { return variables_.empty(); }

size_t MemoryManager::GetFreeSpacePointer() const { return pointer_; }

std::vector<Variable>::const_iterator MemoryManager::begin() const {
  return variables_.begin();
}

std::vector<Variable>::const_iterator MemoryManager::end() const {
  return variables_.end();
}

template <typename T>
std::string int_to_hex(T i) {
  std::stringstream stream;
  stream << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex
         << i;
  return stream.str();
}

const Variable &MemoryManager::GetVarById(const clang::VarDecl *id) const {
  //  TypeSimpl typeInfo =
  //  transpiler.type_manger_.Add(id->getType().getTypePtrOrNull()); if
  //  (typeInfo.id==-1){
  //    return ;
  //  }
  auto res = find_if(variables_.begin(), variables_.end(),
                     [id](const Variable &x) { return x.id == id; });
  if (res == variables_.end()) {
    id->dump();
    throw std::invalid_argument("exception: element with id " +
                                int_to_hex(reinterpret_cast<uint64_t>(id)) +
                                " not found");
  }
  return *res;
}

EOObject MemoryManager::GetEOObject() const {
  EOObject res{"ram"};
  res.postfix = name_;
  res.nested.emplace_back(std::to_string(mem_size_), EOObjectType::EO_LITERAL);
  return res;
}

void MemoryManager::RemoveAllUsed(const std::vector<Variable> &all_local) {
  for (const auto &var : all_local) {
    auto var_in_memory = find(variables_.begin(), variables_.end(), var);
    if (var_in_memory != variables_.end()) {
      pointer_ -= transpiler.type_manger_.GetById(var_in_memory->typeInfoID)
                      .GetSizeOfType();
      variables_.erase(var_in_memory);
    }
  }
}

void MemoryManager::SetExtEqGlob() {
  for (auto &var : variables_) {
    if (var.alias.substr(0, 2) == "e-") {
      const std::string real_name = var.alias.substr(2, var.alias.size());
      auto place = std::find_if(variables_.begin(), variables_.end(),
                                [real_name](const Variable &x) {
                                  return x.alias == "g-" + real_name;
                                });
      if ((place != variables_.end())) {
        var.position = (*place).position;
      }
    }
  }
}

void MemoryManager::ShiftMemoryLimitPointer(size_t shift) {
  mem_limit_ -= static_cast<int>(shift);
}
size_t MemoryManager::GetMemoryLimitPointer() const { return mem_limit_; }

EOObject Variable::GetInitializer() const {
  if (value.type == EOObjectType::EO_EMPTY && value.name == "*") {
    return ReplaceEmpty(value, {alias, EOObjectType::EO_LITERAL});
  }
  if (!is_initialized) {
    return EOObject(EOObjectType::EO_EMPTY);
  }
  EOObject res("write");
  EOObject constData{"write"};
  EOObject _value = value;
  const TypeSimpl typeInfo = transpiler.type_manger_.GetById(typeInfoID);
  if (typeInfo.name == "ptr" && value.nested.empty()) {
    const TypeSimpl element_type =
        transpiler.type_manger_.GetById(typeInfo.subTypeId);
    if (element_type.name != "undefinedtype") {
      uint64_t type_size = 0;
      if (element_type.name == "int8") {
        constData.name += "-as-string";
        type_size = value.name.length() - 1;
      } else {
        if (element_type.typeStyle != ComplexType::RECORD &&
            element_type.typeStyle != ComplexType::ARRAY &&
            element_type.typeStyle != ComplexType::PHANTOM) {
          constData.name += "-as-" + element_type.name;
        }
        type_size = typeInfo.GetSizeOfType();
      }
      {
        EOObject address{"address"};
        address.nested.emplace_back("global-ram");
        transpiler.glob_.ShiftMemoryLimitPointer(type_size);
        address.nested.emplace_back(
            std::to_string(transpiler.glob_.GetMemoryLimitPointer()),
            EOObjectType::EO_LITERAL);
        constData.nested.push_back(address);
        constData.nested.push_back(value);
        _value = EOObject{"addr-of"};
        _value.nested.push_back(address);
      }
    }
  }
  if ((typeInfo.name != "undefinedtype" && !typeInfo.name.empty() &&
       typeInfo.typeStyle != ComplexType::RECORD &&
       typeInfo.typeStyle != ComplexType::ARRAY) ||
      typeInfo.name == "string") {
    res.name += "-as-" + typeInfo.name;
  }
  res.nested.emplace_back(alias);
  if (value.type == EOObjectType::EO_PLUG) {
    // Probably just emplace value.
    res.nested.emplace_back(EOObjectType::EO_PLUG);
  } else if (value.type == EOObjectType::EO_EMPTY) {
    res.nested.push_back(_value.nested[0]);
    constData = _value.nested[1];
  } else {
    res.nested.push_back(_value);
  }
  if (constData.nested.empty()) {
    return res;
  }
  EOObject ret{EOObjectType::EO_EMPTY};
  ret.nested.push_back(constData);
  ret.nested.push_back(res);
  return ret;
}

EOObject Variable::GetAddress(const std::string &mem_name) const {
  EOObject address("address", alias);
  if (!mem_name.empty()) {
    address.nested.emplace_back(mem_name);
  }
  if (!local_pointer.empty()) {
    EOObject shift_obj("plus");
    shift_obj.nested.emplace_back(local_pointer);
    // TODO(nkchuykin) may be, this doesn't work with dynamic memory allocation,
    // but probably also should work
    shift_obj.nested.emplace_back(std::to_string(position - shift),
                                  EOObjectType::EO_LITERAL);
    address.nested.push_back(shift_obj);
  } else {
    address.nested.emplace_back(std::to_string(position),
                                EOObjectType::EO_LITERAL);
  }
  return address;
}

bool Variable::operator==(const Variable &var) const {
  return this->id == var.id;
}
