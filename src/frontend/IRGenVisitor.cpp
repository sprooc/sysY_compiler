#include "IRGenVisitor.h"

#include <algorithm>
#include <memory>
#include <ostream>
#include <string>

#include "IR.h"

std::any IRGenVisitor::visitProgram(SysYParser::ProgramContext* ctx) {
  return visitChildren(ctx);
}

std::any IRGenVisitor::visitCompUnit(SysYParser::CompUnitContext* ctx) {
  return visitChildren(ctx);
}

std::any IRGenVisitor::visitFuncDef(SysYParser::FuncDefContext* ctx) {
  FunctionIR* function_ir = new FunctionIR();
  function_ir->name = ctx->funcName()->IDENT()->getText();
  if (ctx->funcType()->VOID()) {
    function_ir->ret_type = std::make_unique<VoidType>();
  } else if (ctx->funcType()->INT()) {
    function_ir->ret_type = std::make_unique<Int32Type>();
  }
  Label* entry_label;
  if (function_ir->name != "main") {
    entry_label = new Label();
  } else {
    entry_label = new Label("entry");
  }
  BasicBlockIR* basic_block = new BasicBlockIR(entry_label);
  ir_module.pushFunction(function_ir);
  ir_module.pushBasicBlock(basic_block);
  // Funtion params scope
  ir_module.pushScope();
  if (ctx->funcFParams()) {
    for (auto* param : ctx->funcFParams()->funcFParam()) {
      std::string var_name = param->IDENT()->getText();
      ParamIR* param_ir = new ParamIR(var_name);
      Type* type = new Int32Type();
      std::vector<int>* arr_shape = new std::vector<int>();
      for (auto* exp : param->exp()) {
        int len =
            ((IntegerValueIR*)std::any_cast<ValueIR*>(visitExp(exp)))->number;
        arr_shape->push_back(len);
      }
      int size = arr_shape->size();
      for (int i = size - 1; i >= 0; i--) {
        type = new ArrayType(arr_shape->at(i), type);
      }
      if (!param->L_BRACKT().empty()) {
        type = new PointerType(type);
      }
      param_ir->type = std::unique_ptr<Type>(type);

      function_ir->params.push_back(std::unique_ptr<ParamIR>(param_ir));
      VariableIR* var_ir = new VariableIR(param_ir->type.get(), var_name);
      AllocInstrIR* alloc_ir = new AllocInstrIR(var_ir);
      ir_module.putVar(var_ir->name, alloc_ir);
      ir_module.pushValueToBasicBlock(alloc_ir);
      StoreInstrIR* store_ir = new StoreInstrIR(param_ir, alloc_ir);
      ir_module.pushValueToBasicBlock(store_ir);
    }
  }
  if (ctx->block()->blockItem().empty()) {
    ReturnValueIR* empty_ret = new ReturnValueIR();
    ir_module.endBasicBlock(empty_ret);
  } else {
    visitBlock(ctx->block());
  }
  ir_module.endFunction();
  ir_module.popScope();
  return nullptr;
}

std::any IRGenVisitor::visitBlock(SysYParser::BlockContext* ctx) {
  ir_module.pushScope();
  for (auto* block_item : ctx->blockItem()) {
    auto* stmt = block_item->stmt();
    if (stmt && (stmt->ifElseStmt() || stmt->whileStmt())) {
      Label* label = new Label();
      ir_module.pushNextLabel(label);
      visitStmt(stmt);
      ir_module.popNextLabel();
      ir_module.pushBasicBlock(new BasicBlockIR(label));
    } else {
      visitBlockItem(block_item);
    }
  }
  ir_module.popScope();
  return nullptr;
}

std::any IRGenVisitor::visitStmt(SysYParser::StmtContext* ctx) {
  if (ctx->ASSIGN()) {
    ValueIR* rval = std::any_cast<ValueIR*>(visitExp(ctx->exp()));
    ir_module.setLval(true);
    ValueIR* lval = std::any_cast<ValueIR*>(visitLVal(ctx->lVal()));
    ir_module.setLval(false);
    StoreInstrIR* store_ir = new StoreInstrIR(rval, lval);
    ir_module.pushValueToBasicBlock((ValueIR*)store_ir);
  } else if (ctx->RETURN()) {
    ReturnValueIR* ret_ir = new ReturnValueIR();
    if (ctx->exp()) {
      ret_ir->ret_value = std::any_cast<ValueIR*>(visitExp(ctx->exp()));
    } else {
      ret_ir->ret_value = nullptr;
    }
    ir_module.endBasicBlock((ValueIR*)ret_ir);
  } else if (ctx->block()) {
    visitBlock((ctx->block()));
  } else if (ctx->exp()) {
    visitExp(ctx->exp());
  } else if (ctx->ifElseStmt()) {
    return visitIfElseStmt(ctx->ifElseStmt());
  } else if (ctx->whileStmt()) {
    return visitWhileStmt(ctx->whileStmt());
  } else if (ctx->BREAK()) {
    ir_module.endBasicBlock(new JumpInstrIR(ir_module.peekBreakStack()));
  } else if (ctx->CONTINUE()) {
    ir_module.endBasicBlock(new JumpInstrIR(ir_module.peekContinueStack()));
  }
  return nullptr;
}

