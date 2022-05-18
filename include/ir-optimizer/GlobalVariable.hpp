//
// Created by yiwei yang on 2/15/21.
//

#ifndef CHOCOPY_COMPILER_GLOBALVARIABLE_HPP
#define CHOCOPY_COMPILER_GLOBALVARIABLE_HPP

#include "Constant.hpp"
#include "Module.hpp"
#include "User.hpp"

using std::map;
using std::string;
using std::vector;

namespace lightir {
/** Code-generation related information about a global variable. */
class GlobalVariable : public User {
public:
    bool is_const_ = true;
    bool is_print_head_ = false;
    bool is_init = false;

    Constant *init_val_;
    static GlobalVariable *create(const string &name, Module *m, Type *ty, bool is_const, Constant *init);
    static GlobalVariable *create(const string &name, Module *m, ConstantStr *init);
    GlobalVariable(const string &name, Module *m, Type *ty, bool is_const, Constant *init);

    Constant *get_init() const { return init_val_; }
    bool is_list_init() const { return !init_val_ ? true : ((ConstantArray *)init_val_)->get_size_of_array() == 0; }
    void set_list(const vector<Constant *> &new_array) const;
    string print() override;
};
} // namespace lightir
#endif // CHOCOPY_COMPILER_GLOBALVARIABLE_HPP
