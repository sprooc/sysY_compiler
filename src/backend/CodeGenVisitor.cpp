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
  if (st_size < 2048) {
    emitCodeI("addi", sp, sp, -st_size);
  } else {
    int tr = reg_alloc.GetOne();
    emitCodeU("li", tr, -st_size);
    emitCodeR("add", sp, sp, tr);
    reg_alloc.freeAll();
  }
  // save return address: ra
  emitSave(ra, sp, st_size - 4);

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
    case ValueTag::IRV_GEP:
      visit((GetElemPtrIR*)value);
      break;
    case ValueTag::IRV_GP:
      visit((GetPtrInstrIR*)value);
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
    emitLoad(ra, sp, men_alloc.getStackSize() - 4);
  }
  int st_size = men_alloc.getStackSize();
  if (st_size < 2048) {
    emitCodeI("addi", sp, sp, st_size);
  } else {
    int tr = reg_alloc.GetOne();
    emitCodeU("li", tr, st_size);
    emitCodeR("add", sp, sp, tr);
  }
  emitCodePI("ret");
  reg_alloc.freeAll();
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
  emitSave(rd, sp, dmen);
  reg_alloc.freeAll();
}

void CodeGenVisitor::visit(AllocInstrIR* alloc_instr) {
  if (state == SCAN) {
    // if (alloc_instr->var->type->tag == IRT_POINTER) {
    //   men_alloc.setDymPtr(alloc_instr->toString());
    // }
    men_alloc.alloc(alloc_instr->toString(), alloc_instr->var->type);
    return;
  }
}
void CodeGenVisitor::visit(LoadInstrIR* load_instr) {
  if (state == SCAN) {
    men_alloc.alloc(load_instr->toString(), 4);
    if (load_instr->src->tag == IRV_ALLOC &&
        ((AllocInstrIR*)load_instr->src)->var->type->tag == IRT_POINTER) {
      auto* var = ((AllocInstrIR*)load_instr->src)->var;
      men_alloc.setType(load_instr->toString(),
                        ((PointerType*)var->type)->elem_type.get());
      men_alloc.setDymPtr(load_instr->toString());
    }
    return;
  }
  int src_ptr = loadPtr(load_instr->src);
  int dmen = men_alloc.getLoc(load_instr->toString());
  int tr = reg_alloc.GetOne();
  emitLoad(tr, src_ptr, 0);
  emitSave(tr, sp, dmen);

  reg_alloc.freeAll();
  // int src = loadFromMen(load_instr->src);
  // int dmen = men_alloc.getLoc(load_instr->toString());

  // if (men_alloc.isDymPtr(load_instr->src->toString())) {
  //   int tr = reg_alloc.GetOne();
  //   emitLoad(tr, src, 0);
  //   emitSave(tr, sp, dmen);
  // } else {
  //   emitSave(src, sp, dmen);
  // }
  // reg_alloc.freeAll();
}

void CodeGenVisitor::visit(StoreInstrIR* store_instr) {
  if (state == SCAN) return;
  int src = loadFromMen(store_instr->src);

  int dst_ptr = loadPtr(store_instr->dst);
  emitSave(src, dst_ptr, 0);
  reg_alloc.freeAll();

  // if (store_instr->dst->tag == IRV_GALLOC) {
  //   int reg = reg_alloc.GetOne();
  //   emitCodeIRL("la", reg, ((GlobalAllocIR*)store_instr->dst)->var->name);
  //   emitCodeI("sw", src, reg, 0);
  //   reg_alloc.freeAll();
  // } else {
  //   int dmen = men_alloc.getLoc(store_instr->dst->toString());
  //   if (men_alloc.isDymPtr(store_instr->dst->toString())) {
  //     int tr = reg_alloc.GetOne();
  //     emitLoad(tr, sp, dmen);
  //     emitSave(src, tr, 0);
  //   } else {
  //     emitSave(src, sp, dmen);
  //   }
  //   reg_alloc.freeAll();
  // }
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
      // emitCodeI("sw", reg, sp, (l - 8) * 4);
      emitSave(reg, sp, (l - 8) * 4);
    }
    reg_alloc.freeAll();
    l++;
  }
  emitCodeIL("call", call_instr->function->name);
  if (call_instr->function->ret_type->tag != IRT_VOID) {
    // emitCodeI("sw", a0, sp, men_alloc.getLoc(call_instr->toString()));
    emitSave(a0, sp, men_alloc.getLoc(call_instr->toString()));
  }
  reg_alloc.freeAll();
}