std::any IRGenVisitor::visitExp(SysYParser::ExpContext* ctx) {
  return visitLOrExp(ctx->lOrExp());
}

std::any IRGenVisitor::visitCond(SysYParser::CondContext* ctx) {
  return visitLOrExp(ctx->lOrExp());
}

std::any IRGenVisitor::visitPrimaryExp(SysYParser::PrimaryExpContext* ctx) {
  if (ctx->L_PAREN()) {
    return visitExp(ctx->exp());
  } else if (ctx->lVal()) {
    return visitLVal(ctx->lVal());
  } else {
    return visitNumber(ctx->number());
  }
}

std::any IRGenVisitor::visitLVal(SysYParser::LValContext* ctx) {
  std::string var = ctx->IDENT()->getText();
  ValueIR* value = ir_module.getVar(var);
  if (!value) {
    std::cerr << "Variable not define: " << var << std::endl;
    exit(1);
  }
  if (value->isConst()) {
    return (ValueIR*)value;
  }
  bool pre_lval = ir_module.getLval();
  bool is_pointer = ((AllocInstrIR*)value)->var->type->tag == IRT_POINTER;
  bool is_pr = false;
  if (is_pointer) {
    Type* array_type =
        ((PointerType*)((AllocInstrIR*)value)->var->type)->elem_type.get();
    if (array_type->tag == IRT_ARRAY) {
      if (((ArrayType*)array_type)->getDimen() + 1 != ctx->exp().size()) {
        is_pr = true;
      }
    }
  }
  if (((AllocInstrIR*)value)->var->type->tag == IRT_ARRAY) {
    if (((ArrayType*)((AllocInstrIR*)value)->var->type)->getDimen() !=
        ctx->exp().size()) {
      is_pr = true;
    }
  }
  ir_module.setLval(false);
  for (auto* exp : ctx->exp()) {
    ValueIR* offset = std::any_cast<ValueIR*>(visitExp(exp));
    if (is_pointer) {
      value = new LoadInstrIR(value, getTmp());
      ir_module.pushValueToBasicBlock(value);
      value = new GetPtrInstrIR(value, offset, getTmp());
      is_pointer = false;
    } else {
      value = new GetElemPtrIR(value, offset, getTmp());
    }
    ir_module.pushValueToBasicBlock(value);
  }
  ir_module.setLval(pre_lval);
  if (ir_module.getLval()) {
    return (ValueIR*)value;
  }
  if (is_pr) {
    value = new GetElemPtrIR(value, new IntegerValueIR(0), getTmp());
    ir_module.pushValueToBasicBlock(value);
    return (ValueIR*)value;
  }
  LoadInstrIR* load_ir = new LoadInstrIR(value, getTmp());
  ir_module.pushValueToBasicBlock((ValueIR*)load_ir);
  return (ValueIR*)load_ir;
}

std::any IRGenVisitor::visitUnaryExp(SysYParser::UnaryExpContext* ctx) {
  if (ctx->primaryExp()) {
    return visitPrimaryExp(ctx->primaryExp());
  } else if (ctx->funcName()) {
    std::string func_name = ctx->funcName()->IDENT()->getText();
    FunctionIR* func = ir_module.getFunction(func_name);
    CallInstrIR* call_ir = new CallInstrIR(func, getTmp());
    if (ctx->funcRParams()) {
      for (auto* exp : ctx->funcRParams()->exp()) {
        call_ir->params.push_back(std::any_cast<ValueIR*>(visitExp(exp)));
      }
    }
    ir_module.pushValueToBasicBlock(call_ir);
    return (ValueIR*)call_ir;
  } else {
    auto uop = ctx->unaryOp();
    if (uop->PLUS()) {
      return visitUnaryExp(ctx->unaryExp());
    }
    ValueIR* rvalue = std::any_cast<ValueIR*>(visitUnaryExp(ctx->unaryExp()));
    if (rvalue->tag == IRV_INTEGER) {
      IntegerValueIR* int_ir =
          new IntegerValueIR(((IntegerValueIR*)rvalue)->number);
      if (uop->MINUS()) {
        int_ir->number = -int_ir->number;
      } else {
        int_ir->number = !int_ir->number;
      }
      ir_module.pushValueToBasicBlock((ValueIR*)int_ir);
      return (ValueIR*)int_ir;
    }
    auto zero = new IntegerValueIR();
    zero->number = 0;
    ir_module.pushValueToBasicBlock((ValueIR*)zero);
    if (uop->MINUS()) {
      BinaryOpInstrIR* binstr = new BinaryOpInstrIR();
      binstr->op_type = OP_SUB;
      binstr->left = zero;
      binstr->right = rvalue;
      binstr->name = getTmp();
      ir_module.pushValueToBasicBlock((ValueIR*)binstr);
      return (ValueIR*)binstr;
    } else if (uop->NOT()) {
      BinaryOpInstrIR* equ_ir = new BinaryOpInstrIR();
      equ_ir->op_type = OP_EQU;
      equ_ir->left = zero;
      equ_ir->right = rvalue;
      equ_ir->name = getTmp();
      ir_module.pushValueToBasicBlock((ValueIR*)equ_ir);
      return (ValueIR*)equ_ir;
    } else {
      assert(0);
    }
  }
}

