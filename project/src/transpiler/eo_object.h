#ifndef C2EO_SRC_TRANSPILER_EO_OBJECT_H_
#define C2EO_SRC_TRANSPILER_EO_OBJECT_H_

#include <ostream>
#include <vector>
#include <string>

enum class EOObjectType {
  EO_EMPTY,
  EO_COMPLETE,
  EO_LITERAL,
  EO_ABSTRACT,
  EO_PLUG,
};

struct EOObject {
 public:
  EOObject() = default;

  explicit EOObject(EOObjectType type);

  // Create simple complete Object
  explicit EOObject(std::string name);

  // Create simple object, may be used for literal
  EOObject(std::string name, EOObjectType type);

  // create complete name_ with body
  EOObject(std::string name, std::string postfix);

  // Add nested object to vector of nested
  __attribute__((unused)) void AddNested(const EOObject &obj);

  std::vector<std::string> arguments;
  std::string name;
  std::string prefix;
  std::string postfix;
  EOObjectType type = EOObjectType::EO_EMPTY;
  std::vector<EOObject> nested;

  friend std::ostream &operator<<(std::ostream &os, const EOObject &obj);

 private:
  static auto GetSpaceIndent();
  inline static int indent = 0;
};

#endif // C2EO_SRC_TRANSPILER_EO_OBJECT_H_
