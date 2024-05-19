#ifndef LABEL_H
#define LABEL_H

#include <string>
#include "IR.h"

class BasicBlockIR;

static int label_id = 0;
struct Label {
  std::string name;
  BasicBlockIR* basic_block;
  Label() : name("L"+ std::to_string(label_id++)) {}
  Label(const std::string& n) : name(n) {}
  std::string toString() { return "%" + name; }
};
#endif