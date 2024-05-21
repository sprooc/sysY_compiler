#ifndef IR_H
#define IR_H

#include <fstream>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "Label.h"
#include "Type.h"

extern std::ofstream out_file;

static int id_alloc = 0;
class BaseIR {
 public:
  virtual ~BaseIR() = default;
  virtual void PrintIR() const {};
};

typedef enum {
  IRV_RETURN,
  IRV_INTEGER,
  IRV_FLOAT,
  IRV_CONSTANT,
  IRV_INSTR,
  IRV_BOP,
  IRV_VARIABLE,
  IRV_ALLOC,
  IRV_LOAD,
  IRV_STORE,
  IRV_BR,
  IRV_JUMP,
  IRV_PARAM,
  IRV_CALL,
  IRV_DECL,
  IRV_GALLOC,
  IRV_GEP,
  IRV_AGGREGATE,
  IRV_GP
} ValueTag;

typedef enum {
  OP_ADD,
  OP_SUB,
  OP_EQU,
  OP_MUL,
  OP_DIV,
  OP_MOD,
  OP_LE,
  OP_GE,
  OP_LT,
  OP_GT,
  OP_AND,
  OP_OR,
  OP_NEQ,
} OpType;

class ValueIR : public BaseIR {
 public:
  ValueIR(ValueTag t) : tag(t){};
  ValueTag tag;
  int reg;
  void PrintIR() const override {};
  virtual void PrintName() const {};
  virtual std::string toString() const { return "empty"; }
  bool isConst() { return tag == IRV_INTEGER || tag == IRV_FLOAT; }
  bool isInstr() { return !isConst() && tag != IRV_VARIABLE; }
};

class ConstantIR : public ValueIR {
 public:
  ConstantIR() : ValueIR(ValueTag::IRV_CONSTANT){};
  void PrintIR() const override {};
  void PrintName() const override {};
};

class InstrIR : public ValueIR {
 public:
  InstrIR() : ValueIR(ValueTag::IRV_INSTR){};
  void PrintIR() const override {};
  void PrintName() const override {};
};

class ParamIR : public ValueIR {
 public:
  std::string name;
  int loc;
  std::unique_ptr<Type> type;
  ParamIR() : ValueIR(IRV_PARAM) {}
  ParamIR(std::string& n, int l) : ValueIR(IRV_PARAM), name(n), loc(l) {}
  ParamIR(Type* t, std::string& n, int l)
      : ValueIR(IRV_PARAM), name(n), type(std::unique_ptr<Type>(t)),loc(l) {}
  void PrintName() const override { out_file << "@" << name; };
  void PrintIR() const override {
    out_file << "@" << name << ": " << type->toString();
  };
  std::string toString() const override { return name; }
};

class VariableIR : public ValueIR {
 public:
  std::string name;
  int id;
  Type* type;
  VariableIR() : ValueIR(ValueTag::IRV_VARIABLE) { id = id_alloc++; }
  VariableIR(std::string& n) : ValueIR(ValueTag::IRV_VARIABLE), name(n) {
    id = id_alloc++;
  }
  VariableIR(Type* t, std::string n)
      : ValueIR(ValueTag::IRV_VARIABLE), type(t), name(n) {
    id = id_alloc++;
  }
  std::string toString() const override {
    return name + "_" + std::to_string(id);
  }
  void PrintIR() const override {};
  void PrintName() const override { out_file << name << "_" << id; };
};

class IntegerValueIR : public ConstantIR {
 public:
  IntegerValueIR() { this->tag = IRV_INTEGER; }
  IntegerValueIR(int n) : number(n) { this->tag = IRV_INTEGER; }
  IntegerValueIR(const IntegerValueIR& other) : number(other.number) {}
  int number;
  void PrintIR() const override { out_file << number; }
  void PrintName() const override { out_file << number; };
};

