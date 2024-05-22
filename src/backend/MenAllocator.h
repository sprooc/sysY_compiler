#include <memory>
#include <string>
#include "../frontend/Type.h"
#include <unordered_map>
#include <iostream>
#include <unordered_set>
using std::string;
using std::unique_ptr;
using std::unordered_map;
using std::unordered_set;

class MenAllocator {
 private:
  unique_ptr<unordered_map<string, int>> men_table;
  unique_ptr<unordered_map<string, Type*>> type_table;
  unique_ptr<unordered_set<string>> dym_ptr;
  int men_size;
  bool has_call;
  int max_ps;

 public:
  MenAllocator() {}
  void enterFunction();
  void exitFunxtion();
  void alloc(string name, int size);
  void alloc(string name, Type* type);
  int getStackSize();
  int getLoc(string name);
  void noticeCall(int ps);
  bool hasCall() { return has_call; }
  Type* getType(string name);
  void setType(string name, Type* type);
  void setDymPtr(string name);
  bool isDymPtr(string name);
};