std::any IRGenVisitor::visitMulExp(SysYParser::MulExpContext* ctx) {
  if (ctx->mulExp()) {
    ValueIR* lvalue = std::any_cast<ValueIR*>(visitMulExp(ctx->mulExp()));
    ValueIR* rvalue = std::any_cast<ValueIR*>(visitUnaryExp(ctx->unaryExp()));
    if (lvalue->tag == IRV_INTEGER && rvalue->tag == IRV_INTEGER) {
      IntegerValueIR* int_ir = new IntegerValueIR();
      if (ctx->MUL()) {
        int_ir->number = ((IntegerValueIR*)lvalue)->number *
                         ((IntegerValueIR*)rvalue)->number;
      } else if (ctx->DIV()) {
        int_ir->number = ((IntegerValueIR*)lvalue)->number /
                         ((IntegerValueIR*)rvalue)->number;
      } else {
        int_ir->number = ((IntegerValueIR*)lvalue)->number %
                         ((IntegerValueIR*)rvalue)->number;
      }
      ir_module.pushValueToBasicBlock((ValueIR*)int_ir);
      return (ValueIR*)int_ir;
    }
    BinaryOpInstrIR* bin_ir = new BinaryOpInstrIR();
    if (ctx->MUL()) {
      bin_ir->op_type = OP_MUL;
    } else if (ctx->DIV()) {
      bin_ir->op_type = OP_DIV;
    } else if (ctx->MOD()) {
      bin_ir->op_type = OP_MOD;
    } else {
      assert(0);
    }
    bin_ir->left = lvalue;
    bin_ir->right = rvalue;
    bin_ir->name = getTmp();
    ir_module.pushValueToBasicBlock((ValueIR*)bin_ir);
    return (ValueIR*)bin_ir;
  } else {
    return visitUnaryExp(ctx->unaryExp());
  }
}

std::any IRGenVisitor::visitAddExp(SysYParser::AddExpContext* ctx) {
  ir_module.pushState(EXP);
  ValueIR* ret;
  if (ctx->addExp()) {
    ValueIR* lvalue = std::any_cast<ValueIR*>(visitAddExp(ctx->addExp()));
    ValueIR* rvalue = std::any_cast<ValueIR*>(visitMulExp(ctx->mulExp()));
    if (lvalue->tag == IRV_INTEGER && rvalue->tag == IRV_INTEGER) {
      IntegerValueIR* int_ir = new IntegerValueIR();
      if (ctx->PLUS()) {
        int_ir->number = ((IntegerValueIR*)lvalue)->number +
                         ((IntegerValueIR*)rvalue)->number;
      } else {
        int_ir->number = ((IntegerValueIR*)lvalue)->number -
                         ((IntegerValueIR*)rvalue)->number;
      }
      ir_module.pushValueToBasicBlock((ValueIR*)int_ir);
      ret = (ValueIR*)int_ir;
    } else {
      BinaryOpInstrIR* bin_ir = new BinaryOpInstrIR();
      if (ctx->PLUS()) {
        bin_ir->op_type = OP_ADD;
      } else if (ctx->MINUS()) {
        bin_ir->op_type = OP_SUB;
      } else {
        assert(0);
      }
      bin_ir->left = lvalue;
      bin_ir->right = rvalue;
      bin_ir->name = getTmp();
      ir_module.pushValueToBasicBlock((ValueIR*)bin_ir);
      ret = (ValueIR*)bin_ir;
    }
  } else {
    ret = std::any_cast<ValueIR*>(visitMulExp(ctx->mulExp()));
  }
  ir_module.popState();
  return ret;
}

std::any IRGenVisitor::visitRelExp(SysYParser::RelExpContext* ctx) {
  ValueIR* ret;
  if (ctx->relExp()) {
    ir_module.pushState(EXP);
    ValueIR* lvalue = std::any_cast<ValueIR*>(visitRelExp(ctx->relExp()));
    ValueIR* rvalue = std::any_cast<ValueIR*>(visitAddExp(ctx->addExp()));
    ir_module.popState();
    if (lvalue->tag == IRV_INTEGER && rvalue->tag == IRV_INTEGER) {
      IntegerValueIR* int_ir = new IntegerValueIR();
      if (ctx->LT()) {
        int_ir->number = ((IntegerValueIR*)lvalue)->number <
                         ((IntegerValueIR*)rvalue)->number;
      } else if (ctx->GT()) {
        int_ir->number = ((IntegerValueIR*)lvalue)->number >
                         ((IntegerValueIR*)rvalue)->number;
      } else if (ctx->GE()) {
        int_ir->number = ((IntegerValueIR*)lvalue)->number >=
                         ((IntegerValueIR*)rvalue)->number;
      } else if (ctx->LE()) {
        int_ir->number = ((IntegerValueIR*)lvalue)->number <=
                         ((IntegerValueIR*)rvalue)->number;
      }
      ir_module.pushValueToBasicBlock((ValueIR*)int_ir);
      ret = (ValueIR*)int_ir;
    } else {
      BinaryOpInstrIR* rel_ir = new BinaryOpInstrIR();
      if (ctx->LT()) {
        rel_ir->op_type = OP_LT;
      } else if (ctx->GT()) {
        rel_ir->op_type = OP_GT;
      } else if (ctx->GE()) {
        rel_ir->op_type = OP_GE;
      } else if (ctx->LE()) {
        rel_ir->op_type = OP_LE;
      } else {
        assert(0);
      }
      rel_ir->left = lvalue;
      rel_ir->right = rvalue;
      rel_ir->name = getTmp();
      ir_module.pushValueToBasicBlock((ValueIR*)rel_ir);
      ret = (ValueIR*)rel_ir;
    }
  } else {
    ret = std::any_cast<ValueIR*>(visitAddExp(ctx->addExp()));
  }
  if (ir_module.inCond()) {
    BrInstrIR* br_ir = new BrInstrIR(ret, ir_module.peekTrueLabel(),
                                     ir_module.peekFalseLabel());
    ir_module.endBasicBlock((ValueIR*)br_ir);
    return nullptr;
  } else {
    return ret;
  }
}

