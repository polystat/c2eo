#include "memory_manager.h"
#include "unit_transpiler.h"
#include "transpile_helper.h"
#include "vardecl.h"

#include <algorithm>
#include <exception>
#include <stdexcept>
#include <utility>

using namespace std;
using namespace clang;

Variable MemoryManager::Add(const VarDecl *id, size_t size,
                            const std::string &type, std::string alias,
                            EOObject value, std::string local_name,
                            size_t shift, bool is_initialized) {
  if (duplicates[alias] > 0) {
    alias += "-" + to_string(duplicates[alias]);
  }
  duplicates[alias]++;
  Variable var = {id,
                  pointer_,
                  size,
                  type,
                  std::move(alias),
                  std::move(value),
                  std::move(local_name),
                  shift,
                  type.substr(2),
                  is_initialized};
  // TODO fix this plug (rework for check value == EoObject::PLUG)
  if (var.value.name.empty()) {
    var.value.name = "plug";
  }
  variables_.push_back(var);
  pointer_ += size;
  return var;
}

Variable MemoryManager::AddExternal(
    const VarDecl *id, size_t size, const std::string &type, std::string alias,
    EOObject value, std::string local_name, size_t shift,
    __attribute__((unused)) bool is_initialized) {
  Variable var = {id,
                  some_non_zero_position,
                  size,
                  type,
                  std::move(alias),
                  std::move(value),
                  std::move(local_name),
                  shift,
                  type.substr(2),
                  false};
  // TODO fix this plug (rework for check value == EoObject::PLUG)
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

size_t MemoryManager::RealMemorySize() {
  size_t result = 0;
  for (const auto &v: variables_) {
    if (v.alias.substr(0, 2) != "e-") {
      result += v.size;
    }
  }
  return result;
}

std::vector<Variable>::const_iterator MemoryManager::begin() const {
  return variables_.begin();
}

std::vector<Variable>::const_iterator MemoryManager::end() const {
  return variables_.end();
}

const Variable &MemoryManager::GetVarById(const VarDecl *id) const {
  auto res = find_if(variables_.begin(), variables_.end(),
                     [id](const Variable &x) { return x.id == id; });
  if (res == variables_.end()) {
    throw invalid_argument(
        "exception: element with id " +
        to_string(reinterpret_cast<uint64_t>(
                      id))  // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
        + " not found");
  }
  return *res;
}

EOObject MemoryManager::GetEOObject() const {
  EOObject res{"ram"};
  res.postfix = name_;
  res.nested.emplace_back(to_string(mem_size_), EOObjectType::EO_LITERAL);
  return res;
}

void MemoryManager::RemoveAllUsed(const std::vector<Variable> &all_local) {
  for (const auto &var: all_local) {
    pointer_ -= var.size;
    variables_.erase(find(variables_.begin(), variables_.end(), var));
  }
}

void MemoryManager::SetExtEqGlob() {
  for (auto &var: variables_) {
    if (var.alias.substr(0, 2) == "e-") {
      std::string real_name = var.alias.substr(2, var.alias.size());
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

std::vector<EOObject> Variable::GetInitializer() const {
  if (!is_initialized) {
    return {EOObject(EOObjectType::EO_EMPTY)};
  }
  if (value.name == "*") {
    return GetListInitializer(EOObject{alias}, value, id->getType());
  }
  EOObject res("write");
  if ((type_postfix.length() < 3 ||
       (type_postfix.substr(0, 3) != "st-" &&
        type_postfix.substr(0, 3) != "un-"))  // todo recordDecl check
      && type_postfix != "undefinedtype" && type_postfix != "char") {
    {
      {  // todo char!?
        res.name += "-as-" + type_postfix;
      }
    }
  }
  res.nested.emplace_back(alias);
  if (value.type == EOObjectType::EO_PLUG) {
    // Probably just emplace value.
    res.nested.emplace_back(EOObjectType::EO_PLUG);
  } else {
    // Probably just emplace value.
    res.nested.emplace_back(value);
  }
  return {res};
}

EOObject Variable::GetAddress(const string &mem_name) const {
  EOObject address("address", alias);
  if (!mem_name.empty()) {
    address.nested.emplace_back(mem_name);
  }
  if (!local_pointer.empty()) {
    EOObject shift_obj("add");
    shift_obj.nested.emplace_back(local_pointer);
    // TODO may be, this doesn't work with dynamic memory allocation, but
    // probably also should work
    shift_obj.nested.emplace_back(to_string(position - shift),
                                  EOObjectType::EO_LITERAL);
    address.nested.push_back(shift_obj);
  } else {
    address.nested.emplace_back(to_string(position), EOObjectType::EO_LITERAL);
  }
  return address;
}

bool Variable::operator==(const Variable &var) const {
  return this->id == var.id;
}

vector<EOObject> Variable::GetListInitializer(EOObject rootAlias, EOObject listValue, clang::QualType qualType) const {
  std::vector<EOObject> inits;
  extern UnitTranspiler transpiler;
  auto* recordType = transpiler.record_manager_.GetById(qualType->getAsRecordDecl()->getID());
  std::string elementTypeName = "";
  std::map<std::string, std::pair<clang::QualType, size_t>>::iterator recElement;
  size_t elementSize = 0;
  if (qualType->isArrayType()) {
    clang::QualType elementQualType = llvm::dyn_cast<ConstantArrayType>(qualType)->getElementType();
    elementTypeName = GetTypeName(elementQualType);
    elementSize = id->getASTContext().getTypeInfo(elementQualType).Align / byte_size;
  } else if (qualType->isRecordType()) {
    recElement = recordType->fields.begin();
  }
  for (int i = 0; i < value.nested.size(); i++) {
    EOObject shiftedAlias{"add"};
    shiftedAlias.nested.emplace_back(alias);
    if (qualType->isArrayType()) {
      EOObject shift{"mul"};
      shift.nested.emplace_back(to_string(i), EOObjectType::EO_LITERAL);
      shift.nested.emplace_back(to_string(elementSize), EOObjectType::EO_LITERAL);
      shiftedAlias.nested.push_back(shift);
    } else if (qualType->isRecordType()) {
      shiftedAlias.nested.emplace_back(transpiler.record_manager_.GetShiftAlias(
          qualType->getAsRecordDecl()->getID(), recElement->first));
      elementTypeName = GetTypeName(recElement->second.first);
      recElement++;
    }
    if (value.nested[i].name == "*") {
      auto subInits = GetListInitializer(shiftedAlias, value.nested[i], qualType);
      inits.insert(inits.end(), subInits.begin(), subInits.end());
    } else {
      EOObject res("write");
      if (!elementTypeName.empty()) {
        res.name += "-as-" + elementTypeName;
      }
      res.nested.emplace_back(shiftedAlias);
      res.nested.emplace_back(value.nested[i]);
      inits.push_back(res);
    }
  }
  return inits;
}
