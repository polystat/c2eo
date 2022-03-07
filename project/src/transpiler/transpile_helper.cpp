#include "transpile_helper.h"
#include "unit_transpiler.h"
using namespace clang;
using namespace llvm;
using namespace std;

EOObject GetBinaryStmtEOObject(const BinaryOperator *p_operator);

EOObject GetFunctionBody(const clang::FunctionDecl *FD) {
  const CompoundStmt* funcBody = dyn_cast<CompoundStmt>(FD->getBody());
  if(!funcBody)
    return EOObject(EOObjectType::EO_EMPTY);
  return GetCompoundStmt(funcBody);
}
//Function to get eo representation of CompoundStmt
EOObject GetCompoundStmt(const clang::CompoundStmt *CS) {
  EOObject res {"seq","@"};
  for (auto stmt : CS->body() ) {
    EOObject stmt_obj = GetStmtEOObject(stmt);
    res.nested.push_back(stmt_obj);
  }
  res.nested.emplace_back("TRUE",EOObjectType::EO_LITERAL);
  return res;
}

EOObject GetStmtEOObject(const Stmt* stmt) {
  Stmt::StmtClass stmtClass = stmt->getStmtClass();
  if (stmtClass == Stmt::BinaryOperatorClass) {
    const auto* op = dyn_cast<BinaryOperator>(stmt);
    return GetBinaryStmtEOObject(op);
  }
  else if (stmtClass == Stmt::ParenExprClass) {
    const auto* op = dyn_cast<ParenExpr>(stmt);
    return GetStmtEOObject(*op->child_begin());
  }
  else if (stmtClass == Stmt::ImplicitCastExprClass) {
    const auto* op = dyn_cast<ImplicitCastExpr>(stmt);
    //TODO if cast kinds and also split it to another func
    return GetStmtEOObject(*op->child_begin());
  }
  else if (stmtClass == Stmt::DeclRefExprClass) {
    const DeclRefExpr* op = (DeclRefExpr*)stmt;
    extern UnitTranspiler transpiler;
    //TODO fix unsafety code
    return EOObject{transpiler.glob.GetVarByID(reinterpret_cast<uint64_t>(op->getFoundDecl())).alias};
  }
  return EOObject(EOObjectType::EO_EMPTY);
}

EOObject GetBinaryStmtEOObject(const BinaryOperator *p_operator) {
  string typenm = "-"+GetTypeName(p_operator->getType());
  std::string opName = p_operator->getOpcodeStr().str();
  std::string operation;
  if (opName == "=") {
    operation = "write";
  } else if (opName == "+") {
    operation = "add" + typenm;
  } else if (opName == "-") {
    operation = "sub" + typenm;
  } else {
    operation = "undefined";
  }

  EOObject binop {operation};
  binop.nested.push_back(GetStmtEOObject(p_operator->getLHS()));
  binop.nested.push_back(GetStmtEOObject(p_operator->getRHS()));
  return binop;
}

std::string GetTypeName(QualType qualType)
{
  extern ASTContext* context;
  const clang::Type* typePtr = qualType.getTypePtr();
  TypeInfo typeInfo = context->getTypeInfo(typePtr);
  uint64_t typeSize = typeInfo.Width;
  std::string str;

  if (typePtr->isBooleanType()) {
    str += "bool";
    return str;
  }
  if (typePtr->isFloatingType()) {
    str += "float" + std::to_string(typeSize);
    return str;
  }

  if (!typePtr->isSignedIntegerType())
    str += "u";
  if (typePtr->isCharType()) {
    str += "char";
    return str;
  }
  if (typePtr->isIntegerType()) {
    str += "int" + std::to_string(typeSize);
    return str;
  }


  if (typePtr->isUnionType())
    str = "un_";
  if (typePtr->isStructureType())
    str = "st_";
  if (typePtr->isUnionType() || typePtr->isStructureType()) {
    RecordDecl* RD = typePtr->getAsRecordDecl();
    if (RD->hasNameForLinkage())
      str += RD->getNameAsString();
    else
      str += std::to_string(reinterpret_cast<uint64_t>(RD));
    return str;
  }
  return "undefinedtype";
}


