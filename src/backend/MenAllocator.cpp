#include "MenAllocator.h"

void MenAllocator::enterFunction() {
  men_size = 0;
  men_table = std::make_unique<unordered_map<string, int>>();
}
void MenAllocator::exitFunxtion() {
  men_size = 0;
  men_table.reset();
}
void MenAllocator::alloc(string name, int size) {
  men_table->emplace(name, men_size);
  men_size += size;
}
int MenAllocator::getStackSize() {
  return (men_size + 15) & ~15;
}
int MenAllocator::getLoc(string name) {
  return men_table->find(name)->second;
}