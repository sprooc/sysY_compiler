#include <memory>
#include <string>
#include <vector>
/*-------------- Type --------------------*/
typedef enum {
  IRT_VOID,
  IRT_INT32,
  IRT_FLOAT,
  IRT_ARRAY,
  IRT_POINTER,
  IRT_FUNCTION,
} TypeTag;

class Type {
 public:
  Type() {}
  Type(TypeTag t) : tag(t) {}
  TypeTag tag;
  std::string toString() const {
    switch (tag) {
      case IRT_VOID:
        return "";
      case IRT_INT32:
        return "i32";
      case IRT_FLOAT:
        return "float";
      case IRT_POINTER:
        return "pointer";
      case IRT_FUNCTION:
        return "function";
      default:
        return "errorType";
    }
  }
};
class VoidType : public Type {
 public:
  VoidType() : Type(IRT_VOID) {}
};

class Int32Type : public Type {
 public:
  Int32Type() : Type(IRT_INT32) {}
};

class FloatType : public Type {
 public:
  FloatType() : Type(IRT_FLOAT) {}
};

class ArrayType : public Type {
 public:
  ArrayType() : Type(IRT_ARRAY) {}
  std::unique_ptr<Type> elem_type;
  size_t len;
};

class PointerType : public Type {
 public:
  PointerType() : Type(IRT_POINTER) {}
  std::unique_ptr<Type> elem_type;
};

class FunctionType : public Type {
 public:
  FunctionType() : Type(IRT_FUNCTION) {}
  std::unique_ptr<Type> ret_type;
  std::vector<std::unique_ptr<Type>> params;
};
/*----------------- Type end -------------------------*/