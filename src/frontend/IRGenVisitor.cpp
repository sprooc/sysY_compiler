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
  BasicBlockIR* basic_block = new BasicBlockIR();
  basic_block->name = "entry";
  function_ir->basic_blocks.push_back(
      std::unique_ptr<BasicBlockIR>(basic_block));
  ir_module.setCurrBasicBlock(basic_block);
  ir_module.pushFunction(function_ir);
  visitBlock(ctx->block());
  return nullptr;
}

std::any IRGenVisitor::visitBlock(SysYParser::BlockContext* ctx) {
  ir_module.pushScope();
  visitChildren(ctx);
  ir_module.popScope();
  return nullptr;
}

std::any IRGenVisitor::visitStmt(SysYParser::StmtContext* ctx) {
  if (ctx->ASSIGN()) {
    ValueIR* rval = std::any_cast<ValueIR*>(visitExp(ctx->exp()));
    std::string var = ctx->lVal()->IDENT()->getText();
    ValueIR* lval = ir_module.getVar(var);
    StoreInstrIR* store_ir = new StoreInstrIR(rval, lval);
    ir_module.pushValueToBasicBlock((ValueIR*)store_ir);

  } else if (ctx->RETURN()) {
    ReturnValueIR* ret_ir = new ReturnValueIR();
    ret_ir->ret_value = std::any_cast<ValueIR*>(visitExp(ctx->exp()));
    ir_module.pushValueToBasicBlock((ValueIR*)ret_ir);
  } else if (ctx->block()) {
    visitBlock((ctx->block()));
  } else if(ctx->exp()) {
    visitExp(ctx->exp());
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
  LoadInstrIR* load_ir = new LoadInstrIR(value, getTmp());
  ir_module.pushValueToBasicBlock((ValueIR*)load_ir);
  return (ValueIR*)load_ir;
}

std::any IRGenVisitor::visitUnaryExp(SysYParser::UnaryExpContext* ctx) {
  if (ctx->primaryExp()) {
    return visitPrimaryExp(ctx->primaryExp());
  } else if (ctx->funcName()) {
    // TODO
    return nullptr;
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
      return (ValueIR*)int_ir;
    }
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
    return (ValueIR*)bin_ir;
  } else {
    return visitMulExp(ctx->mulExp());
  }
}

std::any IRGenVisitor::visitRelExp(SysYParser::RelExpContext* ctx) {
  if (ctx->relExp()) {
    ValueIR* lvalue = std::any_cast<ValueIR*>(visitRelExp(ctx->relExp()));
    ValueIR* rvalue = std::any_cast<ValueIR*>(visitAddExp(ctx->addExp()));
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
      return (ValueIR*)int_ir;
    }
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
    return (ValueIR*)rel_ir;
  } else {
    return visitAddExp(ctx->addExp());
  }
}

std::any IRGenVisitor::visitEqExp(SysYParser::EqExpContext* ctx) {
  if (ctx->eqExp()) {
    ValueIR* lvalue = std::any_cast<ValueIR*>(visitEqExp(ctx->eqExp()));
    ValueIR* rvalue = std::any_cast<ValueIR*>(visitRelExp(ctx->relExp()));
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
      return (ValueIR*)int_ir;
    }
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
    return (ValueIR*)eq_ir;
  } else {
    return visitRelExp(ctx->relExp());
  }
}

