#include "CodeGenVisitor.h"

#include <string>

// TODO : fix memory leak

void CodeGenVisitor::visit(ProgramIR* program) {
  out_file << "  .data" << std::endl;
  for (auto& galloc : program->global_vars) {
    visit((GlobalAllocIR*)galloc.get());
  }

  out_file << "  .text" << std::endl;
  for (auto& function : program->functions) {
    visit((FunctionIR*)function.get());
  }
}
void CodeGenVisitor::visit(FunctionIR* function) {
  out_file << "  .globl " << function->name << std::endl;
  out_file << function->name << ":" << std::endl;
  men_alloc.enterFunction();
  state = SCAN;
  for (auto& bb : function->basic_blocks) {
    visit((BasicBlockIR*)bb.get());
  }
  // alloc stack space
  int st_size = men_alloc.getStackSize();
  emitCodeI("addi", sp, sp, -st_size);
  // save return address: ra
  if (men_alloc.hasCall()) {
    emitCodeI("sw", ra, sp, st_size - 4);
  }
  state = GEN;
  for (auto& bb : function->basic_blocks) {
    visit((BasicBlockIR*)bb.get());
  }
  men_alloc.exitFunxtion();
  out_file << std::endl;
}
void CodeGenVisitor::visit(BasicBlockIR* basic_block) {
  if (state == GEN) {
    outLabel(basic_block->label->name);
  }
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
    case ValueTag::IRV_JUMP:
      visit((JumpInstrIR*)value);
      break;
    case ValueTag::IRV_BR:
      visit((BrInstrIR*)value);
      break;
    case ValueTag::IRV_CALL:
      visit((CallInstrIR*)value);
      break;
    default:
      // assert(0);
      break;
  }
}
void CodeGenVisitor::visit(ReturnValueIR* return_value) {
  if (state == SCAN) return;
  if (return_value->ret_value) {
    int reg = loadFromMen(return_value->ret_value, a0);
  }
  if (men_alloc.hasCall()) {
    emitCodeI("lw", ra, sp, men_alloc.getStackSize() - 4);
  }
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
  if (store_instr->dst->tag == IRV_GALLOC) {
    int reg = reg_alloc.GetOne();
    emitCodeIRL("la", reg, ((GlobalAllocIR*)store_instr->dst)->var->name);
    emitCodeI("sw", src, 0, reg);
    reg_alloc.freeAll();
  } else {
    int dmen = men_alloc.getLoc(store_instr->dst->toString());
    emitCodeI("sw", src, sp, dmen);
    reg_alloc.freeAll();
  }
}

void CodeGenVisitor::visit(JumpInstrIR* jump_instr) {
  if (state == SCAN) return;
  emitCodeIL("j", jump_instr->label->name);
  reg_alloc.freeAll();
}

void CodeGenVisitor::visit(BrInstrIR* br_instr) {
  if (state == SCAN) return;
  int reg = loadFromMen(br_instr->cond);
  emitCodeIRL("bnez", reg, br_instr->true_label->name);
  emitCodeIL("j", br_instr->false_label->name);
  reg_alloc.freeAll();
}

void CodeGenVisitor::visit(CallInstrIR* call_instr) {
  if (state == SCAN) {
    men_alloc.noticeCall(call_instr->params.size());
    if (call_instr->function->ret_type->tag != IRT_VOID) {
      men_alloc.alloc(call_instr->toString(), 4);
    }
    return;
  }
  int l = 0;
  for (auto* param : call_instr->params) {
    if (l < 8) {
      loadFromMen(param, a0 + l);
    } else {
      int reg = loadFromMen(param);
      emitCodeI("sw", reg, sp, (l - 8) * 4);
    }
    reg_alloc.freeAll();
    l++;
  }
  emitCodeIL("call", call_instr->function->name);
  if (call_instr->function->ret_type->tag != IRT_VOID) {
    emitCodeI("sw", a0, sp, men_alloc.getLoc(call_instr->toString()));
  }
  reg_alloc.freeAll();
}
void CodeGenVisitor::visit(GlobalAllocIR* galloc_instr) {
  out_file << "  .globl " << galloc_instr->var->name << std::endl;
  int size = galloc_instr->var->type->getSize();
  outLabel(galloc_instr->var->name);
  if (!galloc_instr->init_val) {
    out_file << "  .zero " << size << std::endl;
  } else {
    out_file << "  .word ";
    galloc_instr->init_val->PrintName();
    out_file << std::endl;
  }
}

