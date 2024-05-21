#include <memory>
#include <string>
#include <unordered_map>
using std::string;
using std::unique_ptr;
using std::unordered_map;

class MenAllocator {
 private:
  unique_ptr<unordered_map<string, int>> men_table;
  int men_size;
  bool has_call;
  int max_ps;

 public:
  MenAllocator() {}
  void enterFunction();
  void exitFunxtion();
  void alloc(string name, int size);
  int getStackSize();
  int getLoc(string name);
  void noticeCall(int ps);
  bool hasCall() { return has_call; }
};