std::any IRGenVisitor::visitLAndExp(SysYParser::LAndExpContext* ctx) {
  if (ctx->lAndExp()) {
    ValueIR* lvalue = std::any_cast<ValueIR*>(visitLAndExp(ctx->lAndExp()));
    ValueIR* rvalue = std::any_cast<ValueIR*>(visitEqExp(ctx->eqExp()));
    if (lvalue->tag == IRV_INTEGER && rvalue->tag == IRV_INTEGER) {
      IntegerValueIR* int_ir = new IntegerValueIR();
      int_ir->number = ((IntegerValueIR*)lvalue)->number &&
                       ((IntegerValueIR*)rvalue)->number;

      ir_module.pushValueToBasicBlock((ValueIR*)int_ir);
      return (ValueIR*)int_ir;
    }
    auto zero = new IntegerValueIR();
    zero->number = 0;
    ir_module.pushValueToBasicBlock((ValueIR*)zero);
    BinaryOpInstrIR* ne_ir1 = new BinaryOpInstrIR();
    ne_ir1->left = zero;
    ne_ir1->right = lvalue;
    ne_ir1->name = getTmp();
    ne_ir1->op_type = OP_NEQ;
    ir_module.pushValueToBasicBlock((ValueIR*)ne_ir1);

    BinaryOpInstrIR* ne_ir2 = new BinaryOpInstrIR();
    ne_ir2->left = zero;
    ne_ir2->right = rvalue;
    ne_ir2->name = getTmp();
    ne_ir2->op_type = OP_NEQ;
    ir_module.pushValueToBasicBlock((ValueIR*)ne_ir2);

    BinaryOpInstrIR* land_ir = new BinaryOpInstrIR();
    land_ir->op_type = OP_AND;
    land_ir->left = ne_ir1;
    land_ir->right = ne_ir2;
    land_ir->name = getTmp();
    ir_module.pushValueToBasicBlock((ValueIR*)land_ir);
    return (ValueIR*)land_ir;
  } else {
    return visitEqExp(ctx->eqExp());
  }
}

std::any IRGenVisitor::visitLOrExp(SysYParser::LOrExpContext* ctx) {
  if (ctx->lOrExp()) {
    ValueIR* lvalue = std::any_cast<ValueIR*>(visitLOrExp(ctx->lOrExp()));
    ValueIR* rvalue = std::any_cast<ValueIR*>(visitLAndExp(ctx->lAndExp()));
    if (lvalue->tag == IRV_INTEGER && rvalue->tag == IRV_INTEGER) {
      IntegerValueIR* int_ir = new IntegerValueIR();
      int_ir->number = ((IntegerValueIR*)lvalue)->number ||
                       ((IntegerValueIR*)rvalue)->number;

      ir_module.pushValueToBasicBlock((ValueIR*)int_ir);
      return (ValueIR*)int_ir;
    }
    BinaryOpInstrIR* lor_ir = new BinaryOpInstrIR();
    lor_ir->op_type = OP_OR;
    lor_ir->left = lvalue;
    lor_ir->right = rvalue;
    lor_ir->name = getTmp();
    ir_module.pushValueToBasicBlock((ValueIR*)lor_ir);

    BinaryOpInstrIR* ne_ir = new BinaryOpInstrIR();
    auto zero = new IntegerValueIR();
    zero->number = 0;
    ir_module.pushValueToBasicBlock((ValueIR*)zero);
    ne_ir->left = zero;
    ne_ir->right = lor_ir;
    ne_ir->name = getTmp();
    ne_ir->op_type = OP_NEQ;
    ir_module.pushValueToBasicBlock((ValueIR*)ne_ir);
    return (ValueIR*)ne_ir;
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
  std::string var_name = ctx->IDENT()->getText();
  ValueIR* value =
      std::any_cast<ValueIR*>(visitConstInitVal(ctx->constInitVal()));
  if (!ir_module.putVar(var_name, value)) {
    // Error hanlder
    std::cerr << "Redefine var: " << var_name << std::endl;
    exit(1);
  }
  return nullptr;
}

std::any IRGenVisitor::visitConstInitVal(SysYParser::ConstInitValContext* ctx) {
  if (ctx->constExp()) {
    return visitConstExp(ctx->constExp());
  } else {
    // TODO
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
  std::string var_name = ctx->IDENT()->getText();
  VariableIR* variable = new VariableIR(Int32Type(), var_name);
  ir_module.pushValueToBasicBlock((ValueIR*)variable);
  AllocInstrIR* alloc_ir = new AllocInstrIR(variable);
  ir_module.pushValueToBasicBlock((ValueIR*)alloc_ir);
  if (ctx->initVal()) {
    ValueIR* value = std::any_cast<ValueIR*>(visitInitVal(ctx->initVal()));
    StoreInstrIR* store_ir = new StoreInstrIR(value, (ValueIR*)alloc_ir);
    ir_module.pushValueToBasicBlock((ValueIR*)store_ir);
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
    // TODO
    return visitChildren(ctx);
  }
}