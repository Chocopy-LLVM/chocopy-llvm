#include "Constant.hpp"
#include "Module.hpp"
#include <chocopy_parse.hpp>
#include <iostream>

using namespace std;
namespace lightir {

ConstantInt *ConstantInt::get(int val, Module *m) { return new ConstantInt(Type::get_int32_type(m), val); }

ConstantInt *ConstantInt::get(bool val, Module *m) { return new ConstantInt(Type::get_int1_type(m), val ? 1 : 0); }

string ConstantInt::print() {
    string const_ir;
    const_ir += fmt::format("{} ", this->get_type()->print());
    Type *ty = this->get_type();
    if (ty->is_integer_type() && dynamic_cast<IntegerType *>(ty)->get_num_bits() == 1) {
        /** int 1 */
        const_ir += (this->get_value() == 0) ? "false" : "true";
    } else {
        /** int 32 */
        const_ir += fmt::format("{}", this->get_value());
    }
    return const_ir;
}

ConstantArray::ConstantArray(ArrayType *ty, const vector<Constant *> &val) : Constant(ty, "", val.size()) {
    for (int i = 0; i < val.size(); i++) {
        set_operand(i, val[i]);
    }
    this->const_array.assign(val.begin(), val.end());
}

Constant *ConstantArray::get_element_value(int index) { return this->const_array.at(index); }

void ConstantArray::set_const_array(const vector<Constant *> &new_array) {
    const_array = new_array;
    this->type_ = new ArrayType(dynamic_cast<ArrayType *>(this->type_)->get_element_type(), new_array.size());
};

string ConstantArray::print() {
    string const_ir;
    const_ir += this->get_type()->print();
    if (this->get_size_of_array() == 0)
        const_ir += " undef";
    else {
        const_ir += " [";
        for (int i = 0; i < this->get_size_of_array(); i++) {
            const_ir += get_element_value(i)->print();
            if (i != this->get_size_of_array() - 1)
                const_ir += ",";
            const_ir += " ";
        }
        const_ir += " ]";
    }
    return const_ir;
}

ConstantZero *ConstantZero::get(Type *ty, Module *m) { return new ConstantZero(ty); }

std::string ConstantZero::print() { return "zeroinitializer"; }
ConstantStr *ConstantStr::get(const string &val, int id, Module *m) {
    return new ConstantStr(new StringType(val, m), val, id);
}
string ConstantStr::print() {
    string const_ir;
    if (header_print_) {
        const_ir += "@const_" + std::to_string(id_) + " = global %$str$prototype_type {\n" +
                    fmt::format("  i32 {},\n  i32 {},\n  %$str$dispatchTable_type* @$str$dispatchTable,\n  i32 {},\n  "
                                "i8* getelementptr inbounds ([{} x i8], [{} x i8]* @str.const_{}, i32 0, i32 0) ",
                                this->get_type()->get_type_id(), int(value_.size() / 4) + 5, value_.size(),
                                value_.size() + 1, value_.size() + 1, id_) +
                    "\n}";
        const_ir += fmt::format("\n@str.const_{} = private unnamed_addr global [{} x i8] c\"{}\\00\", align 1\n", id_,
                                value_.size() + 1, value_);
        header_print_ = false;
    } else {
        const_ir += fmt::format("@const_{}", id_);
    }
    return const_ir;
}
} // namespace lightir