#include "CodeGenVisitor.h"




// TODO : fix memory leak

void CodeGenVisitor::Visit(ProgramIR* program) {
  for (auto& function : program->functions) {
    // out_file << "  .text" << std::endl;
    Visit((FunctionIR*)function.get());
  }
}
void CodeGenVisitor::Visit(FunctionIR* function) {
  out_file << "  .global " << function->name << std::endl;
  out_file << function->name << ":" << std::endl;
  for (auto& bb : function->basic_blocks) {
    Visit((BasicBlockIR*)bb.get());
  }
}
void CodeGenVisitor::Visit(BasicBlockIR* basic_block) {
  for (auto& value : basic_block->values) {
    Visit((ValueIR*)value.get());
  }
}
void CodeGenVisitor::Visit(ValueIR* value) {
  switch (value->tag) {
    case ValueTag::IRV_RETURN:
      Visit((ReturnValueIR*)value);
      break;
    case ValueTag::IRV_INTEGER:
      Visit((IntegerValueIR*)value);
      break;
    case ValueTag::IRV_BOP:
      Visit((BinaryOpInstrIR*)value);
      break;
    default:
      assert(0);
      break;
  }
}
void CodeGenVisitor::Visit(ReturnValueIR* return_value) {
  if (return_value->ret_value->tag == IRV_INTEGER) {
    OutCode(new std::string("li"), new std::string("a0"),
            ((IntegerValueIR*)return_value->ret_value)->number);
  } else {
    int reg = ((ValueIR*)(return_value->ret_value))->reg;
    // leak
    OutCode(new std::string("mv"), new std::string("a0"),
            reg_alloc.GetName(reg));
  }
  OutCode(new std::string("ret"));
}
void CodeGenVisitor::Visit(IntegerValueIR* integer_value) {
  int number = integer_value->number;
  if (number == 0) {
    integer_value->reg = 0;
    return;
  }
  int r = reg_alloc.GetOne();
  OutCode(new std::string("li"), reg_alloc.GetName(r), number);
  integer_value->reg = r;
}
void CodeGenVisitor::Visit(BinaryOpInstrIR* binary_op_instr) {
  if (binary_op_instr->left->tag == IRV_INTEGER) {
    Visit((IntegerValueIR*)binary_op_instr->left);
  }
  if (binary_op_instr->right->tag == IRV_INTEGER) {
    Visit((IntegerValueIR*)binary_op_instr->right);
  }
  int lr = binary_op_instr->left->reg;
  int rr = binary_op_instr->right->reg;
  int dr = lr != 0 ? lr : (rr != 0 ? rr : reg_alloc.GetOne());
  binary_op_instr->reg = dr;
  switch (binary_op_instr->op_type) {
    case OP_ADD:
      OutCode(new std::string("add"), reg_alloc.GetName(dr),
              reg_alloc.GetName(lr), reg_alloc.GetName(rr));
      break;
    case OP_SUB:
      OutCode(new std::string("sub"), reg_alloc.GetName(dr),
              reg_alloc.GetName(lr), reg_alloc.GetName(rr));
      break;

    case OP_MUL:
      OutCode(new std::string("mul"), reg_alloc.GetName(dr),
              reg_alloc.GetName(lr), reg_alloc.GetName(rr));
      break;
    case OP_DIV:
      OutCode(new std::string("div"), reg_alloc.GetName(dr),
              reg_alloc.GetName(lr), reg_alloc.GetName(rr));
      break;
    case OP_MOD:
      OutCode(new std::string("rem"), reg_alloc.GetName(dr),
              reg_alloc.GetName(lr), reg_alloc.GetName(rr));
      break;
    case OP_EQU:
      OutCode(new std::string("xor"), reg_alloc.GetName(dr),
              reg_alloc.GetName(lr), reg_alloc.GetName(rr));
      OutCode(new std::string("seqz"), reg_alloc.GetName(dr),
              reg_alloc.GetName(dr));
      break;
    case OP_NEQ:
      OutCode(new std::string("xor"), reg_alloc.GetName(dr),
              reg_alloc.GetName(lr), reg_alloc.GetName(rr));
      OutCode(new std::string("snez"), reg_alloc.GetName(dr),
              reg_alloc.GetName(dr));
      break;
    case OP_LT:
      OutCode(new std::string("slt"), reg_alloc.GetName(dr),
              reg_alloc.GetName(lr), reg_alloc.GetName(rr));
      break;
    case OP_GT:
      OutCode(new std::string("sgt"), reg_alloc.GetName(dr),
              reg_alloc.GetName(lr), reg_alloc.GetName(rr));
      break;
    case OP_LE:
      OutCode(new std::string("sgt"), reg_alloc.GetName(dr),
              reg_alloc.GetName(lr), reg_alloc.GetName(rr));
      OutCode(new std::string("seqz"), reg_alloc.GetName(dr),
              reg_alloc.GetName(dr));
      break;
    case OP_GE:
      OutCode(new std::string("slt"), reg_alloc.GetName(dr),
              reg_alloc.GetName(lr), reg_alloc.GetName(rr));
      OutCode(new std::string("seqz"), reg_alloc.GetName(dr),
              reg_alloc.GetName(dr));
      break;
    case OP_OR:
      OutCode(new std::string("or"), reg_alloc.GetName(dr),
              reg_alloc.GetName(lr), reg_alloc.GetName(rr));
      break;
    case OP_AND:
      OutCode(new std::string("and"), reg_alloc.GetName(dr),
              reg_alloc.GetName(lr), reg_alloc.GetName(rr));
      break;
    default:
      break;
  }
}

void CodeGenVisitor::OutCode(const std::string* instr, const std::string* rd,
                             const std::string* rs1, const std::string* rs2) {
  out_file << "  " << *instr;
  for (int i = 6 - instr->size(); i; i--) {
    out_file << " ";
  }
  out_file << *rd << ", " << *rs1 << ", " << *rs2 << std::endl;
}

void CodeGenVisitor::OutCode(const std::string* instr, const std::string* rd,
                             int imm) {
  out_file << "  " << *instr;
  for (int i = 6 - instr->size(); i; i--) {
    out_file << " ";
  }
  out_file << *rd << ", " << imm << std::endl;
}

void CodeGenVisitor::OutCode(const std::string* instr, const std::string* rd,
                             const std::string* rs) {
  out_file << "  " << *instr;
  for (int i = 6 - instr->size(); i; i--) {
    out_file << " ";
  }
  out_file << *rd << ", " << *rs << std::endl;
}

void CodeGenVisitor::OutCode(const std::string* instr) {
  out_file << "  " << *instr << std::endl;
}