#include "MenAllocator.h"
void MenAllocator::enterFunction() {
  men_size = 0;
  men_table = std::make_unique<unordered_map<string, int>>();
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
  return men_table->find(name)->second + max_ps * 4;
}

void MenAllocator::noticeCall(int ps) {
  has_call = true;
  max_ps = std::max(max_ps, ps);
}