class AggregateValueIR : public ValueIR {
 public:
  std::unique_ptr<std::vector<int>> arr_shape;
  std::unique_ptr<std::vector<ValueIR*>> arr_elems;
  std::unique_ptr<std::vector<int>> arr_width;
  std::unique_ptr<std::stack<int>> depth_stack;
  int depth = 0;
  int ploc = 0;
  int loc = 0;
  AggregateValueIR() : ValueIR(IRV_AGGREGATE) {}
  AggregateValueIR(std::vector<int>* shape, std::vector<ValueIR*>* elems)
      : ValueIR(IRV_AGGREGATE) {
    arr_shape = std::unique_ptr<std::vector<int>>(shape);
    arr_elems = std::unique_ptr<std::vector<ValueIR*>>(elems);
  }
  AggregateValueIR(std::vector<int>* shape) : ValueIR(IRV_AGGREGATE) {
    arr_shape = std::unique_ptr<std::vector<int>>(shape);
    int n = 1;
    int size = arr_shape->size();
    arr_width = std::make_unique<std::vector<int>>(size);
    arr_width->at(size - 1) = 4;
    for (int i = size - 1; i >= 0; i--) {
      n *= arr_shape->at(i);
      if (i == size - 1) {
        arr_width->at(i) = arr_shape->at(i);
      } else {
        arr_width->at(i) = arr_shape->at(i) * arr_width->at(i + 1);
      }
    }
    auto zero = new IntegerValueIR(0);
    arr_elems = std::make_unique<std::vector<ValueIR*>>(n, zero);
    depth_stack = std::make_unique<std::stack<int>>();
    depth = loc = 0;
  }
  void pushValue(ValueIR* v) { arr_elems->at(loc++) = v; }
  void inBrace() {
    int size = arr_shape->size();
    int nd = depth + 1;
    for (int i = size - 1; i >= depth; i--) {
      if (loc % arr_width->at(i) != 0) {
        nd = i + 1;
        break;
      }
    }
    depth_stack->push(depth);
    depth = nd;
    ploc = loc;
  }
  void outBrace() {
    auto zero = new IntegerValueIR(0);
    int size = arr_width->at(0);
    while (loc < size && (loc % arr_width->at(depth) != 0 || ploc == loc)) {
      arr_elems->at(loc++) = zero;
    }
    if (!depth_stack->empty()) {
      depth = depth_stack->top();
      depth_stack->pop();
    }
  }
  void PrintIR() const override {
    if(!arr_elems) {
      out_file << "zeroinit";
    }
    dfsPrint(0, arr_shape->size() - 1, 0, arr_elems->size());
  }
  void PrintName() const override { PrintIR(); }

  void dfsPrint(int level, int maxD, int l, int r) const {
    out_file << "{";
    if (level == maxD) {
      int w = arr_shape->at(level);
      for (int i = 0; i < w; i++) {
        arr_elems->at(l + i)->PrintName();
        if (i != w - 1) {
          out_file << ", ";
        }
      }
    } else {
      int seg = arr_shape->at(level);
      int w = (r - l) / seg;
      for (int i = 0; i < seg; i++) {
        dfsPrint(level + 1, maxD, l + w * i, l + w * (i + 1));
        if (i != seg - 1) {
          out_file << ", ";
        }
      }
    }

    out_file << "}";
  }
};

class GetElemPtrIR : public InstrIR {
 public:
  std::string name;
  ValueIR* ptr;
  ValueIR* index;
  GetElemPtrIR() { this->tag = IRV_GEP; }
  GetElemPtrIR(ValueIR* p, ValueIR* id, std::string n)
      : ptr(p), index(id), name(n) {
    this->tag = IRV_GEP;
  }
  std::string toString() const override { return name; }
  void PrintName() const override { out_file << name; }
  void PrintIR() const override {
    out_file << name << " = getelemptr ";
    ptr->PrintName();
    out_file << ", ";
    index->PrintName();
  }
};

class GetPtrInstrIR : public InstrIR {
 public:
  std::string name;
  ValueIR* ptr;
  ValueIR* index;
  GetPtrInstrIR() { this->tag = IRV_GP; }
  GetPtrInstrIR(ValueIR* p, ValueIR* id, std::string n)
      : ptr(p), index(id), name(n) {
    this->tag = IRV_GEP;
  }
  std::string toString() const override { return name; }
  void PrintName() const override { out_file << name; }
  void PrintIR() const override {
    out_file << name << " = getptr ";
    ptr->PrintName();
    out_file << ", ";
    index->PrintName();
  }
};

