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
  virtual std::string toString() const { return ""; }
};
class VoidType : public Type {
 public:
  VoidType() : Type(IRT_VOID) {}
  std::string toString() const override { return ""; }
};

class Int32Type : public Type {
 public:
  Int32Type() : Type(IRT_INT32) {}
  std::string toString() const override { return "i32"; }
};

class FloatType : public Type {
 public:
  FloatType() : Type(IRT_FLOAT) {}
  std::string toString() const override { return "float"; }
};

class ArrayType : public Type {
 public:
  ArrayType() : Type(IRT_ARRAY) {}
  ArrayType(int n, Type* et)
      : Type(IRT_ARRAY), len(n), elem_type(std::unique_ptr<Type>(et)) {}
  std::unique_ptr<Type> elem_type;
  size_t len;
  std::string toString() const override {
    return "[" + elem_type->toString() + ", " + std::to_string(len) + "]";
  }
  int getDimen() {
    if (elem_type->tag != IRT_ARRAY) {
      return 1;
    } else {
      return 1 + ((ArrayType*)elem_type.get())->getDimen();
    }
  }
};

class PointerType : public Type {
 public:
  PointerType() : Type(IRT_POINTER) {}
  PointerType(Type* e)
      : Type(IRT_POINTER), elem_type(std::unique_ptr<Type>(e)) {}
  std::unique_ptr<Type> elem_type;
  std::string toString() const override { return "*" + elem_type->toString(); }
};

class FunctionType : public Type {
 public:
  FunctionType() : Type(IRT_FUNCTION) {}
  std::unique_ptr<Type> ret_type;
  std::vector<std::unique_ptr<Type>> params;
  std::string toString() const override { return "function"; }
};
/*----------------- Type end -------------------------*/