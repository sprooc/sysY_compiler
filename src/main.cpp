#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "antlr4-runtime/ANTLRInputStream.h"
#include "antlr4-runtime/CommonTokenStream.h"
#include "antlr4-runtime/antlr4-runtime.h"
#include "backend/CodeGenVisitor.h"
#include "frontend/IRGenVisitor.h"
#include "frontend/SysYLexer.h"
#include "frontend/SysYParser.h"
#include "tree/ParseTreeWalker.h"

using namespace std;
using namespace antlr4;

const char *koopa_mode = "-koopa";
const char *riscv_mod = "-riscv";

std::ofstream out_file;
int main(int argc, const char *argv[]) {
  out_file.open(argv[4]);


  std::ifstream sourceFile(argv[2]);
  ANTLRInputStream input(sourceFile);

  SysYLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SysYParser parser(&tokens);
  IRGenVisitor visitor;
  visitor.visitProgram(parser.program());
  visitor.PrintResult();
  CodeGenVisitor code_gen;
  // code_gen.visit(visitor.getProgram());

  return 0;

}
