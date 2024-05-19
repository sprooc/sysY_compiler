#include <assert.h>

#include <iostream>

#include "IR.h"
#include "Module.h"
#include "SysYParserBaseVisitor.h"
class IRGenVisitor : public SysYParserBaseVisitor {
 private:

  int tmp = 0;
  std::string getTmp() { return "%" + std::to_string(tmp++); }
  Module ir_module;

 public:
  void PrintResult() {
    ir_module.printProgram();
  }
  virtual std::any visitProgram(SysYParser::ProgramContext* ctx) override;

  virtual std::any visitCompUnit(SysYParser::CompUnitContext* ctx) override;

  virtual std::any visitFuncDef(SysYParser::FuncDefContext* ctx) override;

  virtual std::any visitBlock(SysYParser::BlockContext* ctx) override;

  virtual std::any visitStmt(SysYParser::StmtContext* ctx) override;

  virtual std::any visitExp(SysYParser::ExpContext* ctx) override;

  virtual std::any visitCond(SysYParser::CondContext* ctx) override;

  virtual std::any visitPrimaryExp(SysYParser::PrimaryExpContext* ctx) override;

  virtual std::any visitLVal(SysYParser::LValContext* ctx) override;

  virtual std::any visitUnaryExp(SysYParser::UnaryExpContext* ctx) override;

  virtual std::any visitMulExp(SysYParser::MulExpContext* ctx) override;

  virtual std::any visitAddExp(SysYParser::AddExpContext* ctx) override;

  virtual std::any visitRelExp(SysYParser::RelExpContext* ctx) override;

  virtual std::any visitEqExp(SysYParser::EqExpContext* ctx) override;

  virtual std::any visitLAndExp(SysYParser::LAndExpContext* ctx) override;

  virtual std::any visitLOrExp(SysYParser::LOrExpContext* ctx) override;

  virtual std::any visitNumber(SysYParser::NumberContext* ctx) override;

  virtual std::any visitConstDecl(SysYParser::ConstDeclContext* ctx) override;

  virtual std::any visitConstDef(SysYParser::ConstDefContext* ctx) override;

  virtual std::any visitConstInitVal(SysYParser::ConstInitValContext* ctx) override;

  virtual std::any visitConstExp(SysYParser::ConstExpContext* ctx) override;

  virtual std::any visitVarDecl(SysYParser::VarDeclContext* ctx) override;

  virtual std::any visitVarDef(SysYParser::VarDefContext* ctx) override;

  virtual std::any visitInitVal(SysYParser::InitValContext* ctx) override;
};
