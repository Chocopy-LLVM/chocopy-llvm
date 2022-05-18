//
// Created by yiwei yang on 2021/8/7.
//
#include "Class.hpp"
#include <utility>

namespace lightir {
void Class::add_method(Function *func) {
    auto func_exist = [&func](Function *func_) { return func_->get_name() == func->get_name(); };
    auto idx = std::find_if(this->methods_->begin(), this->methods_->end(), func_exist);
    if (idx != this->methods_->end()) {
        (*this->methods_)[std::distance(this->methods_->begin(), idx)] = func;
        return;
    }
    this->methods_->emplace_back(func);
}

Class::Class(Module *m, const string &name_, int type_tag, Class *super_class_info, bool with_dispatch_table_,
             bool is_dispatch_table_, bool is_append)
    : Type(static_cast<type>(type_tag), m), Value(this, name_), class_name_(name_), super_class_info_(super_class_info),
      type_tag_(type_tag), print_dispatch_table_(is_dispatch_table_) {
    prototype_label_ = fmt::format("${}${}", name_, "prototype");
    if (with_dispatch_table_) {
        dispatch_table_label_ = fmt::format("${}${}", name_, "dispatchTable");
    }
    attributes_ = new vector<AttrInfo *>();
    methods_ = new vector<Function *>();
#if defined(WIN32) || defined(_WIN32)
    if (std::ranges::none_of(m->get_class(), [&](const Class *c) { return c->type_tag_ == type_tag; })) {
        m->add_class(this);
    }
#else
    bool flag = false;
    for (auto &&c : m->get_class())
        if (c->type_tag_ == type_tag)
            flag = true;
    if (!flag && is_append)
        m->add_class(this);
#endif
}

Class::Class(Module *m, const string &name_, bool anon_)
    : Type(static_cast<type>(-7), m), Value(this, name_), class_name_(name_), super_class_info_(nullptr), anon_(anon_) {
    attributes_ = new vector<AttrInfo *>();
    methods_ = new vector<Function *>();
    m->add_class(this);
}

/** First print out the type name */
string Class::print_class() {
    /** Test case for the print
        class A(object):
            a:int = 42

            def foo(self:"A", ignore:object) -> int:
                return self.a

        class B(A):
            b:bool = True

            def __init__(self:"B"):
                print("B")

            def bar(self:"B") -> int:
                a:A = None
                return a.foo(self.b)
     */

    string const_ir;

    if (anon_) {
        /** %class.anon = type { i32* } */
        const_ir += fmt::format("%$class.anon_{} = type", class_name_) + " {";
        for (auto &&attr : *this->attributes_) {
            const_ir += fmt::format("{} ", attr->print());
            if (attr != this->attributes_->at(this->attributes_->size() - 1)) {
                const_ir += ",\n  ";
            } else {
                const_ir += "\n";
            }
        }
        const_ir += "}\n";

        return const_ir;
    }

    /**
     * From here, we print out the prototype of the class.
     * In Cgen:
     * .globl $A$prototype
     * $A$prototype:
     *   .word 4                                  # Type tag for class: A
     *   .word 4                                  # Object size
     *   .word $A$dispatchTable                   # Pointer to dispatch table
     *   .word 42                                 # Initial value of attribute: a
     *   .align 2
     * .globl $B$prototype
     * $B$prototype:
     *   .word 5                                  # Type tag for class: B
     *   .word 5                                  # Object size
     *   .word $B$dispatchTable                   # Pointer to dispatch table
     *   .word 42                                 # Initial value of attribute: a
     *   .word 1                                  # Initial value of attribute: b
     *   .align 2
     * In LLVM:
     * %$A$prototype_type = type { i32, i32, i32(%$A$dispatchTable_type*)*, i32 }
     * @$A$prototype = global %$A$prototype_type {
     *    i32 4,
     *        i32 4,
     *        i32(%A*)* @$A$dispatchTable,
     *        i32 4
     *    }
     */

    const_ir += "%" + this->prototype_label_ + "_type  = type  {\n  ";
    const_ir += fmt::format("i32,\n  ");
    const_ir += fmt::format("i32,\n  ");

    if (!dispatch_table_label_.empty()) {
        if (this->attributes_->empty()) {
            const_ir += fmt::format("%{}_type*\n", this->dispatch_table_label_);
        } else {
            const_ir += fmt::format("%{}_type*,\n  ", this->dispatch_table_label_);
        }
    }

    for (auto &&attr : *this->attributes_) {
        const_ir += fmt::format("{} ", attr->print());
        if (attr != this->attributes_->at(this->attributes_->size() - 1)) {
            const_ir += ",\n  ";
        } else {
            const_ir += "\n";
        }
    }
    const_ir += "}\n";

    const_ir += "@" + this->prototype_label_ + "  = global %" + this->prototype_label_ + "_type{\n  ";
    const_ir += fmt::format("i32 {},\n  ", this->type_tag_);
    if (!dispatch_table_label_.empty()) {
        const_ir += fmt::format("i32 {},\n  ", 3 + this->attributes_->size());
        if (this->attributes_->empty()) {
            const_ir += fmt::format("%{}_type* @{}\n", this->dispatch_table_label_, this->dispatch_table_label_);
        } else {
            const_ir += fmt::format("%{}_type* @{},\n  ", this->dispatch_table_label_, this->dispatch_table_label_);
        }
    } else
        const_ir += fmt::format("i32 {},\n  ", 2 + this->attributes_->size());

    for (auto &&attr : *this->attributes_) {
        if (attr->init_obj == nullptr && (attr->get_type()->is_integer_type() || attr->get_type()->is_bool_type())) {
            const_ir += fmt::format("{} {}", attr->print(), attr->init_val);
        } else if (attr->init_obj == nullptr) {
            const_ir +=
                fmt::format("{} {}", attr->print(), fmt::format("inttoptr (i32 0 to {})", attr->get_type()->print()));
        } else if (dynamic_cast<ConstantStr *>(attr->init_obj)) {
            const_ir += fmt::format("{} {}", attr->print(), attr->init_obj->get_name());
        } else if (dynamic_cast<Union *>(attr->init_obj)) {
            const_ir += attr->print() + " {";
            const_ir += fmt::format("i{}", ((Union *)attr->init_obj)->length_);
            const_ir += " 0 }";
        } else {
            const_ir += attr->print() + " {" + attr->init_obj->get_type()->print() + " undef}";
        }

        if (attr != this->attributes_->at(this->attributes_->size() - 1)) {
            const_ir += ",\n  ";
        } else {
            const_ir += "\n";
        }
    }

    const_ir += "}\n";

    /**
     * From here, we print out the dispatch table of the class.
     * In Cgen:
     *     .globl $A$dispatchTable
     *     $A$dispatchTable:
     *       .word $object.__init__                   # Implementation for method: A.__init__
     *       .word $A.foo                             # Implementation for method: A.foo
     *     .globl $B$dispatchTable
     *     $B$dispatchTable:
     *       .word $B.__init__                        # Implementation for method: B.__init__
     *       .word $A.foo                             # Implementation for method: B.foo
     *       .word $B.bar                             # Implementation for method: B.bar
     * In LLVM:
     *  %$A$dispatchTable_type = type {
     *     object(%A*)*
     *     i32(%A*)*
     *  }
     *  @$A$dispatchTable = global %$A$dispatchTable_type {
     *     $A$prototype (%A*)* @object.__init__
     *     i32(%A*)* @A.foo
     *  }
     *  %$B$dispatchTable_type = type {
     *     object(%B*)*
     *     i32(%B*)*
     *  }
     *  @$B$dispatchTable = global %$B$dispatchTable_type {
     *      $B$prototype (%B*)* @B.__init__
     *      i32(%B*)* @A.foo
     *      i32(%B*)* @B.bar
     *  }
     */
    if (!this->dispatch_table_label_.empty()) {
        const_ir += "%" + this->dispatch_table_label_ + "_type = type {\n  ";
        for (auto &&method : *this->methods_) {
            const_ir += fmt::format("{}({})*", method->get_return_type()->print(), method->print_args());
            if (method != this->methods_->at(this->methods_->size() - 1)) {
                const_ir += ",\n  ";
            } else {
                const_ir += "\n";
            }
        }
        const_ir += "}\n";

        const_ir += "@" + this->dispatch_table_label_ + " = global %" + this->dispatch_table_label_ + "_type {\n  ";

        for (auto &&method : *this->methods_) {
            // FIXME: Use dispatch_table_vec to print.
            const_ir += fmt::format("{}({})* @{}", method->get_return_type()->print(), method->print_args(),
                                    method->get_name());
            if (method != this->methods_->at(this->methods_->size() - 1)) {
                const_ir += ",\n  ";
            } else {
                const_ir += "\n";
            }
        }
        const_ir += "}\n";
    }
    return const_ir;
}

int Class::get_method_offset(string method) const {
    return std::distance(methods_->begin(), std::find_if(methods_->begin(), methods_->end(), [&method](Function *tmp) {
                             return tmp->get_name() == method;
                         }));
}

Type *Class::get_offset_method(int idx_) { return this->methods_->at(idx_)->get_type(); }

string AttrInfo::print() {
    string const_ir;
    if (dynamic_cast<Union *>(this->init_obj))
        const_ir += this->init_obj->print();
    else
        const_ir += this->get_type()->print();
    return const_ir;
}

List::List(Class *list_class, vector<Value *> contained, const string &name)
    : Class(list_class->get_module(), name, type::LIST, list_class->super_class_info_, true, false),
      contained_(std::move(contained)) {}

List *List::get(Class *list_class, vector<Value *> contained, const string &name) {
    return new List(list_class, std::move(contained), name);
}

string List::print() {
    string list_ir;
    list_ir += fmt::format("{} = global %$.list$prototype_type ", this->name_) + "{\n";
    list_ir += "  i32 -1,\n  i32 5,\n  %$unionl.type {";
    /** One prototype, one value */
    if (this->contained_.at(0) == nullptr) {
    }
    return list_ir;
}
} // namespace lightir