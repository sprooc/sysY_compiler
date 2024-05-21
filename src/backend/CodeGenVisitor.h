#include <cassert>

#include "../frontend/IR.h"
#include "../frontend/IRGenVisitor.h"
#include "MenAllocator.h"
#include "RegAllocator.h"
extern std::ofstream out_file;
class CodeGenVisitor {
 private:
  enum { SCAN, GEN } state;
  RegAllocator reg_alloc;
  MenAllocator men_alloc;
  void outCode(std::string instr, const std::string* rd, const std::string* rs1,
               const std::string* rs2);
  void outCode(std::string instr, const std::string* rd, const std::string* rs1,
               int imm);
  void outCode(std::string instr, const std::string* rd, int imm);
  void outCode(std::string instr, const std::string* rd, const std::string* rs);
  void outCode(std::string instr);
  void outCode(std::string instr, int imm, const std::string* rs1,
               const std::string* rs2);
  void outCode(std::string instr, const std::string* r, std::string label);
  void outCode(std::string instr, std::string label);
  void outCodeOffset(std::string instr, const std::string* rd,
                     const std::string* rs1, int imm);
  void outLabel(std::string label);
  void emitCodeR(std::string instr, int rd, int rs1, int rs2);
  void emitCodeI(std::string instr, int rd, int rs1, int imm);
  void emitCodeS(std::string instr, int imm, int rs1, int rs2);
  void emitCodeB(std::string instr, int imm, int rs1, int rs2);
  void emitCodeU(std::string instr, int rd, int imm);
  void emitCodeJ(std::string instr, int rd, int rs);
  void emitCodePI(std::string instr);
  void emitCodeIRL(std::string instr, int r, std::string label);
  void emitCodeIL(std::string instr, std::string label);
  void emitCodePIRR(std::string instr, int rd, int rs);
  int loadFromMen(ValueIR* value);

 public:
  void visit(ProgramIR* program);
  void visit(FunctionIR* function);
  void visit(BasicBlockIR* basic_block);
  void visit(ValueIR* value);
  void visit(ReturnValueIR* return_value);
  void visit(IntegerValueIR* integer_value);
  void visit(BinaryOpInstrIR* binary_op_instr);
  void visit(AllocInstrIR* alloc_instr);
  void visit(LoadInstrIR* load_instr);
  void visit(StoreInstrIR* store_instr);
  void visit(JumpInstrIR* jump_instr);
  void visit(BrInstrIR* br_instr);
  void visit(CallInstrIR* br_instr);
};