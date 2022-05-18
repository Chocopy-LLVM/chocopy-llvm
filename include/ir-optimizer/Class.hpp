//
// Created by yiwei yang on 2021/7/31.
//

#ifndef CHOCOPY_COMPILER_CLASS_HPP
#define CHOCOPY_COMPILER_CLASS_HPP

#include "BasicBlock.hpp"
#include "Constant.hpp"
#include "Function.hpp"
#include "Module.hpp"
#include "Type.hpp"
#include "User.hpp"
#include <fmt/format.h>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

using std::map;
using std::string;
using std::vector;

namespace lightir {
class Function;
class Constant;
/** Information for code generation a class. */
class AttrInfo : public Value {
public:
    AttrInfo(Type *varType, string name_, int init_val_) : Value(varType, std::move(name_)), init_val(init_val_){};
    AttrInfo(Type *varType, string name_, Value *init_val_) : Value(varType, std::move(name_)), init_obj(init_val_){};
    AttrInfo(Type *varType, string name_) : Value(varType, std::move(name_)){};
    string print() override;
    int init_val = 0;
    Value *init_obj = nullptr;
};
/** Class info, derived from both @Type and @Value,
 *  Tag indicating type of value:
 *     -2: void
 *     -1: [T] T can be class, int, bool, str, list.
 *      0: object
 *      1: int
 *      2: bool
 *      3: str
 *     >3: User-defined types.
 */
class Class : public Type, public Value {
public:
    vector<AttrInfo *> *attributes_ = new vector<AttrInfo *>();
    vector<Function *> *methods_ = new vector<Function *>();

    int type_tag_{};
    string prototype_label_;
    string dispatch_table_label_;
    bool anon_ = false;

    Class(Module *m, const string &name_, int type_tag, Class *super_class_info, bool with_dispatch_table_ = true,
          bool print_dispatch_table_ = false,bool is_append=true);
    Class(Module *m, const string &name_, bool anon_);

    void add_attribute(AttrInfo *attrInfo) const { this->attributes_->emplace_back(attrInfo); }
    void add_method(Function *func);


    int get_size() override {
        auto get_size = [&]() {
            int x = 0;
            for (const auto &v : *this->attributes_)
                x += v->get_type()->get_size();
            return x;
        };
        return get_size();
    }

    string get_string() { return class_name_; }

    vector<Function *> *get_method() const { return methods_; }
    vector<AttrInfo *> *get_attribute() const { return attributes_; }
    Class *get_current_type() const { return class_; }
    void set_current_type(Class *claz_) { this->class_ = claz_; }
    Type *get_anon_type(int idx_) {
        return this->attributes_->at(idx_)->init_obj->get_type();
    }

    Type *get_offset_attr(int idx_){
        return this->attributes_->at(idx_)->get_type();
    }
    Type* get_offset_method(int idx_);

    int get_attr_offset(string_view attribute) const {
        return std::distance(attributes_->begin(),
                             std::find_if(attributes_->begin(), attributes_->end(),
                                          [&attribute](AttrInfo *tmp) { return tmp->get_name() == attribute; }));
    }
    int get_method_offset(string method) const;
    // FIXME:return prototype if it's passed to alloc
    virtual string print() override {
        return anon_ ? fmt::format("%$class.anon_{}", this->get_name())
                     : (print_dispatch_table_ ? fmt::format("%{}_type", this->dispatch_table_label_)
                                              : fmt::format("%{}_type", this->prototype_label_));
    }

    string print_class();
    Class *super_class_info_;
    bool print_dispatch_table_{};

private:
    string class_name_;
    Class *class_{};
};

/** For use of duck typing in function passing and list dispatch table,
 *  connected with @Bitcast */
class List : public Class {
public:
    List(Class *list_class, vector<Value *> contained, const string &name);
    static List *get(Class *list_class, vector<Value *> contained, const string &name);

    Value *get_list_value(int idx) const { return contained_[idx]; }
    void set_list(vector<Value *> cont) { contained_ = cont; }
    string print() override;
    string get_name() override { return this->name_; };

private:
    vector<Value *> contained_; // The element types of the value.
    string name_; // list name
    Type::type inside_tag_;
};
} // namespace lightir
#endif // CHOCOPY_COMPILER_CLASS_HPP