int CodeGenVisitor::loadFromMen(ValueIR* value, int reg) {
  bool has_reg = true;
  if (reg == 0) {
    has_reg = false;
    reg = reg_alloc.GetOne();
  }
  int loc;
  switch (value->tag) {
    case IRV_INTEGER:
      if (((IntegerValueIR*)value)->number == 0 && !has_reg) {
        return x0;
      }
      emitCodeU("li", reg, ((IntegerValueIR*)value)->number);
      return reg;
    case IRV_PARAM:
      loc = ((ParamIR*)value)->loc;
      if (loc < 8) {
        return a0 + loc;
      }
      loc -= 8;
      emitCodeI("lw", reg, sp, men_alloc.getStackSize() + loc * 4);
      return reg;
    case IRV_GALLOC:
      emitCodeIRL("la", reg, ((GlobalAllocIR*)value)->var->name);
      emitCodeI("lw", reg, reg, 0);
      return reg;
    default:
      loc = men_alloc.getLoc(value->toString());
      emitCodeI("lw", reg, sp, loc);
      return reg;
  }
}

void CodeGenVisitor::emitCodeR(std::string instr, int rd, int rs1, int rs2) {
  outCode(instr, reg_alloc.GetName(rd), reg_alloc.GetName(rs1),
          reg_alloc.GetName(rs2));
}

void CodeGenVisitor::emitCodeI(std::string instr, int rd, int rs1, int imm) {
  if (instr == "sw" || instr == "lw") {
    outCodeOffset(instr, reg_alloc.GetName(rd), reg_alloc.GetName(rs1), imm);
  } else {
    outCode(instr, reg_alloc.GetName(rd), reg_alloc.GetName(rs1), imm);
  }
}

void CodeGenVisitor::emitCodeS(std::string instr, int imm, int rs1, int rs2) {
  outCode(instr, imm, reg_alloc.GetName(rs1), reg_alloc.GetName(rs2));
}

void CodeGenVisitor::emitCodeB(std::string instr, int imm, int rs1, int rs2) {
  outCode(instr, imm, reg_alloc.GetName(rs1), reg_alloc.GetName(rs2));
}

void CodeGenVisitor::emitCodeU(std::string instr, int rd, int imm) {
  outCode(instr, reg_alloc.GetName(rd), imm);
}
void CodeGenVisitor::emitCodeJ(std::string instr, int rd, int rs) {
  outCode(instr, reg_alloc.GetName(rd), reg_alloc.GetName(rs));
}

void CodeGenVisitor::emitCodePI(std::string instr) { outCode(instr); }

void CodeGenVisitor::emitCodePIRR(std::string instr, int rd, int rs) {
  outCode(instr, reg_alloc.GetName(rd), reg_alloc.GetName(rs));
}

void CodeGenVisitor::emitCodeIRL(std::string instr, int r, std::string label) {
  outCode(instr, reg_alloc.GetName(r), label);
}
void CodeGenVisitor::emitCodeIL(std::string instr, std::string label) {
  outCode(instr, label);
}

void CodeGenVisitor::outCode(std::string instr, const std::string* rd,
                             const std::string* rs1, const std::string* rs2) {
  out_file << "  " << instr;
  for (int i = 6 - instr.size(); i; i--) {
    out_file << " ";
  }
  out_file << *rd << ", " << *rs1 << ", " << *rs2 << std::endl;
}
void CodeGenVisitor::outCode(std::string instr, const std::string* rd,
                             const std::string* rs1, int rs2) {
  out_file << "  " << instr;
  for (int i = 6 - instr.size(); i; i--) {
    out_file << " ";
  }
  out_file << *rd << ", " << *rs1 << ", " << rs2 << std::endl;
}

void CodeGenVisitor::outCode(std::string instr, const std::string* rd,
                             int imm) {
  out_file << "  " << instr;
  for (int i = 6 - instr.size(); i; i--) {
    out_file << " ";
  }
  out_file << *rd << ", " << imm << std::endl;
}

void CodeGenVisitor::outCode(std::string instr, const std::string* rd,
                             const std::string* rs) {
  out_file << "  " << instr;
  for (int i = 6 - instr.size(); i; i--) {
    out_file << " ";
  }
  out_file << *rd << ", " << *rs << std::endl;
}

void CodeGenVisitor::outCode(std::string instr) {
  out_file << "  " << instr << std::endl;
}

void CodeGenVisitor::outCode(std::string instr, int imm, const std::string* rs1,
                             const std::string* rs2) {}

void CodeGenVisitor::outCodeOffset(std::string instr, const std::string* rd,
                                   const std::string* rs1, int imm) {
  out_file << "  " << instr;
  for (int i = 6 - instr.size(); i; i--) {
    out_file << " ";
  }
  out_file << *rd << ", " << imm << "(" << *rs1 << ")" << std::endl;
}
void CodeGenVisitor::outCode(std::string instr, const std::string* r,
                             std::string label) {
  out_file << "  " << instr;
  for (int i = 6 - instr.size(); i; i--) {
    out_file << " ";
  }
  out_file << *r << ", " << label << std::endl;
}
void CodeGenVisitor::outCode(std::string instr, std::string label) {
  out_file << "  " << instr;
  for (int i = 6 - instr.size(); i; i--) {
    out_file << " ";
  }
  out_file << label << std::endl;
}
void CodeGenVisitor::outLabel(std::string label) {
  out_file << label << ":" << std::endl;
}