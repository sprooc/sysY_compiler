#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "antlr4-runtime/ANTLRInputStream.h"
#include "antlr4-runtime/CommonTokenStream.h"
#include "IRGenVisitor.h"
#include "SysYLexer.h"
#include "SysYParser.h"
#include "antlr4-runtime/antlr4-runtime.h"
#include "tree/ParseTreeWalker.h"

using namespace std;
using namespace antlr4;


const char *koopa_mode = "-koopa";
const char *riscv_mod = "-riscv";

std::ofstream out_file;
int main(int argc, const char *argv[]) {

  out_file.open(argv[4]);

  // std::ifstream file(input);
  // std::string line;
  // int i = 0;
  // while (std::getline(file, line)) {
  //   if (i++ == 1) out_file << line << std::endl;  // 输出每一行到标准输出
  // }

  std::ifstream sourceFile(argv[2]);
  ANTLRInputStream input(sourceFile);

  SysYLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SysYParser parser(&tokens);
  IRGenVisitor visitor;
  visitor.visitProgram(parser.program());
  visitor.PrintResult();
  return 0;

  // // 输出解析得到的 AST, 其实就是个字符串
  // // ast->Dump();
  // IRGenerateVisitor IR_visitor;
  // IR_visitor.Visit((CompUnit *)ast.get());

  // std::string mode_str = std::string(mode);
  // if (mode_str == "-koopa") {
  //   IR_visitor.PrintResult();
  // } else if (mode_str == "-riscv") {
  //   CodeGenVisitor cg_visitor;
  //   cg_visitor.Visit(IR_visitor.GetProgramIR());
  // }
  // return 0;
}
