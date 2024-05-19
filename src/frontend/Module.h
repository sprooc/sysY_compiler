#ifndef MODULE_H
#define MODULE_H

#include <memory>
#include <stack>

#include "IR.h"
#include "Scope.h"
enum ExpState { EXP, COND };
class Module {
 private:
  Scope* curr_scope;
  BasicBlockIR* curr_basic_block;
  FunctionIR* curr_function;
  std::unique_ptr<ProgramIR> program_ir;
  std::unique_ptr<std::stack<Label*>> true_stack;
  std::unique_ptr<std::stack<Label*>> false_stack;
  std::unique_ptr<std::stack<Label*>> next_stack;
  std::unique_ptr<std::stack<ExpState>> state_stack;
  std::unique_ptr<std::stack<Label*>> continue_stack;
  std::unique_ptr<std::stack<Label*>> break_stack;

 public:
  Module() : curr_basic_block(nullptr), program_ir(new ProgramIR()) {
    curr_scope = new Scope(nullptr);
    true_stack = std::unique_ptr<std::stack<Label*>>(new std::stack<Label*>());
    false_stack = std::unique_ptr<std::stack<Label*>>(new std::stack<Label*>());
    next_stack = std::unique_ptr<std::stack<Label*>>(new std::stack<Label*>());
    state_stack = std::unique_ptr<std::stack<ExpState>>(new std::stack<ExpState>());
    continue_stack = std::unique_ptr<std::stack<Label*>>(new std::stack<Label*>());
    break_stack = std::unique_ptr<std::stack<Label*>>(new std::stack<Label*>());

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

  void pushValueToBasicBlock(ValueIR* value) {
    assert(curr_basic_block);
    if (curr_basic_block->is_end) {
      pushBasicBlock(new BasicBlockIR(new Label()));
    }
    curr_basic_block->values.push_back(std::unique_ptr<ValueIR>(value));
  }

  void endBasicBlock(ValueIR* value) {
    if (curr_basic_block->is_end) {
      return;
    }
    pushValueToBasicBlock(value);
    curr_basic_block->is_end = true;
  }

  void pushFunction(FunctionIR* function) {
    curr_function = function;
    program_ir->functions.push_back(std::unique_ptr<FunctionIR>(function));
  }

  void pushBasicBlock(BasicBlockIR* basic_block) {
    curr_basic_block = basic_block;
    curr_function->basic_blocks.push_back(
        std::unique_ptr<BasicBlockIR>(basic_block));
  }

  void printProgram() { program_ir->PrintIR(); }

  void pushTrueLabel(Label* label) { true_stack->push(label); }

  void pushFalseLabel(Label* label) { false_stack->push(label); }

  void pushNextLabel(Label* label) { next_stack->push(label); }

  void popTrueLabel() { true_stack->pop(); }

  void popFalseLabel() { false_stack->pop(); }

  void popNextLabel() { next_stack->pop(); }

  Label* peekTrueLabel() { return true_stack->top(); }

  Label* peekFalseLabel() { return false_stack->top(); }

  Label* peekNextLabel() {
    return next_stack->empty() ? nullptr : next_stack->top();
  }

  void pushState(ExpState st) { state_stack->push(st); }

  void popState() { state_stack->pop(); }

  bool inCond() { return !state_stack->empty() && state_stack->top() == COND; }

  void pushContinueStack(Label* label) { continue_stack->push(label); }

  void popContinueStack() { continue_stack->pop(); }

  Label* peekContinueStack() { return continue_stack->top(); }

  void pushBreakStack(Label* label) { break_stack->push(label); }

  void popBreakStack() { break_stack->pop(); }

  Label* peekBreakStack() { return break_stack->top(); }
};

#endif