std::any IRGenVisitor::visitEqExp(SysYParser::EqExpContext* ctx) {
  if (ctx->eqExp()) {
    ir_module.pushState(EXP);
    ValueIR* lvalue = std::any_cast<ValueIR*>(visitEqExp(ctx->eqExp()));
    ValueIR* rvalue = std::any_cast<ValueIR*>(visitRelExp(ctx->relExp()));
    ir_module.popState();
    ValueIR* ret;
    if (lvalue->tag == IRV_INTEGER && rvalue->tag == IRV_INTEGER) {
      IntegerValueIR* int_ir = new IntegerValueIR();
      if (ctx->EQ()) {
        int_ir->number = ((IntegerValueIR*)lvalue)->number ==
                         ((IntegerValueIR*)rvalue)->number;
      } else {
        int_ir->number = ((IntegerValueIR*)lvalue)->number !=
                         ((IntegerValueIR*)rvalue)->number;
      }
      ir_module.pushValueToBasicBlock((ValueIR*)int_ir);
      ret = (ValueIR*)int_ir;
    } else {
      BinaryOpInstrIR* eq_ir = new BinaryOpInstrIR();
      if (ctx->EQ()) {
        eq_ir->op_type = OP_EQU;
      } else if (ctx->NEQ()) {
        eq_ir->op_type = OP_NEQ;
      } else {
        assert(0);
      }
      eq_ir->left = lvalue;
      eq_ir->right = rvalue;
      eq_ir->name = getTmp();
      ir_module.pushValueToBasicBlock((ValueIR*)eq_ir);
      ret = (ValueIR*)eq_ir;
    }
    if (ir_module.inCond()) {
      BrInstrIR* br_ir = new BrInstrIR(ret, ir_module.peekTrueLabel(),
                                       ir_module.peekFalseLabel());
      ir_module.endBasicBlock((ValueIR*)br_ir);
      return ret;
    } else {
      return ret;
    }
  } else {
    return visitRelExp(ctx->relExp());
  }
}

