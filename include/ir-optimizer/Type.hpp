//
// Created by yiwei yang on 2/15/21.
//

#ifndef CHOCOPY_COMPILER_TYPE_HPP
#define CHOCOPY_COMPILER_TYPE_HPP

#include "Constant.hpp"
#include "Value.hpp"
#include <chocopy_parse.hpp>
#include <chocopy_semant.hpp>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;
namespace lightir {

class Module;
class IntegerType;
class FunctionType;
class ArrayType;
class StringType;
class Class;
class ConstantArray;

class Type {
public:
    enum type { CLASS_ANON = -7, LABEL, FUNC, VOID, UNION, VECTOR, LIST, OBJECT, INT, BOOL, STRING, CLASS };

    explicit Type(type tid, Module *m);
    explicit Type(type tid) : Type(tid, nullptr){};
    ~Type() = default;

    constexpr int get_type_id() const { return get_underlying<type>(tid_); }

    constexpr bool is_class_anon() const { return tid_ == type::CLASS_ANON; }

    constexpr bool is_func_type() const { return tid_ == type::FUNC; }

    constexpr bool is_object_type() const { return tid_ == type::OBJECT; }

    constexpr bool is_vector_type() const { return tid_ == type::VECTOR; }

    constexpr bool is_list_type() const { return tid_ == type::LIST; }

    constexpr bool is_union_type() const { return tid_ == type::UNION; }

    constexpr bool is_array_type();

    constexpr bool is_ptr_type();

    constexpr bool is_void_type() const { return tid_ == type::VOID; /** reserved type */ }

    constexpr bool is_integer_type() const { return tid_ == type::INT; }

    constexpr bool is_bool_type() const { return tid_ == type::BOOL; /** same as int 1 */ }

    constexpr bool is_string_type() const { return tid_ == type::STRING; }

    constexpr bool is_value_type() const { return is_bool_type() || is_string_type() || is_integer_type(); }

    constexpr bool is_class_type() const { return tid_ >= type::CLASS; }

    static Type *get_void_type(Module *m);

    static Type *get_label_type(Module *m);

    static Type *get_class_type(Module *m, int id_);

    static IntegerType *get_int1_type(Module *m);

    static IntegerType *get_int32_type(Module *m);

    static StringType *get_str_type(Module *m);

    static ArrayType *get_array_type(Module *m);

    static ArrayType *get_array_type(Type *contained);

    Type *get_array_element_type();

    Type *get_ptr_element_type();

    Module *get_module();

    virtual int get_size();

    virtual string print();

#if __cplusplus > 202000L && !defined(__clang__)
    std::strong_ordering operator<=>(Type rhs);
#else
    bool operator==(Type rhs);
    bool operator>(Type rhs);
    bool operator<(Type rhs);
    bool operator<=(Type rhs);
    bool operator>=(Type rhs);
#endif
private:
    const type tid_;
    Module *m_;
};

class IntegerType : public Type {
public:
    explicit IntegerType(unsigned num_bits, Module *m);

    static IntegerType *get(unsigned num_bits, Module *m);

    unsigned get_num_bits() const;

    virtual string print() { return fmt::format("i{}", get_num_bits()); }

private:
    unsigned num_bits_;
};

class FunctionType : public Type {
public:
    FunctionType(Type *result, const vector<Type *> &params);

    static FunctionType *get(Type *result, const vector<Type *> &params);

    static FunctionType *get(Type *result, const vector<Type *> &params, bool is_variable_args);

    unsigned get_num_of_args() const;

    Type *get_param_type(unsigned i) const;
    vector<Type *>::iterator param_begin() { return args_.begin(); }
    vector<Type *>::iterator param_end() { return args_.end(); }
    Type *get_return_type() const;
    Type *get_arg_type(unsigned i) const;

    virtual string print();
    bool is_variable_args = false;

private:
    Type *result_;
    vector<Type *> args_;
};

/** Contains both pointer type and array */
class ArrayType : public Type {
public:
    ArrayType(Type *contained, unsigned num_elements);
    explicit ArrayType(Type *Contained); // Do not specify the number of elements;
    static ArrayType *get(Type *contained, unsigned num_elements);
    static ArrayType *get(Type *contained);

    Type *get_element_type() const { return contained_; }
    int get_num_of_elements() const { return num_elements_; }
    void set_num_of_elements(int num_) { num_elements_ = num_; }

    virtual string print();

private:
    Type *contained_; // The element type of the array.
    int num_elements_; // Number of elements in the array. -1 means ptr
};

/** For use of duck typing in function passing and list dispatch table,
 *  connected with @Bitcast */
class Union : public Type, public Value {
public:
    Union(vector<Type *> contained, string name);
    static Union *get(vector<Type *> contained, string name);
    static Union *get(Module *m_, string name);

    vector<Type *> get_union_type() const { return contained_; }
    virtual string print();
    virtual string get_name() { return "$union." + this->name_; };
    int length_ = 0;

private:
    vector<Type *> contained_; // The element types of the union.
    string name_;
};

/** For use of vectorization, connected with @InsertElement and @ExtractElement */
class VectorType : public Type {
public:
    VectorType(Value *contained, unsigned num_elements);
    VectorType(Value *contained);
    static VectorType *get(Value *contained, unsigned num_elements);
    static VectorType *get(ConstantArray *contained);

    Value *get_element_value() const { return contained_; }
    unsigned get_num_of_elements() const { return num_elements_; }
    void set_num_of_elements(int num_) { num_elements_ = num_; }

    virtual string print();
    string print_as_op();

private:
    Value *contained_; // The element type of the vector.
    unsigned num_elements_; // Number of elements in the array. typically 4
};

class StringType : public Type {
public:
    explicit StringType(string str_, Module *m);

    static StringType *get(string str_, Module *m);

    string get_string() const;

    virtual string print() { return "i8*"; }

private:
    string str_;
};

class LabelType : public Type {
public:
    explicit LabelType(string str_, Class *stored_, Module *m);

    static LabelType *get(string str_, Class *stored_, Module *m);

    string get_label() const;
    Class *get_class() const { return stored_; };

    virtual string print() { return "%"+label_+"_type"; }

private:
    string label_;
    Class *stored_;
};

class VoidType : public Type {
public:
    explicit VoidType(Module *m) : Type(Type::type::VOID, m){};

    static VoidType *get(Module *m) { return new VoidType(m); };

    virtual string print() { return "void"; }
};

} // namespace lightir

#endif // CHOCOPY_COMPILER_TYPE_HPP
