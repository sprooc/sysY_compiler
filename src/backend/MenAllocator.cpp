#include "MenAllocator.h"
void MenAllocator::enterFunction() {
  men_size = 0;
  men_table = std::make_unique<unordered_map<string, int>>();
  type_table = std::make_unique<unordered_map<string, Type*>>();
  dym_ptr = std::make_unique<unordered_set<string>>();
  has_call = false;
  max_ps = 0;
}
void MenAllocator::exitFunxtion() {
  men_size = 0;
  men_table.reset();
}
void MenAllocator::alloc(string name, int size) {
  men_table->emplace(name, men_size);
  men_size += size;
}

void MenAllocator::alloc(string name, Type* type) {
  alloc(name, type->getSize());
  type_table->emplace(name, type);
}

Type* MenAllocator::getType(string name) {
  auto it = type_table->find(name);
  if (it == type_table->end()) {
    return nullptr;
    // std::cerr << "Cannot find type of " << name << std::endl;
    // exit(1);
  }
  return it->second;
}

void MenAllocator::setType(string name, Type* type) {
  if (type_table->find(name) == type_table->end()) {
    type_table->emplace(name, type);
  }
}

int MenAllocator::getStackSize() {
  int size = men_size;
  if (has_call) {
    size += 4;
  }
  if (max_ps > 8) {
    size += (4 * (max_ps - 8));
  }
  return (size + 15) & ~15;
}

int MenAllocator::getLoc(string name) {
  auto it = men_table->find(name);
  if (it == men_table->end()) {
    std::cerr << "Cannot find: " << name << std::endl;
    // exit(1);
  }
  return it->second + std::max(max_ps - 8, 0) * 4;
}

void MenAllocator::noticeCall(int ps) {
  has_call = true;
  max_ps = std::max(max_ps, ps);
}

void MenAllocator::setDymPtr(string name) { dym_ptr->insert(name); }
bool MenAllocator::isDymPtr(string name) {
  return dym_ptr->find(name) != dym_ptr->end();
}