std::any IRGenVisitor::visitLAndExp(SysYParser::LAndExpContext* ctx) {
  if (ctx->lAndExp()) {
    if (ir_module.inCond()) {
      Label* b1_true = new Label();
      ir_module.pushTrueLabel(b1_true);
      visitLAndExp(ctx->lAndExp());
      ir_module.popTrueLabel();
      ir_module.pushBasicBlock(new BasicBlockIR(b1_true));
      visitEqExp(ctx->eqExp());
      return nullptr;
    }
    Label* true_label = new Label();
    Label* false_label = new Label();

    ValueIR* lvalue = std::any_cast<ValueIR*>(visitLAndExp(ctx->lAndExp()));
    if (lvalue->tag == IRV_INTEGER) {
      if (!((IntegerValueIR*)lvalue)->number) {
        return (ValueIR*)new IntegerValueIR(0);
      } else {
        ValueIR* rval = std::any_cast<ValueIR*>(visitEqExp(ctx->eqExp()));
        if (rval->tag == IRV_INTEGER) {
          return (ValueIR*)new IntegerValueIR(((IntegerValueIR*)rval)->number !=
                                              0);
        }
        BinaryOpInstrIR* ne_ir = new BinaryOpInstrIR();
        auto zero = new IntegerValueIR(0);
        ir_module.pushValueToBasicBlock((ValueIR*)zero);
        ne_ir->left = zero;
        ne_ir->right = rval;
        ne_ir->name = getTmp();
        ne_ir->op_type = OP_NEQ;
        ir_module.pushValueToBasicBlock((ValueIR*)ne_ir);
        return (ValueIR*)ne_ir;
      }
    }
    std::string tmp_name = "tmp";
    auto ret_var = new VariableIR(new Int32Type(), tmp_name);
    auto alloc_var = new AllocInstrIR(ret_var);
    StoreInstrIR* store_ir = new StoreInstrIR(lvalue, alloc_var);
    ir_module.pushValueToBasicBlock((ValueIR*)alloc_var);
    ir_module.pushValueToBasicBlock((ValueIR*)store_ir);
    BrInstrIR* br_ir = new BrInstrIR(lvalue, true_label, false_label);
    ir_module.endBasicBlock((ValueIR*)br_ir);
    ir_module.pushBasicBlock(new BasicBlockIR(true_label));
    ValueIR* rvalue = std::any_cast<ValueIR*>(visitEqExp(ctx->eqExp()));
    store_ir = new StoreInstrIR(rvalue, alloc_var);
    ir_module.pushValueToBasicBlock((ValueIR*)store_ir);
    ir_module.endBasicBlock(new JumpInstrIR(false_label));
    ir_module.pushBasicBlock(new BasicBlockIR(false_label));
    LoadInstrIR* load_ir = new LoadInstrIR(alloc_var, getTmp());
    ir_module.pushValueToBasicBlock((ValueIR*)load_ir);
    BinaryOpInstrIR* ne_ir = new BinaryOpInstrIR();
    auto zero = new IntegerValueIR(0);
    ir_module.pushValueToBasicBlock((ValueIR*)zero);
    ne_ir->left = zero;
    ne_ir->right = load_ir;
    ne_ir->name = getTmp();
    ne_ir->op_type = OP_NEQ;
    ir_module.pushValueToBasicBlock((ValueIR*)ne_ir);
    return (ValueIR*)ne_ir;
    // ValueIR* lvalue = std::any_cast<ValueIR*>(visitLAndExp(ctx->lAndExp()));
    // ValueIR* rvalue = std::any_cast<ValueIR*>(visitEqExp(ctx->eqExp()));
    // if (lvalue->tag == IRV_INTEGER && rvalue->tag == IRV_INTEGER) {
    //   IntegerValueIR* int_ir = new IntegerValueIR();
    //   int_ir->number = ((IntegerValueIR*)lvalue)->number &&
    //                    ((IntegerValueIR*)rvalue)->number;

    //   ir_module.pushValueToBasicBlock((ValueIR*)int_ir);
    //   return (ValueIR*)int_ir;
    // }

    // ir_module.pushValueToBasicBlock((ValueIR*)zero);
    // BinaryOpInstrIR* ne_ir1 = new BinaryOpInstrIR();
    // ne_ir1->left = zero;
    // ne_ir1->right = lvalue;
    // ne_ir1->name = getTmp();
    // ne_ir1->op_type = OP_NEQ;
    // ir_module.pushValueToBasicBlock((ValueIR*)ne_ir1);

    // BinaryOpInstrIR* ne_ir2 = new BinaryOpInstrIR();
    // ne_ir2->left = zero;
    // ne_ir2->right = rvalue;
    // ne_ir2->name = getTmp();
    // ne_ir2->op_type = OP_NEQ;
    // ir_module.pushValueToBasicBlock((ValueIR*)ne_ir2);

    // BinaryOpInstrIR* land_ir = new BinaryOpInstrIR();
    // land_ir->op_type = OP_AND;
    // land_ir->left = ne_ir1;
    // land_ir->right = ne_ir2;
    // land_ir->name = getTmp();
    // ir_module.pushValueToBasicBlock((ValueIR*)land_ir);
    // return (ValueIR*)land_ir;
  } else {
    return visitEqExp(ctx->eqExp());
  }
}

