#ifndef SCOPE_H
#define SCOPE_H

#include <string>
#include <unordered_map>

#include "IR.h"
#include "Module.h"

class Scope {
 private:
  Scope* parent_scope;
  std::unordered_map<std::string, ValueIR*> symbol_table;
  friend class Module;

 public:
  Scope(Scope* parent) : parent_scope(parent) {}

  bool contain(const std::string& key) const {
    return symbol_table.find(key) != symbol_table.end();
  }

  bool put(const std::string& key, ValueIR* value) {
    if (contain(key)) {
      return false;
    }
    symbol_table[key] = value;
    return true;
  }

  ValueIR* find(const std::string& key) const {
    auto el = symbol_table.find(key);
    if (el != symbol_table.end()) {
      return el->second;
    }
    if (parent_scope) {
      return parent_scope->find(key);
    }
    return nullptr;
  }
};

#endif