class AllocInstrIR : public InstrIR {
 public:
  VariableIR* var;
  AllocInstrIR(VariableIR* v) : var(v) { this->tag = IRV_ALLOC; }
  std::string toString() const override { return var->toString(); }
  void PrintName() const override {
    out_file << "@";
    var->PrintName();
  }
  void PrintIR() const override {
    out_file << "@";
    var->PrintName();
    out_file << " = alloc " << var->type->toString();
  }
};

class GlobalAllocIR : public ValueIR {
 public:
  VariableIR* var;
  ValueIR* init_val;
  GlobalAllocIR(VariableIR* v, ValueIR* val)
      : var(v), init_val(val), ValueIR(IRV_GALLOC) {}
  void PrintName() const override {
    out_file << "@";
    var->PrintName();
  }
  void PrintIR() const override {
    out_file << "global @";
    var->PrintName();
    out_file << " = alloc " << var->type->toString() << ", ";
    init_val->PrintName();
  }
};

class StoreInstrIR : public InstrIR {
 public:
  StoreInstrIR(ValueIR* s, ValueIR* d) : src(s), dst(d) {
    this->tag = IRV_STORE;
  }
  ValueIR* src;
  ValueIR* dst;
  void PrintName() const override {}
  void PrintIR() const override {
    out_file << "store ";
    src->PrintName();
    out_file << ", ";
    dst->PrintName();
  }
};

class LoadInstrIR : public InstrIR {
 public:
  LoadInstrIR(ValueIR* s, std::string n) : src(s), name(n) {
    this->tag = IRV_LOAD;
  }
  std::string name;
  ValueIR* src;
  void PrintName() const override { out_file << name; }
  void PrintIR() const override {
    out_file << name << " = load ";
    src->PrintName();
  }
  std::string toString() const override { return name; }
};

class BinaryOpInstrIR : public InstrIR {
 public:
  BinaryOpInstrIR() { this->tag = ValueTag::IRV_BOP; }
  BinaryOpInstrIR(OpType op, ValueIR* l, ValueIR* r, std::string n)
      : op_type(op), left(l), right(r), name(n) {}
  std::string name;
  OpType op_type;
  ValueIR* left;
  ValueIR* right;
  std::string toString() const override { return name; }
  void PrintName() const override { out_file << name; }
  void PrintIR() const override {
    switch (op_type) {
      case OpType::OP_ADD:
        out_file << name << " = add ";
        break;
      case OpType::OP_SUB:
        out_file << name << " = sub ";
        break;
      case OpType::OP_EQU:
        out_file << name << " = eq ";
        break;
      case OpType::OP_MUL:
        out_file << name << " = mul ";
        break;
      case OpType::OP_DIV:
        out_file << name << " = div ";
        break;
      case OpType::OP_MOD:
        out_file << name << " = mod ";
        break;
      case OpType::OP_LT:
        out_file << name << " = lt ";
        break;
      case OpType::OP_GT:
        out_file << name << " = gt ";
        break;
      case OpType::OP_LE:
        out_file << name << " = le ";
        break;
      case OpType::OP_GE:
        out_file << name << " = ge ";
        break;
      case OpType::OP_NEQ:
        out_file << name << " = ne ";
        break;
      case OpType::OP_OR:
        out_file << name << " = or ";
        break;
      case OpType::OP_AND:
        out_file << name << " = and ";
        break;
      default:
        break;
    }
    left->PrintName();
    out_file << ", ";
    right->PrintName();
  };
};

class ReturnValueIR : public InstrIR {
 public:
  ReturnValueIR() {
    this->tag = ValueTag::IRV_RETURN;
    ret_value = nullptr;
  }
  ValueIR* ret_value;
  void PrintIR() const override {
    out_file << "ret ";
    if (!ret_value) {
    } else if (ret_value->tag == IRV_INTEGER) {
      ret_value->PrintIR();
    } else {
      out_file << ((BinaryOpInstrIR*)ret_value)->name;
    }
  }
};

class BrInstrIR : public InstrIR {
 public:
  ValueIR* cond;
  Label* true_label;
  Label* false_label;
  BrInstrIR() { tag = IRV_BR; }
  BrInstrIR(ValueIR* c, Label* t, Label* f)
      : cond(c), true_label(t), false_label(f) {
    this->tag = IRV_BR;
  }
  void PrintIR() const override {
    out_file << "br ";
    cond->PrintName();
    out_file << ", " << true_label->toString() << ", "
             << false_label->toString();
  }
};

