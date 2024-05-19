#ifndef MODULE_H
#define MODULE_H

#include "IR.h"
#include "Scope.h"
#include <memory>

class Module {
 private:
  Scope* curr_scope;
  BasicBlockIR* curr_basic_block;
  std::unique_ptr<ProgramIR> program_ir;
  

 public:
  Module() : curr_basic_block(nullptr), program_ir(new ProgramIR()) {
    curr_scope = new Scope(nullptr);
  }
  ~Module() { delete curr_scope; }

  ValueIR* getVar(const std::string& var) const {
    return curr_scope->find(var);
  }

  bool putVar(const std::string& var, ValueIR* value) {
    if (curr_scope->contain(var)) {
      return false;
    }
    curr_scope->put(var, value);
    return true;
  }

  void pushScope() { curr_scope = new Scope(curr_scope); }

  void popScope() {
    assert(curr_scope);
    Scope* pre_scope = curr_scope;
    curr_scope = curr_scope->parent_scope;
    delete pre_scope;
  }

  void setCurrBasicBlock(BasicBlockIR* bb) { curr_basic_block = bb; }

  void pushValueToBasicBlock(ValueIR* value) {
    assert(curr_basic_block);
    curr_basic_block->values.push_back(std::unique_ptr<ValueIR>(value));
  }

  void pushFunction(FunctionIR* function) {
    program_ir->functions.push_back(std::unique_ptr<FunctionIR>(function));
  }

  void printProgram() {
    program_ir->PrintIR();
  }

};

#endif