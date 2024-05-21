#include "Module.h"

#include <memory>

#include "IR.h"
#define N_PLOAD 8

void Module::preLoad() {
  std::string par_name = "x";
  Type* ret_type[] = {new Int32Type(), new Int32Type(), new Int32Type(),
                      new VoidType(),  new VoidType(),  new VoidType(),
                      new VoidType(),  new VoidType()};
  std::string name[] = {"getint", "getch",    "getarray",  "putint",
                        "putch",  "putarray", "starttime", "stoptime"};
  std::vector<ParamIR*> params[] = {
      std::vector<ParamIR*>(),
      std::vector<ParamIR*>(),
      std::vector<ParamIR*>({new ParamIR(new PointerType(new Int32Type()), par_name)}),
      std::vector<ParamIR*>({new ParamIR(new Int32Type(), par_name)}),
      std::vector<ParamIR*>({new ParamIR(new Int32Type(), par_name)}),
      std::vector<ParamIR*>({new ParamIR(new Int32Type(), par_name),
                             new ParamIR(new PointerType(new Int32Type()), par_name)}),
      std::vector<ParamIR*>(),
      std::vector<ParamIR*>()};
  for (int i = 0; i < N_PLOAD; i++) {
    FunctionIR* func_ir = new FunctionIR(ret_type[i], name[i]);
    for (ParamIR* p : params[i]) {
      func_ir->params.push_back(std::unique_ptr<ParamIR>(p));
    }
    declFunction(func_ir);
  };
};