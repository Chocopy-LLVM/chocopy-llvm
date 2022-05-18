//
// Created by yiwei yang on 2/15/21.
//

#ifndef CHOCOPY_COMPILER_FUNCTION_HPP
#define CHOCOPY_COMPILER_FUNCTION_HPP

#pragma once

#include "BasicBlock.hpp"
#include "Module.hpp"
#include "Type.hpp"
#include "User.hpp"
#include <cassert>
#include <cstddef>
#include <iterator>
#include <list>
#include <map>

using std::map;
using std::string;
using std::vector;

namespace lightir {
class Module;
class Argument;
class BasicBlock;
class Type;
class FunctionType;
class Class;

class Function : public Value {
public:
    Function(FunctionType *ty, const string &name, Module *parent);
    ~Function() = default;

    static Function *create(FunctionType *ty, const string &name, Module *parent);
    static Function *create(bool is_ctor, FunctionType *ty, const string &name, Module *parent);

    FunctionType *get_function_type() const;

    Type *get_return_type() const;

    void add_basic_block(BasicBlock *bb);

    unsigned get_num_of_args() const;
    unsigned get_num_basic_blocks() const;

    Module *get_parent() const;

    list<Argument *>::iterator arg_begin() { return arguments_.begin(); }
    list<Argument *>::iterator arg_end() { return arguments_.end(); }

    void remove(BasicBlock *bb);
    BasicBlock *get_entry_block() { return *basic_blocks_.begin(); }

    list<BasicBlock *> &get_basic_blocks() { return basic_blocks_; }
    list<Argument *> &get_args() { return arguments_; }

    bool is_declaration() { return basic_blocks_.empty(); }

    void set_instr_name();
    string print() override;
    string print_method(Class *method_);
    string print_args();
    bool is_ctor = false;

private:
    void build_args();

private:
    list<BasicBlock *> basic_blocks_; /* basic blocks */
    list<Argument *> arguments_; /* arguments */
    Module *parent_;
    unsigned seq_cnt_;
};

/* Argument of Function, does not contain actual value. */
class Argument : public Value {
public:
    /* Argument constructor.*/
    explicit Argument(Type *ty, const string &name = "", Function *f = nullptr, unsigned arg_no = 0)
        : Value(ty, name), parent_(f), arg_no_(arg_no) {}
    ~Argument() = default;

    inline const Function *get_parent() const { return parent_; }
    inline Function *get_parent() { return parent_; }

    /* For example in "void foo(int a, float b)" a is 0 and b is 1. */
    unsigned get_arg_no() const {
        assert(parent_ && "can't get number of unparented arg");
        return arg_no_;
    }

    string print() override;

private:
    Function *parent_;
    unsigned arg_no_; /* argument No. */
};
} // namespace lightir
#endif // CHOCOPY_COMPILER_FUNCTION_HPP
