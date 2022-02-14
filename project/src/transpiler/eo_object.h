#ifndef __EO_OBJECT__
#define __EO_OBJECT__

#include <ostream>
#include <vector>
#include <string>


struct EOBase{
  virtual void Generate(std::ostream &out) = 0;
};

struct EOVarRef : public EOBase {
  uint64_t id;
};

struct EOUnit : public EOBase {
  std::string value;
};

struct EOData : public EOUnit { };

struct EOName : public EOUnit { };



struct EOObject : public EOBase{
  std::vector<EOBase*> nested;
  std::string suffix;
};

struct EOAbstractObject : public EOObject{
  std::vector<std::string> attributes;
};

struct EOCompleteObject : public EOObject{
  std::string head;
};



#endif //__EO_OBJECT__
