#include "Function.hpp"
#include "IRprinter.hpp"
#include "Module.hpp"

namespace lightir {

Function::Function(FunctionType *ty, const std::string &name, Module *parent)
    : Value(ty, name), parent_(parent), seq_cnt_(0) {
    parent->add_function(this);
    build_args();
}

Function *Function::create(FunctionType *ty, const std::string &name, Module *parent) {
    return new Function(ty, name, parent);
}

Function *Function::create(bool is_ctor, FunctionType *ty, const std::string &name, Module *parent) {
    auto func = new Function(ty, name, parent);
    func->is_ctor = is_ctor;
    return func;
}

FunctionType *Function::get_function_type() const { return dynamic_cast<FunctionType *>(get_type()); }

Type *Function::get_return_type() const { return get_function_type()->get_return_type(); }

unsigned Function::get_num_of_args() const { return get_function_type()->get_num_of_args(); }

unsigned Function::get_num_basic_blocks() const { return basic_blocks_.size(); }

Module *Function::get_parent() const { return parent_; }

void Function::remove(BasicBlock *bb) {
    basic_blocks_.remove(bb);
    for (auto pre : bb->get_pre_basic_blocks()) {
        pre->remove_succ_basic_block(bb);
    }
    for (auto succ : bb->get_succ_basic_blocks()) {
        succ->remove_pre_basic_block(bb);
    }
}

void Function::build_args() {
    auto *func_ty = get_function_type();
    unsigned num_args = get_num_of_args();
    for (int i = 0; i < num_args; i++) {
        arguments_.push_back(new Argument(func_ty->get_param_type(i), "", this, i));
    }
}

void Function::add_basic_block(BasicBlock *bb) { basic_blocks_.push_back(bb); }

void Function::set_instr_name() {
    std::map<Value *, int> seq;
    for (auto arg : this->get_args()) {
        if (seq.find(arg) == seq.end()) {
            auto seq_num = seq.size() + seq_cnt_;
            if (arg->set_name("arg" + std::to_string(seq_num))) {
                seq.insert({arg, seq_num});
            }
        }
    }
    for (auto &&bb : basic_blocks_) {
        if (seq.find(bb) == seq.end()) {
            auto seq_num = seq.size() + seq_cnt_;
            if (bb->set_name("label" + std::to_string(seq_num))) {
                seq.insert({bb, seq_num});
            }
        }
        for (auto instr : bb->get_instructions()) {
            if (!instr->is_void() && seq.find(instr) == seq.end()) {
                auto seq_num = seq.size() + seq_cnt_;
                if (instr->set_name("op" + std::to_string(seq_num))) {
                    seq.insert({instr, seq_num});
                }
            }
        }
    }
    seq_cnt_ += seq.size();
}

std::string Function::print() {
    std::string func_ir;
    if (this->is_ctor) {
        func_ir += "@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 "
                   "65535, void ()* @" +
                   this->get_name() + ", i8* null }]\n";
    }
    if (this->is_declaration()) {
        func_ir += "declare ";
    } else {
        func_ir += "define ";
    }

    func_ir += this->get_return_type()->print();
    func_ir += " ";
    func_ir += print_as_op(this, false);
    func_ir += "(";

    /** print arg */
    if (this->is_declaration()) {
        for (int i = 0; i < this->get_num_of_args(); i++) {
            if (i)
                func_ir += ", ";
            func_ir += dynamic_cast<FunctionType *>(this->get_type())->get_param_type(i)->print();
        }
        if (dynamic_cast<FunctionType *>(this->get_type())->is_variable_args) {
            func_ir += ", ...";
        }
    } else {
        for (auto arg = this->arg_begin(); arg != arg_end(); arg++) {
            if (arg != this->arg_begin()) {
                func_ir += ", ";
            }
            func_ir += static_cast<Argument *>(*arg)->print();
        }
    }
    func_ir += ")";

    /** print basic block */
    if (!this->is_declaration()) {
        func_ir += " {\n";
        for (auto bb : this->get_basic_blocks()) {
            func_ir += bb->print();
        }
        if (this->is_ctor) {
            func_ir += "\nunreachable\n";
        }
        func_ir += "}";
    }

    return func_ir;
}
string Function::print_args() {
    string arg_ir;
    for (auto arg : arguments_) {
        if (arg != arguments_.back()) {
            arg_ir += arg->get_type()->print();
            arg_ir += ", ";
        } else {
            arg_ir += arg->get_type()->print();
        }
    }
    return arg_ir;
}
string Function::print_method(Class *method_) {
    string func_ir;
    if (method_->get_name() == "object")
        return "";
    func_ir += "define ";

    func_ir += this->get_return_type()->print();
    func_ir += " " + print_as_op(this, false, method_->get_name()) + "(";

    /** print arg */
    if (this->is_declaration()) {
        for (int i = 0; i < this->get_num_of_args(); i++) {
            if (i)
                func_ir += ", ";
            func_ir += dynamic_cast<FunctionType *>(this->get_type())->get_param_type(i)->print();
        }
    } else {
        for (auto arg = this->arg_begin(); arg != arg_end(); arg++) {
            if (arg != this->arg_begin()) {
                func_ir += ", ";
            }
            func_ir += static_cast<Argument *>(*arg)->print();
        }
    }
    func_ir += ")";

    /** Print basic block */
    if (!this->is_declaration()) {
        func_ir += " {\n";
        for (auto bb : this->get_basic_blocks()) {
            func_ir += bb->print();
        }
        func_ir += "}";
        /** Then it's definitely the upper method call */
    } else {
        func_ir += " {\n";

        func_ir += "}";
    }

    return func_ir;
}

std::string Argument::print() {
    std::string arg_ir;
    arg_ir += this->get_type()->print();
    arg_ir += " %";
    arg_ir += this->get_name();
    return arg_ir;
}
} // namespace lightir