std::any IRGenVisitor::visitLOrExp(SysYParser::LOrExpContext* ctx) {
  if (ctx->lOrExp()) {
    if (ir_module.inCond()) {
      Label* b1_false = new Label();
      ir_module.pushFalseLabel(b1_false);
      visitLOrExp(ctx->lOrExp());
      ir_module.popFalseLabel();
      ir_module.pushBasicBlock(new BasicBlockIR(b1_false));
      visitLAndExp(ctx->lAndExp());
      return nullptr;
    }
    Label* true_label = new Label();
    Label* false_label = new Label();

    ValueIR* lvalue = std::any_cast<ValueIR*>(visitLOrExp(ctx->lOrExp()));
    if (lvalue->tag == IRV_INTEGER) {
      if (((IntegerValueIR*)lvalue)->number) {
        return (ValueIR*)new IntegerValueIR(1);
      } else {
        ValueIR* rval = std::any_cast<ValueIR*>(visitLAndExp(ctx->lAndExp()));
        if (rval->tag == IRV_INTEGER) {
          return (ValueIR*)new IntegerValueIR(((IntegerValueIR*)rval)->number !=
                                              0);
        }
        BinaryOpInstrIR* ne_ir = new BinaryOpInstrIR();
        auto zero = new IntegerValueIR(0);
        ir_module.pushValueToBasicBlock((ValueIR*)zero);
        ne_ir->left = zero;
        ne_ir->right = rval;
        ne_ir->name = getTmp();
        ne_ir->op_type = OP_NEQ;
        ir_module.pushValueToBasicBlock((ValueIR*)ne_ir);
        return (ValueIR*)ne_ir;
      }
    }
    std::string tmp_name = "tmp";
    auto ret_var = new VariableIR(new Int32Type(), tmp_name);
    auto alloc_var = new AllocInstrIR(ret_var);

    StoreInstrIR* store_ir = new StoreInstrIR(lvalue, alloc_var);
    ir_module.pushValueToBasicBlock((ValueIR*)alloc_var);
    ir_module.pushValueToBasicBlock((ValueIR*)store_ir);
    BrInstrIR* br_ir = new BrInstrIR(lvalue, true_label, false_label);
    ir_module.endBasicBlock((ValueIR*)br_ir);
    ir_module.pushBasicBlock(new BasicBlockIR(false_label));
    ValueIR* rvalue = std::any_cast<ValueIR*>(visitLAndExp(ctx->lAndExp()));
    store_ir = new StoreInstrIR(rvalue, alloc_var);
    ir_module.pushValueToBasicBlock((ValueIR*)store_ir);
    ir_module.endBasicBlock(new JumpInstrIR(true_label));
    ir_module.pushBasicBlock(new BasicBlockIR(true_label));
    LoadInstrIR* load_ir = new LoadInstrIR(alloc_var, getTmp());
    ir_module.pushValueToBasicBlock((ValueIR*)load_ir);
    BinaryOpInstrIR* ne_ir = new BinaryOpInstrIR();
    auto zero = new IntegerValueIR(0);
    ir_module.pushValueToBasicBlock((ValueIR*)zero);
    ne_ir->left = zero;
    ne_ir->right = load_ir;
    ne_ir->name = getTmp();
    ne_ir->op_type = OP_NEQ;
    ir_module.pushValueToBasicBlock((ValueIR*)ne_ir);
    return (ValueIR*)ne_ir;
    // if (lvalue->tag == IRV_INTEGER && rvalue->tag == IRV_INTEGER) {
    //   IntegerValueIR* int_ir = new IntegerValueIR();
    //   int_ir->number = ((IntegerValueIR*)lvalue)->number ||
    //                    ((IntegerValueIR*)rvalue)->number;

    //   ir_module.pushValueToBasicBlock((ValueIR*)int_ir);
    //   return (ValueIR*)int_ir;
    // }
    // BinaryOpInstrIR* lor_ir = new BinaryOpInstrIR();
    // lor_ir->op_type = OP_OR;
    // lor_ir->left = lvalue;
    // lor_ir->right = rvalue;
    // lor_ir->name = getTmp();
    // ir_module.pushValueToBasicBlock((ValueIR*)lor_ir);

    // BinaryOpInstrIR* ne_ir = new BinaryOpInstrIR();
    // auto zero = new IntegerValueIR(0);
    // ir_module.pushValueToBasicBlock((ValueIR*)zero);
    // ne_ir->left = zero;
    // ne_ir->right = lor_ir;
    // ne_ir->name = getTmp();
    // ne_ir->op_type = OP_NEQ;
    // ir_module.pushValueToBasicBlock((ValueIR*)ne_ir);
    // return (ValueIR*)ne_ir;
  } else {
    return visitLAndExp(ctx->lAndExp());
  }
}

std::any IRGenVisitor::visitNumber(SysYParser::NumberContext* ctx) {
  if (ctx->INTEGER_CONST()) {
    auto integer_ir = new IntegerValueIR();
    integer_ir->number = std::stoi(ctx->INTEGER_CONST()->getText(), nullptr, 0);
    ir_module.pushValueToBasicBlock((ValueIR*)integer_ir);
    return (ValueIR*)integer_ir;
  } else {
    return nullptr;
  }
}
std::any IRGenVisitor::visitConstDecl(SysYParser::ConstDeclContext* ctx) {
  return visitChildren(ctx);
}

std::any IRGenVisitor::visitConstDef(SysYParser::ConstDefContext* ctx) {
  if (!ctx->constExp().empty()) {
    if (ir_module.inGlobal()) {
      std::string var_name = ctx->IDENT()->getText();
      Type* type = new Int32Type();
      std::vector<int>* arr_shape = new std::vector<int>();
      for (auto* exp : ctx->constExp()) {
        int len = ((IntegerValueIR*)std::any_cast<ValueIR*>(visitConstExp(exp)))
                      ->number;
        arr_shape->push_back(len);
      }
      int size = arr_shape->size();
      for (int i = size - 1; i >= 0; i--) {
        type = new ArrayType(arr_shape->at(i), type);
      }
      VariableIR* variable = new VariableIR(type, var_name);
      ValueIR* value;

      // it is an array
      AggregateValueIR* aggre_ir = new AggregateValueIR(arr_shape);
      ir_module.pushAggregate(aggre_ir);
      visitConstInitVal(ctx->constInitVal());
      ir_module.popAggregate();
      value = aggre_ir;
      GlobalAllocIR* galloc = new GlobalAllocIR(variable, value);
      ir_module.pushGlobalVar(galloc);
      if (!ir_module.putVar(var_name, galloc)) {
        // Error hanlder
        std::cerr << "Redefine var: " << var_name << std::endl;
        exit(1);
      }
      return nullptr;
    }
    std::string var_name = ctx->IDENT()->getText();
    Type* type = new Int32Type();
    std::vector<int>* arr_shape = new std::vector<int>();
    for (auto* exp : ctx->constExp()) {
      int len = ((IntegerValueIR*)std::any_cast<ValueIR*>(visitConstExp(exp)))
                    ->number;
      type = new ArrayType(len, type);
      arr_shape->push_back(len);
    }
    VariableIR* variable = new VariableIR(type, var_name);
    ir_module.pushValueToBasicBlock((ValueIR*)variable);
    AllocInstrIR* alloc_ir = new AllocInstrIR(variable);
    ir_module.pushValueToBasicBlock((ValueIR*)alloc_ir);

    AggregateValueIR* aggre_ir = new AggregateValueIR(arr_shape);
    ir_module.pushAggregate(aggre_ir);
    visitConstInitVal(ctx->constInitVal());
    ir_module.popAggregate();
    int n = aggre_ir->arr_width->at(0);
    int dep = arr_shape->size();
    std::vector<int> locs(dep, 0);
    for (int i = 0; i < n; i++) {
      ValueIR* gep_ir = alloc_ir;
      for (int d = 0; d < dep; d++) {
        gep_ir =
            new GetElemPtrIR(gep_ir, new IntegerValueIR(locs[d]), getTmp());
        ir_module.pushValueToBasicBlock(gep_ir);
      }
      StoreInstrIR* store_ir =
          new StoreInstrIR(aggre_ir->arr_elems->at(i), gep_ir);
      ir_module.pushValueToBasicBlock(store_ir);
      int l = dep - 1;
      if (i == n - 1) {
        break;
      }
      while (true) {
        if (++locs[l] == arr_shape->at(l)) {
          locs[l] = 0;
        } else {
          break;
        }
        l--;
      }
    }
    if (!ir_module.putVar(var_name, alloc_ir)) {
      // Error hanlder
      std::cerr << "Redefine var: " << var_name << std::endl;
      exit(1);
    }
  } else {
    std::string var_name = ctx->IDENT()->getText();
    Type* type = new Int32Type();
    ValueIR* rvalue =
        std::any_cast<ValueIR*>(visitConstInitVal(ctx->constInitVal()));

    if (!ir_module.putVar(var_name, rvalue)) {
      // Error hanlder
      std::cerr << "Redefine var: " << var_name << std::endl;
      exit(1);
    }
  }
  return nullptr;
}

