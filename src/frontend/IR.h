#ifndef IR_H
#define IR_H

#include <fstream>
#include <iostream>
#include <memory>
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
  IRV_JUMP
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

class VariableIR : public ValueIR {
 public:
  std::string name;
  int id;
  Type type;
  VariableIR(Type t, std::string n)
      : ValueIR(ValueTag::IRV_VARIABLE), type(t), name(n) {
    id = id_alloc++;
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

class AllocInstrIR : public InstrIR {
 public:
  VariableIR* var;
  AllocInstrIR(VariableIR* v) : var(v) { this->tag = IRV_ALLOC; }
  void PrintName() const override {
    out_file << "@";
    var->PrintName();
  }
  void PrintIR() const override {
    out_file << "@";
    var->PrintName();
    out_file << " = alloc " << var->type.toString();
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
};

class BinaryOpInstrIR : public InstrIR {
 public:
  BinaryOpInstrIR() { this->tag = ValueTag::IRV_BOP; }
  std::string name;
  OpType op_type;
  ValueIR* left;
  ValueIR* right;
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
  ReturnValueIR() { this->tag = ValueTag::IRV_RETURN; }
  ValueIR* ret_value;
  void PrintIR() const override {
    out_file << "ret ";
    if (ret_value->tag == IRV_INTEGER) {
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
  BrInstrIR(ValueIR* c, Label* t, Label* f)
      : cond(c), true_label(t), false_label(f) {}
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
  JumpInstrIR(Label* l) : label(l) {}
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
  // std::unique_ptr<Type> ret_type;
  std::string name;
  // std::vector<std::unique_ptr<struct Type>> params;
  std::vector<std::unique_ptr<BasicBlockIR>> basic_blocks;

  void PrintIR() const override {
    if (name != "main") {
      std::cerr << "error: syntax error";
      exit(1);
    }
    out_file << "fun @" << name << "(): i32 {" << std::endl;
    for (auto& basic_block : basic_blocks) {
      basic_block->PrintIR();
    }
    out_file << "}" << std::endl;
  }
};

class ProgramIR : public BaseIR {
 public:
  std::vector<std::unique_ptr<FunctionIR>> functions;

  void PrintIR() const override {
    for (auto& function : functions) {
      function->PrintIR();
    }
  }
};

// class ConstIntIR :public BaseIR {}

#endif