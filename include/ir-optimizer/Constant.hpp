//
// Created by yiwei yang on 2/15/21.
//

#ifndef CHOCOPY_COMPILER_CONSTANT_HPP
#define CHOCOPY_COMPILER_CONSTANT_HPP

#include "Type.hpp"
#include "User.hpp"
#include "Value.hpp"
#include <chocopy_parse.hpp>
#include <map>
#include <utility>

using std::map;
using std::string;
using std::vector;
namespace lightir {
class Module;
class ArrayType;
class Constant : public User {
public:
    explicit Constant(Type *ty, const string &name = "", unsigned num_ops = 0) : User(ty, name, num_ops) {}
    ~Constant() = default;
};

class ConstantStr : public Constant {
private:
    string value_;
    int id_;
    bool header_print_ = true;

public:
    static string get_value(ConstantStr *const_val) { return const_val->value_; }
    string get_name() override { return fmt::format("const_{}", id_); }
    string get_value() const { return value_; }
    int get_id() const { return id_; }
    static ConstantStr *get(const string &val, int id, Module *m);
    string print() override;
    ConstantStr(Type *ty, string val, int id) : Constant(ty, "", 0), id_(id), value_(std::move(val)) {}
};

class ConstantInt : public Constant {
private:
    int value_;

public:
    ConstantInt(Type *ty, int val) : Constant(ty, "", 0), value_(val) {}
    static int get_value(ConstantInt *const_val) { return const_val->value_; }
    int get_value() const { return value_; }
    static ConstantInt *get(int val, Module *m);
    static ConstantInt *get(bool val, Module *m);
    void set_value(int value) { value_ = value; }
    string print() override;
};

class ConstantArray : public Constant {
private:
    ConstantArray(ArrayType *ty, const vector<Constant *> &val);

public:
    ~ConstantArray() = default;

    Constant *get_element_value(int index);

    unsigned get_size_of_array() const { return const_array.size(); }

    void set_const_array(const vector<Constant *> &new_array);

    static ConstantArray *get(ArrayType *ty, const vector<Constant *> &val) { return new ConstantArray(ty, val); };

    string print() override;
    vector<Constant *> const_array;
};

class ConstantZero : public Constant {
private:
    explicit ConstantZero(Type *ty) : Constant(ty, "", 0) {}

public:
    static ConstantZero *get(Type *ty, Module *m);
    string print() override;
};
} // namespace lightir
#endif // CHOCOPY_COMPILER_CONSTANT_HPP