class JumpInstrIR : public InstrIR {
 public:
  Label* label;
  JumpInstrIR(Label* l) : label(l) { this->tag = IRV_JUMP; }
  void PrintIR() const override { out_file << "jump " << label->toString(); }
};

class BasicBlockIR : public BaseIR {
 public:
  std::unique_ptr<Label> label;
  std::vector<std::unique_ptr<ValueIR>> values;
  bool is_end = false;
  BasicBlockIR(Label* label) : label(std::unique_ptr<Label>(label)) {
    label->basic_block = this;
  }
  void PrintIR() const override {
    // if (values.empty()) {
    //   return;
    // }
    out_file << label->toString() << ":" << std::endl;
    for (auto& value : values) {
      if (!value->isInstr()) {
        continue;
      }
      out_file << "  ";
      value->PrintIR();
      out_file << std::endl;
    }
  }
};

class FunctionIR : public BaseIR {
 public:
  // FunctionIR& operator=(FunctionIR&&) = default;
  // ~FunctionIR() override = default;
  FunctionIR() {}
  FunctionIR(Type* rt, std::string n) {
    ret_type = std::unique_ptr<Type>(rt);
    name = n;
  }
  std::unique_ptr<Type> ret_type;
  std::string name;
  std::vector<std::unique_ptr<ParamIR>> params;
  std::vector<std::unique_ptr<BasicBlockIR>> basic_blocks;
  void PrintName() const { out_file << "@" << name; }

  void PrintIR() const override {
    out_file << "fun @" << name << "(";
    int n_param = params.size();
    for (int i = 0; i < n_param; i++) {
      params[i]->PrintIR();
      if (i != n_param - 1) {
        out_file << ", ";
      }
    }
    out_file << ")";
    if (ret_type->tag == IRT_INT32) {
      out_file << ": i32";
    }
    out_file << " {" << std::endl;
    for (auto& basic_block : basic_blocks) {
      basic_block->PrintIR();
    }
    out_file << "}" << std::endl;
  }
};

class DeclInstrIR : public InstrIR {
 public:
  FunctionIR* function;
  DeclInstrIR() { this->tag = IRV_DECL; }
  DeclInstrIR(FunctionIR* f) : function(f) { this->tag = IRV_DECL; }
  void PrintName() const override {}
  void PrintIR() const override {
    out_file << "decl ";
    function->PrintName();
    out_file << "(";
    int n_param = function->params.size();
    for (int i = 0; i < n_param; i++) {
      out_file << function->params[i]->type->toString();
      if (i != n_param - 1) {
        out_file << ", ";
      }
    }
    out_file << ")";
    if (function->ret_type->tag != IRT_VOID) {
      out_file << ": " << function->ret_type->toString();
    }
    out_file << std::endl;
  }
};
class CallInstrIR : public InstrIR {
 public:
  std::string name;
  FunctionIR* function;
  std::vector<ValueIR*> params;
  CallInstrIR() { this->tag = IRV_CALL; }
  CallInstrIR(FunctionIR* f, std::string n) : function(f), name(n) {
    this->tag = IRV_CALL;
  }
  std::string toString() const override { return name; }
  void PrintName() const override { out_file << name; }
  void PrintIR() const override {
    if (function->ret_type->tag != IRT_VOID) {
      out_file << name << " = ";
    }
    out_file << "call ";
    function->PrintName();
    out_file << "(";
    int n_param = params.size();
    for (int i = 0; i < n_param; i++) {
      params[i]->PrintName();
      if (i != n_param - 1) {
        out_file << ", ";
      }
    }
    out_file << ")";
  }
};

class ProgramIR : public BaseIR {
 public:
  std::vector<std::unique_ptr<FunctionIR>> functions;
  std::vector<std::unique_ptr<DeclInstrIR>> decls;
  std::vector<std::unique_ptr<GlobalAllocIR>> global_vars;
  void PrintIR() const override {
    for (auto& decl : decls) {
      decl->PrintIR();
    }
    for (auto& gvar : global_vars) {
      gvar->PrintIR();
      out_file << std::endl;
    }
    for (auto& function : functions) {
      function->PrintIR();
    }
  }
};

// class ConstIntIR :public BaseIR {}

#endif