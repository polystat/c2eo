#include "eo_object.h"
#include "util.h"
using namespace std;

int EOObject::indent = 0;

void EOObject::AddNested(const EOObject& obj) {
  nested.push_back(obj);
}

// Добавление вложенного объекта в голову вектора
void EOObject::AddToHeadInNested(const EOObject& obj) {
  nested.insert(nested.begin(), obj);
}

auto EOObject::GetSpaceIndent() {
  return string(2 * EOObject::indent,' ');
}

std::ostream& operator<<(ostream &os, const EOObject& obj) {
  if (obj.type == EOObjectType::EO_EMPTY)
  {
    for(const auto& child : obj.nested)
    {
      os << child;
    }
    return os;
  }
  os << EOObject::GetSpaceIndent();
  if (obj.type == EOObjectType::EO_PLUG) {
    os << "plug" << "\n";
    return os;
  }
  if(obj.type == EOObjectType::EO_ABSTRACT)
  {
    os << "[" << str_join(obj.arguments)<<"]";
  } else {
    if(!obj.prefix.empty()) {
      os << obj.prefix << ".";
    }
    os << obj.name;
  }
  if (!obj.postfix.empty())
  {
    os << " > " << obj.postfix;
  }
  os << "\n";
  if (!obj.nested.empty())
  {
    EOObject::indent++;
    for(const auto& child : obj.nested)
    {
      os << child;
    }
    EOObject::indent--;
  }
  return os;
}


//Temporary test function
EOObject createSeq() {
  EOObject eo(EOObjectType::EO_ABSTRACT);
  eo.postfix = "global";
  eo.arguments = {"args..."};

  EOObject glob_ram("ram", "global_ram");
  glob_ram.AddNested({"2048",EOObjectType::EO_LITERAL});
  eo.AddNested(glob_ram);

  eo.AddNested({"memory","empty-global-position"});

  EOObject ret_ram("ram", "return-ram");
  ret_ram.AddNested({"1024",EOObjectType::EO_LITERAL});
  eo.AddNested(ret_ram);

  eo.AddNested({"memory","return-mem_size"});

  EOObject ret("address", "return");
  ret.AddNested(EOObject{"return-ram"});
  ret.AddNested(EOObject{"0",EOObjectType::EO_LITERAL});
  eo.AddNested(ret);

  EOObject x("address", "x");
  x.AddNested(EOObject{"global-ram"});
  x.AddNested(EOObject{"0",EOObjectType::EO_LITERAL});
  eo.AddNested(x);

  EOObject main {EOObjectType::EO_ABSTRACT};
  main.postfix = "main";
  EOObject seq1 {"seq","@"};
  EOObject write1{"write" ,EOObjectType::EO_COMPLETE};
  write1.AddNested(EOObject{"x"});
  EOObject add1 {"add"};
  EOObject rai64{"read-as-int64"};
  rai64.AddNested(EOObject{"x"});
  add1.AddNested(rai64);
  add1.AddNested({"1",EOObjectType::EO_LITERAL});
  write1.AddNested(add1);
  seq1.AddNested(write1);

  EOObject printf1 {"printf"};
  printf1.AddNested({"\"%d\\n\"",EOObjectType::EO_LITERAL});
  printf1.AddNested(rai64);
  seq1.AddNested(printf1);


  EOObject write2{"write" ,EOObjectType::EO_COMPLETE};
  write2.AddNested(EOObject{"x"});
  EOObject add2 {"add"};
  add2.AddNested({"1",EOObjectType::EO_LITERAL});
  add2.AddNested(rai64);
  write2.AddNested(add1);
  seq1.AddNested(write2);

  seq1.AddNested(printf1);

  seq1.AddNested({"TRUE",EOObjectType::EO_LITERAL});
  main.AddNested(seq1);
  eo.AddNested(main);

  EOObject eo_app{EOObjectType::EO_ABSTRACT};
  eo_app.postfix = "eo-application";
  eo_app.arguments = {"arg"};

  EOObject seq2 {"seq","@"};
  seq2.AddNested(EOObject{"main"});
  seq2.AddNested({"TRUE",EOObjectType::EO_LITERAL});
  eo_app.AddNested(seq2);
  eo.AddNested(eo_app);

  EOObject seq3 {"seq","@"};

  EOObject write3{"write"};
  write3.AddNested(EOObject{"x"});
  write3.AddNested(EOObject{"1",EOObjectType::EO_LITERAL});
  seq3.AddNested(write3);

  EOObject write4{"write"};
  write4.AddNested(EOObject{"empty-global-position"});
  write4.AddNested(EOObject{"8",EOObjectType::EO_LITERAL});
  seq3.AddNested(write4);

  EOObject eo_app_st{"eo-application"};
  eo_app_st.AddNested(EOObject{"args"});
  seq3.AddNested(eo_app_st);
  seq3.AddNested({"TRUE",EOObjectType::EO_LITERAL});
  eo.AddNested(seq3);
  /*

  [arg] > eo-application
    seq > @
      main
      TRUE

  seq > @
    write x 1
    write empty-global-position 8
    eo-application args
    TRUE
   */


  return eo;
}
