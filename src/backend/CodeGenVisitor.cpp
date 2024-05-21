#include "CodeGenVisitor.h"

#include <string>

// TODO : fix memory leak

void CodeGenVisitor::visit(ProgramIR* program) {
  for (auto& function : program->functions) {
    out_file << "  .text" << std::endl;
    visit((FunctionIR*)function.get());
  }
}
void CodeGenVisitor::visit(FunctionIR* function) {
  out_file << "  .global " << function->name << std::endl;
  out_file << function->name << ":" << std::endl;
  men_alloc.enterFunction();
  state = SCAN;
  for (auto& bb : function->basic_blocks) {
    visit((BasicBlockIR*)bb.get());
  }
  emitCodeI("sub", sp, sp, men_alloc.getStackSize());
  state = GEN;
  for (auto& bb : function->basic_blocks) {
    visit((BasicBlockIR*)bb.get());
  }
  men_alloc.exitFunxtion();
}
void CodeGenVisitor::visit(BasicBlockIR* basic_block) {
  for (auto& value : basic_block->values) {
    visit((ValueIR*)value.get());
  }
}
void CodeGenVisitor::visit(ValueIR* value) {
  switch (value->tag) {
    case ValueTag::IRV_RETURN:
      visit((ReturnValueIR*)value);
      break;
    case ValueTag::IRV_INTEGER:
      visit((IntegerValueIR*)value);
      break;
    case ValueTag::IRV_BOP:
      visit((BinaryOpInstrIR*)value);
      break;
    case ValueTag::IRV_ALLOC:
      visit((AllocInstrIR*)value);
      break;
    case ValueTag::IRV_STORE:
      visit((StoreInstrIR*)value);
      break;
    case ValueTag::IRV_LOAD:
      visit((LoadInstrIR*)value);
      break;
    default:
      assert(0);
      break;
  }
}
void CodeGenVisitor::visit(ReturnValueIR* return_value) {
  if (return_value->ret_value->tag == IRV_INTEGER) {
    emitCodeU("li", a0, ((IntegerValueIR*)return_value->ret_value)->number);
  } else {
    int reg = ((ValueIR*)(return_value->ret_value))->reg;

    emitCodePIRR("mv", a0, reg);
  }
  emitCodePI("ret");
}
void CodeGenVisitor::visit(IntegerValueIR* integer_value) {
  int number = integer_value->number;
  if (number == 0) {
    integer_value->reg = 0;
    return;
  }
  int r = reg_alloc.GetOne();
  emitCodeU("li", r, number);
  integer_value->reg = r;
}
void CodeGenVisitor::visit(BinaryOpInstrIR* binary_op_instr) {
  if (state == SCAN) {
    men_alloc.alloc(binary_op_instr->name, 4);
    return;
  }
  if (binary_op_instr->left->tag == IRV_INTEGER) {
    visit((IntegerValueIR*)binary_op_instr->left);
  }
  if (binary_op_instr->right->tag == IRV_INTEGER) {
    visit((IntegerValueIR*)binary_op_instr->right);
  }
  int rs1 = binary_op_instr->left->reg;
  int rs2 = binary_op_instr->right->reg;
  int rd = rs1 != 0 ? rs1 : (rs2 != 0 ? rs2 : reg_alloc.GetOne());
  binary_op_instr->reg = rd;
  switch (binary_op_instr->op_type) {
    case OP_ADD:
      emitCodeR("add", rd, rs1, rs2);
      break;
    case OP_SUB:
      emitCodeR("sub", rd, rs1, rs2);
      break;
    case OP_MUL:
      emitCodeR("mul", rd, rs1, rs2);
      break;
    case OP_DIV:
      emitCodeR("div", rd, rs1, rs2);
      break;
    case OP_MOD:
      emitCodeR("rem", rd, rs1, rs2);
      break;
    case OP_EQU:
      emitCodeR("xor", rd, rs1, rs2);
      emitCodePIRR("seqz", rd, rd);
      break;
    case OP_NEQ:
      emitCodeR("xor", rd, rs1, rs2);
      emitCodePIRR("snez", rd, rd);
      break;
    case OP_LT:
      emitCodeR("slt", rd, rs1, rs2);
      break;
    case OP_GT:
      emitCodeR("sgt", rd, rs1, rs2);
      break;
    case OP_LE:
      emitCodeR("sgt", rd, rs1, rs2);
      emitCodePIRR("seqz", rd, rd);
      break;
    case OP_GE:
      emitCodeR("slt", rd, rs1, rs2);
      emitCodePIRR("seqz", rd, rd);
      break;
    case OP_OR:
      emitCodeR("or", rd, rs1, rs2);
      break;
    case OP_AND:
      emitCodeR("and", rd, rs1, rs2);
      break;
    default:
      break;
  }
}

