#include "eo_object.h"
#include "util.h"

using namespace std;

int EOObject::indent = 0;

EOObject::EOObject(EOObjectType type) : type(type) {
  //   nested.reserve(10000);
  #ifdef TRACEOUT_NEW_EO
  std::cout << *this; // << "\n";
  #endif
}

// Create simple complete Object
EOObject::EOObject(std::string name) :
name(std::move(name)),
type(EOObjectType::EO_COMPLETE) {
  //   nested.reserve(10000);
  #ifdef TRACEOUT_NEW_EO
  std::cout << *this; // << "\n";
  #endif
}

// Create simple object, may be used for literal
EOObject::EOObject(std::string name, EOObjectType type) : name(std::move(name)), type(type) {
  #ifdef TRACEOUT_NEW_EO
  std::cout << *this; // << "\n";
  #endif
}

// create complete name_ with body
EOObject::EOObject(std::string name, std::string postfix) :
name(std::move(name)),
postfix(std::move(postfix)),
type(EOObjectType::EO_COMPLETE) {
  //   nested.reserve(10000);
  #ifdef TRACEOUT_NEW_EO
  std::cout << *this; // << "\n";
  #endif
}

__attribute__((unused)) void EOObject::AddNested(const EOObject &obj) {
  nested.push_back(obj);
}

auto EOObject::GetSpaceIndent() {
  return string(2 * EOObject::indent, ' ');
}

std::ostream &operator<<(ostream &os, const EOObject &obj) {
  if (obj.type == EOObjectType::EO_EMPTY) {
    for (const auto &child: obj.nested) {
      os << child;
    }
    return os;
  }
  os << EOObject::GetSpaceIndent();
  if (obj.type == EOObjectType::EO_PLUG) {
    os << "plug" << "\n";
    return os;
  }
  if (obj.type == EOObjectType::EO_ABSTRACT) {
    os << "[" << StrJoin(obj.arguments) << "]";
  } else {
    if (!obj.prefix.empty()) {
      os << obj.prefix << ".";
    }
    os << obj.name;
  }
  if (!obj.postfix.empty()) {
    os << " > " << obj.postfix;
  }
  os << "\n";
  if (!obj.nested.empty()) {
    EOObject::indent++;
    for (const auto &child: obj.nested) {
      os << child;
    }
    EOObject::indent--;
  }
  return os;
}


