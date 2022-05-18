#include "Type.hpp"
#include "Module.hpp"
#include <chocopy_logging.hpp>
#include <utility>

namespace lightir {

Type::Type(type tid, Module *m) : tid_(tid) { m_ = m; }

Module *Type::get_module() { return m_; }

Type *Type::get_void_type(Module *m) { return m->get_void_type(); }

Type *Type::get_label_type(Module *m) { return m->get_label_type(); }

Type *Type::get_class_type(Module *m, int id_) { return m->get_class_type(id_); }

IntegerType *Type::get_int1_type(Module *m) { return m->get_int1_type(); }

IntegerType *Type::get_int32_type(Module *m) { return m->get_int32_type(); }

StringType *Type::get_str_type(Module *m) { return m->get_str_type(); }

ArrayType *Type::get_array_type(Module *m) { return m->get_array_type(); }

ArrayType *Type::get_array_type(Type *contained) { return ArrayType::get(contained, -1); }

Type *Type::get_array_element_type() {
    if (this->is_array_type())
        return dynamic_cast<ArrayType *>(this)->get_element_type();
    else
        return nullptr;
}

Type *Type::get_ptr_element_type() {
    if (!dynamic_cast<ArrayType *>(this) && !dynamic_cast<StringType *>(this))
        return this;
    else if (dynamic_cast<StringType *>(this))
        return ArrayType::get(new IntegerType(8, m_));
    else if (this->is_ptr_type())
        return dynamic_cast<ArrayType *>(this)->get_element_type();
    else
        return this;
}

int Type::get_size() {
    if (this->is_integer_type()) {
        auto bits = dynamic_cast<IntegerType *>(this)->get_num_bits();
        return bits > 0 ? bits : 1;
    }
    if (this->is_array_type()) {
        auto element_size = dynamic_cast<ArrayType *>(this)->get_element_type()->get_size();
        auto num_elements = dynamic_cast<ArrayType *>(this)->get_num_of_elements();
        return element_size * num_elements;
    }
    if (this->is_ptr_type() || this->is_string_type())
        return 32;

    return 0;
}

constexpr bool Type::is_array_type() {
    if (this->tid_ == type::LIST && !dynamic_cast<Class *>(this))
        return dynamic_cast<ArrayType *>(this)->get_num_of_elements() != -1;
    else
        return false;
}
constexpr bool Type::is_ptr_type() {
    if (!dynamic_cast<ArrayType *>(this))
        return true;
    else if (this->tid_ == type::LIST)
        return dynamic_cast<ArrayType *>(this)->get_num_of_elements() == -1;
    else
        return false;
}
#if __cplusplus > 202000L && !defined(__clang__)
std::strong_ordering Type::operator<=>(Type rhs) {
    if (this->is_ptr_type()) {
        return this->get_ptr_element_type() <=> rhs.get_ptr_element_type();
    } else {
        return this->get_type_id() <=> rhs.get_type_id();
    }
}
#else
bool Type::operator==(Type rhs) {
    if (this->is_ptr_type()) {
        return this->get_ptr_element_type() == rhs.get_ptr_element_type();
    } else {
        return this->get_type_id() == rhs.get_type_id();
    }
}
bool Type::operator<=(Type rhs) {
    if (this->is_ptr_type()) {
        return this->get_ptr_element_type() <= rhs.get_ptr_element_type();
    } else {
        return this->get_type_id() <= rhs.get_type_id();
    }
}
bool Type::operator>=(Type rhs) {
    if (this->is_ptr_type()) {
        return this->get_ptr_element_type() >= rhs.get_ptr_element_type();
    } else {
        return this->get_type_id() >= rhs.get_type_id();
    }
}
bool Type::operator>(Type rhs) {
    if (this->is_ptr_type()) {
        return this->get_ptr_element_type() > rhs.get_ptr_element_type();
    } else {
        return this->get_type_id() > rhs.get_type_id();
    }
}
bool Type::operator<(Type rhs) {
    if (this->is_ptr_type()) {
        return this->get_ptr_element_type() < rhs.get_ptr_element_type();
    } else {
        return this->get_type_id() < rhs.get_type_id();
    }
}
#endif
string Type::print() { return {}; }

IntegerType::IntegerType(unsigned num_bits, Module *m)
    : Type(num_bits == 1 ? Type::type::BOOL : Type::type::INT, m), num_bits_(num_bits) {}

IntegerType *IntegerType::get(unsigned num_bits, Module *m) { return new IntegerType(num_bits, m); }

unsigned IntegerType::get_num_bits() const { return num_bits_; }

FunctionType::FunctionType(Type *result, const std::vector<Type *> &params)
    : Type(Type::type::FUNC, result->get_module()) {
    result_ = result;
    for (auto p : params) {
        args_.push_back(p);
    }
}

FunctionType *FunctionType::get(Type *result, const std::vector<Type *> &params) {
    return new FunctionType(result, params);
}

FunctionType *FunctionType::get(Type *result, const std::vector<Type *> &params, bool is_variable_args) {
    auto tmp_func = new FunctionType(result, params);
    tmp_func->is_variable_args = is_variable_args;
    return tmp_func;
}

unsigned FunctionType::get_num_of_args() const { return args_.size(); }

Type *FunctionType::get_param_type(unsigned i) const { return args_[i]; }

Type *FunctionType::get_return_type() const { return result_; }

Type *FunctionType::get_arg_type(unsigned int i) const { return args_[i]; }

string FunctionType::print() {
    string type_ir;
    type_ir += dynamic_cast<FunctionType *>(this)->get_return_type()->print();
    type_ir += " (";
    for (int i = 0; i < dynamic_cast<FunctionType *>(this)->get_num_of_args(); i++) {
        if (i)
            type_ir += ", ";
        type_ir += dynamic_cast<FunctionType *>(this)->get_param_type(i)->print();
    }
    if (is_variable_args) {
        type_ir += ", ...";
    }
    type_ir += ")";
    return type_ir;
}

ArrayType::ArrayType(Type *contained, unsigned num_elements)
    : Type(Type::type::LIST, contained->get_module()), num_elements_(num_elements) {
    contained_ = contained;
}
ArrayType::ArrayType(Type *contained) : Type(Type::type::LIST, contained->get_module()), num_elements_(-1) {
    contained_ = contained;
}

ArrayType *ArrayType::get(Type *contained, unsigned num_elements) {
    return contained->get_module()->get_array_type(contained, num_elements);
}

ArrayType *ArrayType::get(Type *contained) {
    auto res = contained->get_module()->get_array_type(contained);
    res->num_elements_ = -1;
    return res;
}

string ArrayType::print() {
    string type_ir;
    if (dynamic_cast<ArrayType *>(this)->get_num_of_elements() != -1)
        type_ir += fmt::format("[{} x {}]", std::to_string(dynamic_cast<ArrayType *>(this)->get_num_of_elements()),
                               dynamic_cast<ArrayType *>(this)->get_element_type()->print());
    else if (dynamic_cast<StringType *>(dynamic_cast<ArrayType *>(this)->get_element_type()))
        type_ir += "i8*";
    else
        type_ir += dynamic_cast<ArrayType *>(this)->get_element_type()->print() + "*";
    return type_ir;
}

Union::Union(vector<Type *> contained, string name)
    : Type(Type::type::UNION, contained.at(0)->get_module()), Value(this), contained_(contained),
      name_(std::move(name)) {
    for (auto &&type_ : contained) {
        this->length_ = std::max(this->length_, this->get_size());
    }
}

Union *Union::get(vector<Type *> contained, string name) { return new Union(std::move(contained), std::move(name)); }

Union *Union::get(Module *m_, string name) { return new Union({m_->get_void_type()}, std::move(name)); }

string Union::print() {
    string type_ir;
    /** should get the context that it's not array type check. */
    if (this->get_module()->is_declaration_) {
        /** First to print declaration */
        type_ir += "%$union." + this->name_ + " = type { ";
        type_ir += fmt::format("i{}", this->length_);
        type_ir += " }\n";
    } else {
        /** Second to print type */
        type_ir += "%$union." + this->name_;
    }
    return type_ir;
}

VectorType::VectorType(Value *contained, unsigned num_elements)
    : Type(Type::type::VECTOR, contained->get_type()->get_module()), contained_(contained),
      num_elements_(num_elements) {}

VectorType::VectorType(Value *contained)
    : Type(Type::type::VECTOR, contained->get_type()->get_module()), contained_(contained), num_elements_(0) {}

VectorType *VectorType::get(Value *contained, unsigned num_elements) { return new VectorType(contained, num_elements); }

VectorType *VectorType::get(ConstantArray *contained) { return new VectorType(contained); }

string VectorType::print() {
    if (dynamic_cast<ConstantArray *>(this->contained_))
        return fmt::format(
            "<{} x {}>", std::to_string(dynamic_cast<ArrayType *>(this->contained_->get_type())->get_num_of_elements()),
            dynamic_cast<ArrayType *>(this->contained_->get_type())->get_element_type()->print());
    else
        return fmt::format("<{} x {}>", std::to_string(this->num_elements_), this->contained_->get_type()->print());
}

string VectorType::print_as_op() {
    return fmt::format("<{} x {}>* bitcast ({} to <{} x {}>*)",
                       std::to_string(dynamic_cast<ArrayType *>(this->contained_->get_type())->get_num_of_elements()),
                       dynamic_cast<ArrayType *>(this->contained_->get_type())->get_element_type()->print(),
                       dynamic_cast<ArrayType *>(this->contained_->get_type())->print(),
                       std::to_string(dynamic_cast<ArrayType *>(this->contained_->get_type())->get_num_of_elements()),
                       dynamic_cast<ArrayType *>(this->contained_->get_type())->get_element_type()->print());
}

StringType *StringType::get(string str_, Module *m) { return new StringType(std::move(str_), m); }
StringType::StringType(string str_, Module *m) : Type(Type::type::STRING, m), str_(std::move(str_)) {}
string StringType::get_string() const { return str_; }

LabelType *LabelType::get(string str_, Class *stored_, Module *m) { return new LabelType(std::move(str_), stored_, m); }
LabelType::LabelType(string label_, Class *stored_, Module *m)
    : Type(Type::type::LABEL, m), label_(std::move(label_)),stored_(stored_) {}
string LabelType::get_label() const { return label_; }

} // namespace lightir