void CodeGenVisitor::visit(AllocInstrIR* alloc_instr) {
  if (state == SCAN) {
    men_alloc.alloc(alloc_instr->var->name, alloc_instr->var->type->getSize());
  }
}
void CodeGenVisitor::visit(LoadInstrIR* load_instr) {
  if (state == SCAN) {
    men_alloc.alloc(load_instr->name, 4);
  }
}
void CodeGenVisitor::visit(StoreInstrIR* store_instr) {}

void CodeGenVisitor::emitCodeR(std::string instr, int rd, int rs1, int rs2) {
  OutCode(instr, reg_alloc.GetName(rd), reg_alloc.GetName(rs1),
          reg_alloc.GetName(rs2));
}

void CodeGenVisitor::emitCodeI(std::string instr, int rd, int rs1, int imm) {
  OutCode(instr, reg_alloc.GetName(rd), reg_alloc.GetName(rs1), imm);
}

void CodeGenVisitor::emitCodeS(std::string instr, int imm, int rs1, int rs2) {
  OutCode(instr, imm, reg_alloc.GetName(rs1), reg_alloc.GetName(rs2));
}

void CodeGenVisitor::emitCodeB(std::string instr, int imm, int rs1, int rs2) {
  OutCode(instr, imm, reg_alloc.GetName(rs1), reg_alloc.GetName(rs2));
}

void CodeGenVisitor::emitCodeU(std::string instr, int rd, int imm) {
  OutCode(instr, reg_alloc.GetName(rd), imm);
}
void CodeGenVisitor::emitCodeJ(std::string instr, int rd, int rs) {
  OutCode(instr, reg_alloc.GetName(rd), reg_alloc.GetName(rs));
}

void CodeGenVisitor::emitCodePI(std::string instr) { OutCode(instr); }

void CodeGenVisitor::emitCodePIRR(std::string instr, int rd, int rs) {
  OutCode(instr, reg_alloc.GetName(rd), reg_alloc.GetName(rs));
}

void CodeGenVisitor::OutCode(std::string instr, const std::string* rd,
                             const std::string* rs1, const std::string* rs2) {
  out_file << "  " << instr;
  for (int i = 6 - instr.size(); i; i--) {
    out_file << " ";
  }
  out_file << *rd << ", " << *rs1 << ", " << *rs2 << std::endl;
}
void CodeGenVisitor::OutCode(std::string instr, const std::string* rd,
                             const std::string* rs1, int rs2) {
  out_file << "  " << instr;
  for (int i = 6 - instr.size(); i; i--) {
    out_file << " ";
  }
  out_file << *rd << ", " << *rs1 << ", " << rs2 << std::endl;
}

void CodeGenVisitor::OutCode(std::string instr, const std::string* rd,
                             int imm) {
  out_file << "  " << instr;
  for (int i = 6 - instr.size(); i; i--) {
    out_file << " ";
  }
  out_file << *rd << ", " << imm << std::endl;
}

void CodeGenVisitor::OutCode(std::string instr, const std::string* rd,
                             const std::string* rs) {
  out_file << "  " << instr;
  for (int i = 6 - instr.size(); i; i--) {
    out_file << " ";
  }
  out_file << *rd << ", " << *rs << std::endl;
}

void CodeGenVisitor::OutCode(std::string instr) {
  out_file << "  " << instr << std::endl;
}

void CodeGenVisitor::OutCode(std::string instr, int imm, const std::string* rs1,
                             const std::string* rs2) {}