#include "Module.hpp"

namespace lightir {

Module::Module(string name) : module_name_(std::move(name)) {
    void_ty_ = new VoidType(this);
    int1_ty_ = new IntegerType(1, this);
    int32_ty_ = new IntegerType(32, this);
    str_ty_ = new StringType("", this);
    label_ty_ = new Type(Type::type::LABEL);

    instr_id2string_.insert({Instruction::Ret, "ret"});
    instr_id2string_.insert({Instruction::Br, "br"});
    /** Int Negate not supported in clang 14 */
    instr_id2string_.insert({Instruction::Neg, "sub i32 0,"});
    instr_id2string_.insert({Instruction::Add, "add"});
    instr_id2string_.insert({Instruction::Sub, "sub"});
    instr_id2string_.insert({Instruction::Mul, "mul"});
    instr_id2string_.insert({Instruction::Div, "sdiv"});
    instr_id2string_.insert({Instruction::And, "and"});
    instr_id2string_.insert({Instruction::Rem, "srem"});
    instr_id2string_.insert({Instruction::Or, "or"});
    instr_id2string_.insert({Instruction::Not, "not"});
    instr_id2string_.insert({Instruction::Shl, "shl"});
    instr_id2string_.insert({Instruction::AShr, "ashr"});
    instr_id2string_.insert({Instruction::LShr, "lshr"});

    instr_id2string_.insert({Instruction::Alloca, "alloca"});
    instr_id2string_.insert({Instruction::Load, "load"});
    instr_id2string_.insert({Instruction::Store, "store"});
    instr_id2string_.insert({Instruction::ICmp, "icmp"});
    instr_id2string_.insert({Instruction::PHI, "phi"});
    instr_id2string_.insert({Instruction::Call, "call"});
    instr_id2string_.insert({Instruction::GEP, "getelementptr"});
    instr_id2string_.insert({Instruction::ZExt, "zext"});
    instr_id2string_.insert({Instruction::Trunc, "trunc"});
    instr_id2string_.insert({Instruction::InElem, "insertelement"});
    instr_id2string_.insert({Instruction::ExElem, "extractelement"});
    instr_id2string_.insert({Instruction::BitCast, "bitcast"});
}

Module::~Module() {
    delete void_ty_;
    delete int1_ty_;
    delete int32_ty_;
    delete str_ty_;
}

Type *Module::get_void_type() { return void_ty_; }

IntegerType *Module::get_int1_type() { return int1_ty_; }

IntegerType *Module::get_int32_type() { return int32_ty_; }

StringType *Module::get_str_type() { return str_ty_; }

Type *Module::get_label_type() { return label_ty_; }

Type *Module::get_class_type(int id_) { return obj_ty_[id_]; }

ArrayType *Module::get_array_type(Type *contained, unsigned num_elements) {
    if (array_map_.find({contained, num_elements}) == array_map_.end()) {
        array_map_[{contained, num_elements}] = new ArrayType(contained, num_elements);
    }
    return array_map_[{contained, num_elements}];
}
ArrayType *Module::get_array_type(Type *contained) {
    if (array_map_.find({contained, -1}) == array_map_.end()) {
        array_map_[{contained, -1}] = new ArrayType(contained, -1);
    }
    return array_map_[{contained, -1}];
}

void Module::add_function(Function *f) { function_list_.push_back(f); }
list<Function *> Module::get_functions() { return function_list_; }
void Module::add_global_variable(GlobalVariable *g) { global_list_.push_back(g); }
list<GlobalVariable *> Module::get_global_variable() { return global_list_; }
void Module::add_class(Class *c) { class_list_.push_back(c); };
list<Class *> Module::get_class() { return class_list_; }
void Module::add_union(Union *u) { union_list_.push_back(u); }
list<Union *> Module::get_union() { return union_list_; };

void Module::set_print_name() {
    for (auto func : this->get_functions()) {
        func->set_instr_name();
    }
}
void Module::add_class_type(Type *ty_) { obj_ty_[ty_->get_type_id()] = ty_; }

string Module::print() {
    string module_ir;
    this->is_declaration_ = true;
    for (auto &&union_ : this->get_union()) {
        module_ir += fmt::format("{}\n", union_->print());
    }
    this->is_declaration_ = false;
    for (auto &&class_ : this->get_class()) {
        module_ir += fmt::format("{}\n", class_->print_class());
    }
    auto counter = 0;
    for (auto global_val : this->get_global_variable()) {
        if (global_val->init_val_ != nullptr && dynamic_cast<ConstantStr *>(global_val->init_val_)) {
            if (counter == dynamic_cast<ConstantStr *>(global_val->init_val_)->get_id()) {
                continue;
            }
            counter = dynamic_cast<ConstantStr *>(global_val->init_val_)->get_id();
        }
        module_ir += fmt::format("{}\n", global_val->print());
    }
    int count = 0;
    for (auto func : this->function_list_) {
        count++;
        auto this_ptr = this->function_list_.begin();
        std::advance(this_ptr, count);
        auto idx = std::find_if(this_ptr, this->function_list_.end(), [&func](Function *func_) {
            return func_->get_name() == func->get_name() && func->is_declaration();
        });
        if (idx == this->function_list_.end()) {
            module_ir += fmt::format("{}\n", func->print());
        }
    }
    return module_ir;
}
ArrayType *Module::get_array_type() { return get_array_type(int32_ty_); }
} // namespace lightir