std::any IRGenVisitor::visitConstInitVal(SysYParser::ConstInitValContext* ctx) {
  if (ctx->constExp()) {
    return visitConstExp(ctx->constExp());
  } else {
    AggregateValueIR* aggregate = ir_module.peekAggregate();
    for (auto* elem : ctx->constInitVal()) {
      if (elem->constExp()) {
        aggregate->pushValue(
            std::any_cast<ValueIR*>(visitConstExp(elem->constExp())));
      } else {
        aggregate->inBrace();
        visitConstInitVal(elem);
      }
    }
    aggregate->outBrace();
    return nullptr;
  }
}

std::any IRGenVisitor::visitConstExp(SysYParser::ConstExpContext* ctx) {
  return visitExp(ctx->exp());
}

std::any IRGenVisitor::visitVarDecl(SysYParser::VarDeclContext* ctx) {
  return visitChildren(ctx);
}

std::any IRGenVisitor::visitVarDef(SysYParser::VarDefContext* ctx) {
  if (ir_module.inGlobal()) {
    std::string var_name = ctx->IDENT()->getText();
    Type* type = new Int32Type();
    std::vector<int>* arr_shape = new std::vector<int>();
    for (auto* exp : ctx->constExp()) {
      int len = ((IntegerValueIR*)std::any_cast<ValueIR*>(visitConstExp(exp)))
                    ->number;
      arr_shape->push_back(len);
    }
    int size = arr_shape->size();
    for (int i = size - 1; i >= 0; i--) {
      type = new ArrayType(arr_shape->at(i), type);
    }
    VariableIR* variable = new VariableIR(type, var_name);
    ValueIR* value;
    if (ctx->initVal()) {
      if (arr_shape->empty()) {
        // single value
        value = std::any_cast<ValueIR*>(visitInitVal(ctx->initVal()));
      } else {
        // it is an array
        AggregateValueIR* aggre_ir = new AggregateValueIR(arr_shape);
        ir_module.pushAggregate(aggre_ir);
        visitInitVal(ctx->initVal());
        ir_module.popAggregate();
        value = aggre_ir;
      }
    } else {
      if (arr_shape->empty()) {
        // single value
        value = new IntegerValueIR(0);
      } else {
        value = new AggregateValueIR(arr_shape);
      }
    }
    GlobalAllocIR* galloc_ir = new GlobalAllocIR(variable, value);
    ir_module.pushGlobalVar(galloc_ir);

    if (!ir_module.putVar(var_name, galloc_ir)) {
      // Error hanlder
      std::cerr << "Redefine var: " << var_name << std::endl;
      exit(1);
    }
    return nullptr;
  }

  std::string var_name = ctx->IDENT()->getText();
  Type* type = new Int32Type();
  std::vector<int>* arr_shape = new std::vector<int>();
  for (auto* exp : ctx->constExp()) {
    int len =
        ((IntegerValueIR*)std::any_cast<ValueIR*>(visitConstExp(exp)))->number;
    arr_shape->push_back(len);
  }
  int size = arr_shape->size();
  for (int i = size - 1; i >= 0; i--) {
    type = new ArrayType(arr_shape->at(i), type);
  }
  VariableIR* variable = new VariableIR(type, var_name);
  ir_module.pushValueToBasicBlock((ValueIR*)variable);
  AllocInstrIR* alloc_ir = new AllocInstrIR(variable);
  ir_module.pushValueToBasicBlock((ValueIR*)alloc_ir);
  if (ctx->initVal()) {
    if (arr_shape->empty()) {
      // single value
      ValueIR* value = std::any_cast<ValueIR*>(visitInitVal(ctx->initVal()));
      StoreInstrIR* store_ir = new StoreInstrIR(value, (ValueIR*)alloc_ir);
      ir_module.pushValueToBasicBlock((ValueIR*)store_ir);
    } else {
      // it is an array
      AggregateValueIR* aggre_ir = new AggregateValueIR(arr_shape);
      ir_module.pushAggregate(aggre_ir);
      visitInitVal(ctx->initVal());
      ir_module.popAggregate();
      // TODO store
      int n = aggre_ir->arr_width->at(0);
      int dep = arr_shape->size();
      std::vector<int> locs(dep, 0);
      for (int i = 0; i < n; i++) {
        ValueIR* gep_ir = alloc_ir;
        for (int d = 0; d < dep; d++) {
          gep_ir =
              new GetElemPtrIR(gep_ir, new IntegerValueIR(locs[d]), getTmp());
          ir_module.pushValueToBasicBlock(gep_ir);
        }
        StoreInstrIR* store_ir =
            new StoreInstrIR(aggre_ir->arr_elems->at(i), gep_ir);
        ir_module.pushValueToBasicBlock(store_ir);
        int l = dep - 1;
        if (i == n - 1) {
          break;
        }
        while (true) {
          if (++locs[l] == arr_shape->at(l)) {
            locs[l] = 0;
          } else {
            break;
          }
          l--;
        }
      }
      delete aggre_ir;
    }
  }
  if (!ir_module.putVar(var_name, alloc_ir)) {
    // Error hanlder
    std::cerr << "Redefine var: " << var_name << std::endl;
    exit(1);
  }
  return nullptr;
}

