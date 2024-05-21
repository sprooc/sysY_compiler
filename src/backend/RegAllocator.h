#include <cassert>
#include <iostream>
#include <map>
#include <queue>
#include <string>

class RegAllocator {
 private:
  const std::string reg_name[16] = {"x0", "t0", "t1", "t2", "t3", "t4",
                                    "t5", "t6", "a0", "a1", "a2", "a3",
                                    "a4", "a5", "a6", "a7"};
  std::map<std::string, int> alloc_map;
  int upper = 1;
  std::queue<int> free_list;

 public:
  int GetOne() {
    if (free_list.empty()) {
      return upper++;
    } else {
      int r = free_list.front();
      free_list.pop();
      return r;
    }
  }
  int GetOne(std::string& key) {
    int r = GetOne();
    alloc_map[key] = r;
    return r;
  }
  void free(int r) {
    for (auto it = alloc_map.begin(); it != alloc_map.end(); it++) {
      if (it->second == r) {
        alloc_map.erase(it);
        break;
      }
    }
    free_list.push(r);
  }

  const std::string*  GetName(int r) {
    assert(r < 16);
    return &reg_name[r];
  }

  int GetByName(std::string& key) {
    auto loc = alloc_map.find(key);
    if (loc != alloc_map.end()) {
      return loc->second;
    }
    return -1;
  }
};