#include <cassert>

#include "../frontend/IR.h"
#include "../frontend/IRGenVisitor.h"
#include "RegAllocator.h"
extern std::ofstream out_file;
class CodeGenVisitor {
 private:
  RegAllocator reg_alloc;
  void OutCode(std::string instr, const std::string* rd, const std::string* rs1,
               const std::string* rs2);
  void OutCode(std::string instr, const std::string* rd, const std::string* rs1,
               int imm);
  void OutCode(std::string instr, const std::string* rd, int imm);
  void OutCode(std::string instr, const std::string* rd, const std::string* rs);
  void OutCode(std::string instr);
  void OutCode(std::string instr, int imm, const std::string* rs1,
               const std::string* rs2);
  void emitCodeR(std::string instr, int rd, int rs1, int rs2);
  void emitCodeI(std::string instr, int rd, int rs1, int imm);
  void emitCodeS(std::string instr, int imm, int rs1, int rs2);
  void emitCodeB(std::string instr, int imm, int rs1, int rs2);
  void emitCodeU(std::string instr, int rd, int imm);
  void emitCodeJ(std::string instr, int rd, int rs);
  void emitCodePI(std::string instr);
  void emitCodePIRR(std::string instr, int rd, int rs);

 public:
  void Visit(ProgramIR* program);
  void Visit(FunctionIR* function);
  void Visit(BasicBlockIR* basic_block);
  void Visit(ValueIR* value);
  void Visit(ReturnValueIR* return_value);
  void Visit(IntegerValueIR* integer_value);
  void Visit(BinaryOpInstrIR* binary_op_instr);
};