std::any IRGenVisitor::visitInitVal(SysYParser::InitValContext* ctx) {
  if (ctx->exp()) {
    return visitExp(ctx->exp());
  } else {
    AggregateValueIR* aggregate = ir_module.peekAggregate();

    for (auto* elem : ctx->initVal()) {
      if (elem->exp()) {
        aggregate->pushValue(std::any_cast<ValueIR*>(visitExp(elem->exp())));
      } else {
        aggregate->inBrace();
        visitInitVal(elem);
      }
    }
    aggregate->outBrace();
    return aggregate;
  }
}

std::any IRGenVisitor::visitIfElseStmt(SysYParser::IfElseStmtContext* ctx) {
  if (ctx->ELSE()) {
    Label* true_label = new Label();
    Label* false_label = new Label();
    Label* end_label = ir_module.peekNextLabel();
    ir_module.pushTrueLabel(true_label);
    ir_module.pushFalseLabel(false_label);
    ir_module.pushState(COND);
    visitCond(ctx->cond());
    ir_module.popState();
    ir_module.popFalseLabel();
    ir_module.popTrueLabel();
    BasicBlockIR* then_bb = new BasicBlockIR(true_label);
    ir_module.pushBasicBlock(then_bb);
    ir_module.pushNextLabel(end_label);
    visitStmt(ctx->stmt(0));
    JumpInstrIR* jump_ir = new JumpInstrIR(end_label);
    ir_module.endBasicBlock(jump_ir);
    BasicBlockIR* else_bb = new BasicBlockIR(false_label);
    ir_module.pushBasicBlock(else_bb);
    visitStmt(ctx->stmt(1));
    ir_module.popNextLabel();
    ir_module.endBasicBlock(new JumpInstrIR(end_label));
  } else {
    Label* true_label = new Label();
    Label* false_label = ir_module.peekNextLabel();
    ir_module.pushTrueLabel(true_label);
    ir_module.pushFalseLabel(false_label);
    ir_module.pushState(COND);
    visitCond(ctx->cond());
    ir_module.popState();
    ir_module.popFalseLabel();
    ir_module.popTrueLabel();
    BasicBlockIR* then_bb = new BasicBlockIR(true_label);
    ir_module.pushBasicBlock(then_bb);
    visitStmt(ctx->stmt(0));
    JumpInstrIR* jump_ir = new JumpInstrIR(false_label);
    ir_module.endBasicBlock(jump_ir);
  }
  return nullptr;
}

std::any IRGenVisitor::visitWhileStmt(SysYParser::WhileStmtContext* ctx) {
  Label* begin = new Label();
  Label* true_label = new Label();
  Label* false_label = ir_module.peekNextLabel();
  ir_module.pushTrueLabel(true_label);
  ir_module.pushFalseLabel(false_label);
  ir_module.endBasicBlock(new JumpInstrIR(begin));
  ir_module.pushBasicBlock(new BasicBlockIR(begin));
  ir_module.pushState(COND);
  visitCond(ctx->cond());
  ir_module.popState();
  ir_module.popTrueLabel();
  ir_module.popFalseLabel();
  ir_module.pushBasicBlock(new BasicBlockIR(true_label));
  ir_module.pushBreakStack(ir_module.peekNextLabel());
  ir_module.pushContinueStack(begin);
  ir_module.pushNextLabel(begin);
  visitStmt(ctx->stmt());
  ir_module.popNextLabel();
  ir_module.popContinueStack();
  ir_module.popBreakStack();
  ir_module.endBasicBlock(new JumpInstrIR(begin));
  return nullptr;
}