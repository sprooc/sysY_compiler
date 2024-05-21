#include <cassert>
#include "../frontend/IR.h"
#include "../frontend/IRGenVisitor.h"
#include "RegAllocator.h"
extern std::ofstream out_file;
class CodeGenVisitor {
 private:
  RegAllocator reg_alloc;
  void OutCode(const std::string* instr, const std::string* rd,
               const std::string* rs1, const std::string* rs2);
  void OutCode(const std::string* instr, const std::string* rd, int imm);
  void OutCode(const std::string* instr, const std::string* rd,
               const std::string* rs);
  void OutCode(const std::string* instr);

 public:
  void Visit(ProgramIR* program);
  void Visit(FunctionIR* function);
  void Visit(BasicBlockIR* basic_block);
  void Visit(ValueIR* value);
  void Visit(ReturnValueIR* return_value);
  void Visit(IntegerValueIR* integer_value);
  void Visit(BinaryOpInstrIR* binary_op_instr);
};