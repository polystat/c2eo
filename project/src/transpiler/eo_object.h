#ifndef __EO_OBJECT__
#define __EO_OBJECT__

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
    EOObject() {name = "";}

    explicit EOObject(EOObjectType type) : type(type) {}

  // Create simple complete Object
  explicit EOObject(std::string name) :
      name(std::move(name)),
      type(EOObjectType::EO_COMPLETE) {}

  // Create simple object, may be used for literal
  EOObject(std::string name, EOObjectType type) : name(std::move(name)), type(type) {}

  // create complete name with body
  EOObject(std::string name, std::string postfix) :
      name(std::move(name)),
      postfix(std::move(postfix)),
      type(EOObjectType::EO_COMPLETE) {}

  // Add nested object to vector of nested
  void AddNested(const EOObject &obj);

  // Добавление вложенного объекта в голову вектора
  void AddToHeadInNested(const EOObject &obj);

  std::vector<std::string> arguments;
  std::string name;
  std::string prefix;
  std::string postfix;
  EOObjectType type;
  std::vector<EOObject> nested;

  friend std::ostream &operator<<(std::ostream &os, const EOObject &obj);

private:
  static auto GetSpaceIndent();

  static int indent;
};


EOObject createSeq();


#endif // __EO_OBJECT__
