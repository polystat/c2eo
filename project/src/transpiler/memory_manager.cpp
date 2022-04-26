#include <iostream>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include "memory_manager.h"

using namespace std;
using namespace clang;


Variable MemoryManager::Add(const VarDecl* id,
                            size_t size,
                            const std::string &type,
                            std::string alias,
                            EOObject value,
                            std::string local_name,
                            size_t shift,
                            bool is_initialized) {

  Variable var = {id, pointer, size, type, std::move(alias), value,
                  std::move(local_name), shift, type.substr(2), is_initialized};
  // TODO fix this plug (rework for check value == EoObject::PLUG)
  if (var.value.name.empty())
    var.value.name = "plug";
  variables.push_back(var);
  pointer += size;
  return var;
}

Variable MemoryManager::AddExternal(const VarDecl* id,
                            size_t size,
                            const std::string &type,
                            std::string alias,
                            EOObject value,
                            std::string local_name,
                            size_t shift,
                            bool is_initialized) {

  Variable var = {id, 999999, size, type, std::move(alias), value,
                  std::move(local_name), shift, type.substr(2), false};
  // TODO fix this plug (rework for check value == EoObject::PLUG)
  if (var.value.name.empty())
    var.value.name = "plug";
  // Необходимо проверить на уже наличие, т.к. внешние описания допускают дублирование
  auto place = std::find_if(variables.begin(), variables.end(), [var](Variable x) { return x.alias == var.alias; });
  if (place == variables.end()) {
    variables.push_back(var);
  }
  else {
    *place = var;
  }

  //pointer += size;
  return var;
}

bool MemoryManager::Empty() {
  return variables.empty();
}

size_t MemoryManager::RealMemorySize() {
  size_t result = 0;
  for (const auto& v : variables) {
    if(v.alias.substr(0, 2) != "e-") {
      result += v.size;
    }
  }
  return result;
}

std::vector<Variable>::const_iterator MemoryManager::begin() const {
  return variables.begin();
}

std::vector<Variable>::const_iterator MemoryManager::end() const {
  return variables.end();
}

const Variable &MemoryManager::GetVarByID(const VarDecl* id) const {
  auto res = find_if(variables.begin(), variables.end(),
                     [id](const Variable &x) { return x.id == id; });
  if (res == variables.end())
    throw invalid_argument("exception: element with id " + to_string(reinterpret_cast<uint64_t>(id)) + " not found");
  return *res;
}

EOObject MemoryManager::GetEOObject() const {
  EOObject res{"ram"};
  res.postfix = name;
  res.nested.emplace_back(to_string(mem_size), EOObjectType::EO_LITERAL);
  return res;
}

void MemoryManager::RemoveAllUsed(const std::vector<Variable>& all_local) {
  for (const auto& var: all_local) {
    pointer -= var.size;
//     std::cout << "pointer = " << pointer << "\n";
    variables.erase(find(variables.begin(), variables.end(), var));
  }
}

// Поиск имен внешних объявлений, совпадающих с глобальными именами
// и установка в одинаковое значение их адресов в глобальной памяти
void MemoryManager::SetExtEqGlob() {
  for(auto& var: variables) {
    if(var.alias.substr(0, 2) == "e-") {
      std::string realName = var.alias.substr(2, var.alias.size());
      auto place = std::find_if(variables.begin(), variables.end(),
                  [realName](Variable x) { return x.alias == "g-"+realName; });
      if((place != variables.end())) {
//          cout << "     " << (*place).alias << "    " << realName << "\n";
        var.position = (*place).position;
      }
    }
  }
}

EOObject Variable::GetInitializer() const {
  if (!is_initialized)
    return EOObject(EOObjectType::EO_EMPTY);
  EOObject res("write");
  if ((type_postfix.length() < 3 || type_postfix.substr(0, 3) != "st-") && type_postfix != "undefinedtype" &&
      type_postfix != "char") // todo char!?
    res.name += "-as-" + type_postfix;
  res.nested.emplace_back(alias);
  // TODO if value will be EOObject then code below changed. (rework for check value == EoObject::PLUG)
  if (value.name == "plug")
      // Probably just emplace value.
    res.nested.emplace_back(EOObjectType::EO_PLUG);
  else
      // Probably just emplace value.
    res.nested.emplace_back(value.name, EOObjectType::EO_LITERAL);
  return res;
}

EOObject Variable::GetAddress(const string& mem_name) const {
  EOObject addr("address", alias);
  if (!mem_name.empty())
    addr.nested.emplace_back(mem_name);
  if (!local_pointer.empty()) {
    EOObject shift_obj("add");
    shift_obj.nested.emplace_back(local_pointer);
    // TODO may be, this don't work with dynamic memory allocation, but probably also should work
    shift_obj.nested.emplace_back(to_string(position - shift), EOObjectType::EO_LITERAL);
    addr.nested.push_back(shift_obj);
  } else {
    addr.nested.emplace_back(to_string(position), EOObjectType::EO_LITERAL);
  }
  return addr;
}

bool Variable::operator==(const Variable& var) const {
  return this->id == var.id;
}
