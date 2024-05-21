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
  emitCodeI("addi", sp, sp, -men_alloc.getStackSize());
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
      // assert(0);
      break;
  }
}
void CodeGenVisitor::visit(ReturnValueIR* return_value) {
  if (state == SCAN) return;

  int loc = men_alloc.getLoc(return_value->ret_value->toString());
  emitCodeI("lw", a0, sp, loc);
  emitCodeI("addi", sp, sp, men_alloc.getStackSize());

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
    men_alloc.alloc(binary_op_instr->toString(), 4);
    return;
  }
  int rs1 = loadFromMen(binary_op_instr->left);
  int rs2 = loadFromMen(binary_op_instr->right);
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
  int dmen = men_alloc.getLoc(binary_op_instr->toString());
  emitCodeI("sw", rd, sp, dmen);
  reg_alloc.freeAll();
}

void CodeGenVisitor::visit(AllocInstrIR* alloc_instr) {
  if (state == SCAN) {
    men_alloc.alloc(alloc_instr->toString(), alloc_instr->var->type->getSize());
    return;
  }
}
void CodeGenVisitor::visit(LoadInstrIR* load_instr) {
  if (state == SCAN) {
    men_alloc.alloc(load_instr->toString(), 4);
    return;
  }
  int src = loadFromMen(load_instr->src);
  int dmen = men_alloc.getLoc(load_instr->toString());
  emitCodeI("sw", src, sp, dmen);
  reg_alloc.freeAll();
}
void CodeGenVisitor::visit(StoreInstrIR* store_instr) {
  if (state == SCAN) return;

  int src = loadFromMen(store_instr->src);
  int dmen = men_alloc.getLoc(store_instr->dst->toString());
  emitCodeI("sw", src, sp, dmen);
  reg_alloc.freeAll();
}

int CodeGenVisitor::loadFromMen(ValueIR* value) {
  int reg = reg_alloc.GetOne();
  switch (value->tag) {
    case IRV_INTEGER:
      if (((IntegerValueIR*)value)->number == 0) {
        reg_alloc.free(reg);
        return x0;
      }
      emitCodeU("li", reg, ((IntegerValueIR*)value)->number);
      break;
    default:
      int loc = men_alloc.getLoc(value->toString());
      emitCodeI("lw", reg, sp, loc);
      break;
  }
  return reg;
}

void CodeGenVisitor::emitCodeR(std::string instr, int rd, int rs1, int rs2) {
  OutCode(instr, reg_alloc.GetName(rd), reg_alloc.GetName(rs1),
          reg_alloc.GetName(rs2));
}

void CodeGenVisitor::emitCodeI(std::string instr, int rd, int rs1, int imm) {
  if (instr == "sw" || instr == "lw") {
    OutCodeOffset(instr, reg_alloc.GetName(rd), reg_alloc.GetName(rs1), imm);
  } else {
    OutCode(instr, reg_alloc.GetName(rd), reg_alloc.GetName(rs1), imm);
  }
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

void CodeGenVisitor::OutCodeOffset(std::string instr, const std::string* rd,
                                   const std::string* rs1, int imm) {
  out_file << "  " << instr;
  for (int i = 6 - instr.size(); i; i--) {
    out_file << " ";
  }
  out_file << *rd << ", " << imm << "(" << *rs1 << ")" << std::endl;
}