void CodeGenVisitor::visit(GlobalAllocIR* galloc_instr) {
  out_file << "  .globl " << galloc_instr->toString() << std::endl;
  int size = galloc_instr->var->type->getSize();
  outLabel(galloc_instr->toString());
  if (galloc_instr->var->type->tag == IRT_ARRAY) {
    men_alloc.setType(galloc_instr->toString(), galloc_instr->var->type);
    AggregateValueIR* aggre = (AggregateValueIR*)galloc_instr->init_val;
    if (!aggre->arr_elems) {
      out_file << "  .zero " << galloc_instr->var->type->getSize() << std::endl;
    } else {
      for (auto* elem : *aggre->arr_elems) {
        out_file << "  .word ";
        elem->PrintName();
        out_file << std::endl;
      }
    }
  } else {
    out_file << "  .word ";
    galloc_instr->init_val->PrintName();
    out_file << std::endl;
  }
}
void CodeGenVisitor::visit(GetElemPtrIR* gep_instr) {
  if (state == SCAN) {
    men_alloc.alloc(gep_instr->toString(), 4);
    men_alloc.setDymPtr(gep_instr->toString());
    return;
  }

  int reg = loadPtr(gep_instr->ptr);

  ArrayType* array_type =
      (ArrayType*)men_alloc.getType(gep_instr->ptr->toString());
  int type_size = array_type->elem_type->getSize();
  int index_r = loadFromMen(gep_instr->index);
  int width_r = reg_alloc.GetOne();
  emitCodeU("li", width_r, type_size);
  emitCodeR("mul", width_r, width_r, index_r);
  emitCodeR("add", reg, reg, width_r);
  int dmen = men_alloc.getLoc(gep_instr->toString());
  // emitCodeI("sw", reg, sp, dmen);
  emitSave(reg, sp, dmen);
  men_alloc.setType(gep_instr->toString(), array_type->elem_type.get());
  reg_alloc.freeAll();
}

void CodeGenVisitor::visit(GetPtrInstrIR* gp_instr) {
  if (state == SCAN) {
    men_alloc.alloc(gp_instr->toString(), 4);
    men_alloc.setDymPtr(gp_instr->toString());
    men_alloc.setType(gp_instr->toString(),
                      men_alloc.getType(gp_instr->ptr->toString()));
    return;
  }

  int reg = loadPtr(gp_instr->ptr);
  Type* type = men_alloc.getType(gp_instr->ptr->toString());
  int type_size = type->getSize();
  int index_r = loadFromMen(gp_instr->index);
  int width_r = reg_alloc.GetOne();
  emitCodeU("li", width_r, type_size);
  emitCodeR("mul", width_r, width_r, index_r);
  emitCodeR("add", reg, reg, width_r);
  int dmen = men_alloc.getLoc(gp_instr->toString());
  emitSave(reg, sp, dmen);
  reg_alloc.freeAll();
}

void CodeGenVisitor::emitSave(int rs, int rd, int off) {
  if (off < 2048 && off >= -2048) {
    emitCodeI("sw", rs, rd, off);
  } else {
    int tr = reg_alloc.GetOne();
    emitCodeU("li", tr, off);
    emitCodeR("add", tr, rd, tr);
    emitCodeI("sw", rs, tr, 0);
    reg_alloc.free(tr);
  }
}
void CodeGenVisitor::emitLoad(int rd, int rs, int off) {
  if (off < 2048 && off >= -2048) {
    emitCodeI("lw", rd, rs, off);
  } else {
    int tr = reg_alloc.GetOne();
    emitCodeU("li", tr, off);
    emitCodeR("add", tr, rs, tr);
    emitCodeI("lw", rd, tr, 0);
    reg_alloc.free(tr);
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
      // emitCodeI("lw", reg, sp, men_alloc.getStackSize() + loc * 4);
      emitLoad(reg, sp, men_alloc.getStackSize() + loc * 4);
      return reg;
    case IRV_GALLOC:
      emitCodeIRL("la", reg, ((GlobalAllocIR*)value)->var->name);
      emitCodeI("lw", reg, reg, 0);
      return reg;
    default:
      loc = men_alloc.getLoc(value->toString());
      // emitCodeI("lw", reg, sp, loc);
      emitLoad(reg, sp, loc);
      return reg;
  }
}
int CodeGenVisitor::loadPtr(ValueIR* value) {
  int reg = reg_alloc.GetOne();
  if (value->tag == IRV_GALLOC) {
    emitCodeIRL("la", reg, ((GlobalAllocIR*)value)->toString());
    return reg;
  }
  int dmen = men_alloc.getLoc(value->toString());
  if (men_alloc.isDymPtr(value->toString())) {
    emitLoad(reg, sp, dmen);
  } else {
    if (dmen < 2048) {
      emitCodeI("addi", reg, sp, dmen);
    } else {
      int tr = reg_alloc.GetOne();
      emitCodeU("li", tr, dmen);
      emitCodeR("add", reg, sp, tr);
    }
  }
  return reg;
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