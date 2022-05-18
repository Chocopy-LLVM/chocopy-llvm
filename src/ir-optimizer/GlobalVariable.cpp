#include "GlobalVariable.hpp"

#include "IRprinter.hpp"
#include <utility>
namespace lightir {

GlobalVariable::GlobalVariable(const string &name, Module *m, Type *ty, bool is_const, Constant *init)
    : User(ty, name, init != nullptr), init_val_(init), is_const_(is_const) {
    m->add_global_variable(this);
    if (init) {
        this->set_operand(0, init);
    }
}

GlobalVariable *GlobalVariable::create(const string &name, Module *m, Type *ty, bool is_const,
                                       Constant *init = nullptr) {
    return new GlobalVariable(name, m, ty, is_const, init);
}
GlobalVariable *GlobalVariable::create(const string &name, Module *m, ConstantStr *init) {
    return new GlobalVariable(name, m, init->get_type(), true, init);
}

void GlobalVariable::set_list(const vector<Constant *> &new_array) const {
    ((ConstantArray *)this->init_val_)->set_const_array(new_array);
    ((ArrayType *)((Value *)this)->type_)->set_num_of_elements(new_array.size());
}

string GlobalVariable::print() {
    string global_ir;
    if (init_val_ == nullptr) {
        global_ir += fmt::format("@{} = external global {}", this->name_, this->type_->print());
        return global_ir;
    }
    if (dynamic_cast<ConstantStr *>(this->init_val_)) {
        global_ir += this->init_val_->print();
        return global_ir;
    }
    if (!is_print_head_) {
        global_ir = fmt::format("{} = {}{}", print_as_op(this, false), (this->is_const_ ? "constant " : "global "),
                                this->get_init()->print());
        is_print_head_ = true;
    } else {
        if (this->init_val_->get_type()->is_string_type())
            global_ir = fmt::format("$str$prototype_type*{}", print_as_op(this, false));
        else
            global_ir = fmt::format("{}*{}", this->get_type()->print(), print_as_op(this, false));
    }
    return global_ir;
}
} // namespace lightir