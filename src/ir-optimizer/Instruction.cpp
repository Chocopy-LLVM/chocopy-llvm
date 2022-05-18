#include "BasicBlock.hpp"
#include "Function.hpp"
#include "IRprinter.hpp"
#include "Module.hpp"
#include "Type.hpp"
#include <cassert>
#include <iterator>
#include <regex>
#include <utility>
#include <vector>

namespace lightir {
const std::regex to_load_replace("\\*$");

Instruction::Instruction(Type *ty, OpID id, unsigned num_ops, BasicBlock *parent)
    : User(ty, "", num_ops), parent_(parent), op_id_(id), num_ops_(num_ops) {
    parent_->add_instruction(this);
}

Instruction::Instruction(Type *ty, OpID id, unsigned num_ops)
    : User(ty, "", num_ops), parent_(nullptr), op_id_(id), num_ops_(num_ops) {}

const Function *Instruction::get_function() const { return parent_->get_parent(); }

Module *Instruction::get_module() { return parent_->get_module(); }

BinaryInst::BinaryInst(Type *ty, OpID id, Value *v1, Value *v2, BasicBlock *bb) : Instruction(ty, id, 2, bb) {
    set_operand(0, v1);
    set_operand(1, v2);
}

void BinaryInst::assert_valid() {
    assert(get_operand(0)->get_type()->is_integer_type());
    assert(get_operand(1)->get_type()->is_integer_type());
    assert(dynamic_cast<IntegerType *>(get_operand(0)->get_type())->get_num_bits() ==
           dynamic_cast<IntegerType *>(get_operand(1)->get_type())->get_num_bits());
}

BinaryInst *BinaryInst::create_add(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
    return new BinaryInst(Type::get_int32_type(m), Instruction::Add, v1, v2, bb);
}

BinaryInst *BinaryInst::create_sub(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
    return new BinaryInst(Type::get_int32_type(m), Instruction::Sub, v1, v2, bb);
}

BinaryInst *BinaryInst::create_mul(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
    return new BinaryInst(Type::get_int32_type(m), Instruction::Mul, v1, v2, bb);
}

BinaryInst *BinaryInst::create_sdiv(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
    return new BinaryInst(Type::get_int32_type(m), Instruction::Div, v1, v2, bb);
}

BinaryInst *BinaryInst::create_rem(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
    return new BinaryInst(Type::get_int32_type(m), Instruction::Rem, v1, v2, bb);
}

BinaryInst *BinaryInst::create_and(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
    return new BinaryInst(Type::get_int32_type(m), Instruction::And, v1, v2, bb);
}

BinaryInst *BinaryInst::create_or(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
    return new BinaryInst(Type::get_int32_type(m), Instruction::Or, v1, v2, bb);
}

UnaryInst *UnaryInst::create_not(Value *v1, BasicBlock *bb, Module *m) {
    return new UnaryInst(Type::get_int32_type(m), Instruction::Not, v1, bb);
}

UnaryInst *UnaryInst::create_neg(Value *v1, BasicBlock *bb, Module *m) {
    return new UnaryInst(Type::get_int32_type(m), Instruction::Neg, v1, bb);
}

UnaryInst::UnaryInst(Type *ty, OpID id, Value *v1, BasicBlock *bb) : Instruction(ty, id, 1, bb) { set_operand(0, v1); }

string UnaryInst::print() {
    string instr_ir;
    instr_ir += "%";
    instr_ir += this->get_name();
    if (this->get_module()->get_instr_op_name(this->get_instr_type()) != "not") {
        instr_ir += " = sub i32 0, ";
    } else {
        instr_ir += " = sub i32 1, ";
    }
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), false);
    return instr_ir;
}

string BinaryInst::print() {
    string instr_ir;
    instr_ir += "%";
    instr_ir += this->get_name();
    instr_ir += " = ";
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
    instr_ir += " ";
    instr_ir += this->get_operand(0)->get_type()->print();
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), false);
    instr_ir += ", ";
    instr_ir += print_as_op(this->get_operand(1), false);
    return instr_ir;
}

CmpInst::CmpInst(Type *ty, CmpOp op, Value *lhs, Value *rhs, BasicBlock *bb)
    : Instruction(ty, Instruction::ICmp, 2, bb), cmp_op_(op) {
    set_operand(0, lhs);
    set_operand(1, rhs);
    assert_valid();
}

void CmpInst::assert_valid() {
    assert(get_operand(0)->get_type()->is_integer_type());
    assert(get_operand(1)->get_type()->is_integer_type());
    assert(dynamic_cast<IntegerType *>(get_operand(0)->get_type())->get_num_bits() ==
           dynamic_cast<IntegerType *>(get_operand(1)->get_type())->get_num_bits());
}

CmpInst *CmpInst::create_cmp(CmpOp op, Value *lhs, Value *rhs, BasicBlock *bb, Module *m) {
    return new CmpInst(m->get_int1_type(), op, lhs, rhs, bb);
}

string CmpInst::print() {
    string instr_ir;
    instr_ir += "%";
    instr_ir += this->get_name();
    instr_ir += " = ";
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
    instr_ir += " ";
    instr_ir += print_cmp_type(this->cmp_op_);
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), true);
    instr_ir += ", ";
    instr_ir += print_as_op(this->get_operand(1), false);
    return instr_ir;
}

CallInst::CallInst(Function *func, std::vector<Value *> args, BasicBlock *bb)
    : Instruction(func->get_return_type(), Instruction::Call, args.size() + 1, bb) {
    /** Runtime will support the check */
    int num_ops = args.size() + 1;
    set_operand(0, func);
    for (int i = 1; i < num_ops; i++) {
        set_operand(i, args[i - 1]);
    }
}
CallInst::CallInst(Value *real_func, FunctionType *func, std::vector<Value *> args, BasicBlock *bb)
    : Instruction(func->get_return_type(), Instruction::Call, args.size() + 1, bb) {
    /** Runtime will support the check */
    int num_ops = args.size() + 1;
    set_operand(0, real_func);
    for (int i = 1; i < num_ops; i++) {
        set_operand(i, args[i - 1]);
    }
}

CallInst *CallInst::create(Function *func, std::vector<Value *> args, BasicBlock *bb) {
    return new CallInst(func, std::move(args), bb);
}
CallInst *CallInst::create(Value *real_func, FunctionType *func, std::vector<Value *> args, BasicBlock *bb) {
    return new CallInst(real_func, func, std::move(args), bb);
}

FunctionType *CallInst::get_function_type() const { return dynamic_cast<FunctionType *>(get_operand(0)->get_type()); }

string CallInst::print() {
    string instr_ir;
    if (!this->is_void()) {
        instr_ir += "%";
        instr_ir += this->get_name();
        instr_ir += " = ";
    }
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
    instr_ir += " ";
    instr_ir += this->get_function_type()->get_return_type()->print();
    if (this->get_function_type()->is_variable_args) {
        instr_ir += " (";
        for (int i = 0; i < dynamic_cast<FunctionType *>(this->get_function_type())->get_num_of_args(); i++) {
            if (i)
                instr_ir += ", ";
            instr_ir += dynamic_cast<FunctionType *>(this->get_function_type())->get_param_type(i)->print();
        }
        instr_ir += ", ...)";
    }
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), false);
    instr_ir += "(";
    for (int i = 1; i < this->get_num_operand(); i++) {
        if (i > 1)
            instr_ir += ", ";
        if (print_as_op(this->get_operand(0), false) == "@initchars") {
            instr_ir += "i8 ";
        } else if (this->get_operand(i)->get_type()->is_string_type()) {
            instr_ir += "%$str$prototype_type* ";
        } else if (!dynamic_cast<Constant *>(this->get_operand(i)) ||
                   this->get_operand(i)->get_type()->is_integer_type() ||
                   this->get_operand(i)->get_type()->is_bool_type()) {
            instr_ir += this->get_operand(i)->get_type()->print();
            instr_ir += " ";
        }
        instr_ir += print_as_op(this->get_operand(i), false, "");
    }
    instr_ir += ")";
    return instr_ir;
}

BranchInst::BranchInst(Value *cond, BasicBlock *if_true, BasicBlock *if_false, BasicBlock *bb)
    : Instruction(Type::get_void_type(if_true->get_module()), Instruction::Br, 3, bb) {
    set_operand(0, cond);
    set_operand(1, if_true);
    set_operand(2, if_false);
}

BranchInst::BranchInst(BasicBlock *if_true, BasicBlock *bb)
    : Instruction(Type::get_void_type(if_true->get_module()), Instruction::Br, 1, bb) {
    set_operand(0, if_true);
}

BranchInst *BranchInst::create_cond_br(Value *cond, BasicBlock *if_true, BasicBlock *if_false, BasicBlock *bb) {
    if_true->add_pre_basic_block(bb);
    if_false->add_pre_basic_block(bb);
    bb->add_succ_basic_block(if_false);
    bb->add_succ_basic_block(if_true);

    return new BranchInst(cond, if_true, if_false, bb);
}

BranchInst *BranchInst::create_br(BasicBlock *if_true, BasicBlock *bb) {
    if_true->add_pre_basic_block(bb);
    bb->add_succ_basic_block(if_true);

    return new BranchInst(if_true, bb);
}

bool BranchInst::is_cond_br() const { return get_num_operand() == 3; }

string BranchInst::print() {
    string instr_ir;
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());

    if (is_cond_br()) {
        instr_ir += "  ";
        if (dynamic_cast<BinaryInst *>(this->get_operand(0)) && dynamic_cast<BinaryInst *>(this->get_operand(0))->get_operand(0)->get_type()->is_bool_type()) {
            instr_ir += "i1 ";
            instr_ir += print_as_op(this->get_operand(0), false);
        }else{
            instr_ir += print_as_op(this->get_operand(0), true);
        }
        instr_ir += ", label ";
        instr_ir += print_as_op(this->get_operand(1), true);
        instr_ir += ", label ";
        instr_ir += print_as_op(this->get_operand(2), true);
    } else {
        instr_ir += " label ";
        instr_ir += print_as_op(this->get_operand(0), true);
    }
    return instr_ir;
}

bool BranchInst::is_cmp_br() const { return get_num_operand() == 4; }

ReturnInst::ReturnInst(Value *val, BasicBlock *bb)
    : Instruction(Type::get_void_type(bb->get_module()), Instruction::Ret, 1, bb) {
    set_operand(0, val);
}

ReturnInst::ReturnInst(BasicBlock *bb) : Instruction(Type::get_void_type(bb->get_module()), Instruction::Ret, 0, bb) {}

ReturnInst *ReturnInst::create_ret(Value *val, BasicBlock *bb) { return new ReturnInst(val, bb); }

ReturnInst *ReturnInst::create_void_ret(BasicBlock *bb) { return new ReturnInst(bb); }

bool ReturnInst::is_void_ret() const { return get_num_operand() == 0; }

string ReturnInst::print() {
    string instr_ir;
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
    instr_ir += " ";
    if (!is_void_ret()) {
        instr_ir += this->get_operand(0)->get_type()->print();
        instr_ir += " ";
        instr_ir += print_as_op(this->get_operand(0), false);
    } else {
        instr_ir += "void";
    }
    return instr_ir;
}

StoreInst::StoreInst(Value *val, Value *ptr, BasicBlock *bb)
    : Instruction(Type::get_void_type(bb->get_module()), Instruction::Store, 2, bb) {
    set_operand(0, val);
    set_operand(1, ptr);
}

StoreInst *StoreInst::create_store(Value *val, Value *ptr, BasicBlock *bb) { return new StoreInst(val, ptr, bb); }

string StoreInst::print() {
    string instr_ir;
    if (this->get_operand(0)->get_type()->is_list_type()) {
        if (dynamic_cast<ArrayType *>(this->get_operand(0)->get_type()) &&
            dynamic_cast<IntegerType *>(
                dynamic_cast<ArrayType *>(this->get_operand(0)->get_type())->get_element_type())) {
            instr_ir += fmt::format("{} i8 {}, i8* {}", this->get_module()->get_instr_op_name(this->get_instr_type()),
                                    print_as_op(this->get_operand(0), false), print_as_op(this->get_operand(1), false));
        } else
            instr_ir += fmt::format("{} {}, {}* {}", this->get_module()->get_instr_op_name(this->get_instr_type()),
                                    print_as_op(this->get_operand(0), true), this->get_operand(0)->get_type()->print(),
                                    print_as_op(this->get_operand(1), false));
    } else if (dynamic_cast<GlobalVariable *>(this->get_operand(1))) {
        if (this->get_operand(1)->get_type()->is_string_type())
            instr_ir += fmt::format("{} {}, %$str$prototype_type* {}",
                                    this->get_module()->get_instr_op_name(this->get_instr_type()),
                                    print_as_op(this->get_operand(0), true), this->get_operand(1)->print());
        else
            instr_ir += fmt::format("{} {}, {}", this->get_module()->get_instr_op_name(this->get_instr_type()),
                                    print_as_op(this->get_operand(0), true), this->get_operand(1)->print());
    } else if (dynamic_cast<GlobalVariable *>(this->get_operand(0))) {
        if (this->get_operand(0)->get_type()->is_string_type())
            instr_ir += fmt::format("{} %$str$prototype_type* {}, {}",
                                    this->get_module()->get_instr_op_name(this->get_instr_type()),
                                    this->get_operand(0)->print(), print_as_op(this->get_operand(1), true));
        else
            instr_ir += fmt::format("{} {}, {}", this->get_module()->get_instr_op_name(this->get_instr_type()),
                                    this->get_operand(0)->print(), print_as_op(this->get_operand(1), true));
    } else
        instr_ir += fmt::format("{} {}, {}", this->get_module()->get_instr_op_name(this->get_instr_type()),
                                print_as_op(this->get_operand(0), true), print_as_op(this->get_operand(1), true));
    return instr_ir;
}

LoadInst::LoadInst(Type *ty, Value *ptr, BasicBlock *bb) : Instruction(ty, Instruction::Load, 1, bb) {
    set_operand(0, ptr);
    string tmp_inst;
    tmp_inst = this->get_operand(0)->get_type()->print();

    if (dynamic_cast<GetElementPtrInst *>(this->get_operand(0))) {
        get_operand(0)->set_type(tmp_inst.find("i32") != std::string::npos
                                     ? ArrayType::get(Type::get_int32_type(this->get_module()))
                                     : ArrayType::get(Type::get_int1_type(this->get_module())));
    }
}

LoadInst::LoadInst(Value *ptr1, Value *ptr2, BasicBlock *bb) : Instruction(ptr1->get_type(), Instruction::Load, 2, bb) {
    set_operand(0, ptr1);
    set_operand(1, ptr2);
    string tmp_inst;
    tmp_inst = this->get_operand(0)->get_type()->print();

    if (dynamic_cast<GetElementPtrInst *>(this->get_operand(0))) {
        get_operand(0)->set_type(tmp_inst.find("i32") != std::string::npos
                                     ? ArrayType::get(Type::get_int32_type(this->get_module()))
                                     : ArrayType::get(Type::get_int1_type(this->get_module())));
    }
}

LoadInst *LoadInst::create_load(Type *ty, Value *ptr, BasicBlock *bb) {
    return new LoadInst(ty->get_ptr_element_type(), ptr, bb);
}

LoadInst *LoadInst::create_load(Value *ptr1, Value *ptr2, BasicBlock *bb) { return new LoadInst(ptr1, ptr2, bb); }

Type *LoadInst::get_load_type() const { return get_operand(0)->get_type(); }

string LoadInst::print() {
    /** Should remove one * */
    string load_inst;
    /** Generate  %3 = load i32, ptr getelementptr inbounds (i32, ptr @a, i32 4) */
    if (this->get_operands().size() == 2) {
        load_inst += fmt::format("%{} = {} {}, ptr getelementptr inbounds ({}, ptr {}, {})", this->get_name(),
                                 this->get_module()->get_instr_op_name(this->get_instr_type()),
                                 this->get_operand(0)->get_type()->print(), this->get_operand(0)->get_type()->print(),
                                 print_as_op(this->get_operand(0), true), print_as_op(this->get_operand(1), true));
        return load_inst;
    }
    if (dynamic_cast<ConstantStr *>(this->get_operand(0)) ||
        (dynamic_cast<GlobalVariable *>(this->get_operand(0)) &&
         dynamic_cast<ConstantStr *>(dynamic_cast<GlobalVariable *>(this->get_operand(0))->get_init())))
        load_inst += fmt::format("%{} = {} %$str$prototype_type, %$str$prototype_type* {}", this->get_name(),
                                 this->get_module()->get_instr_op_name(this->get_instr_type()),
                                 print_as_op(this->get_operand(0), false));
    else if (dynamic_cast<GlobalVariable *>(this->get_operand(0)))
        load_inst += fmt::format("%{} = {} {}, {}", this->get_name(),
                                 this->get_module()->get_instr_op_name(this->get_instr_type()),
                                 std::regex_replace(this->get_operand(0)->get_type()->print(), to_load_replace, ""),
                                 this->get_operand(0)->print());
    else if (dynamic_cast<GetElementPtrInst *>(this->get_operand(0))) {
        if (dynamic_cast<GetElementPtrInst *>(this->get_operand(0))->get_element_type()->is_func_type())
            load_inst += fmt::format("%{} = {} {}*, {}** {}", this->get_name(),
                                     this->get_module()->get_instr_op_name(this->get_instr_type()),
                                     dynamic_cast<GetElementPtrInst *>(this->get_operand(0))->get_element_type()->print(),
                                     dynamic_cast<GetElementPtrInst *>(this->get_operand(0))->get_element_type()->print(),
                                     print_as_op(this->get_operand(0), false));
        else
            load_inst += fmt::format("%{} = {} {}, {}* {}", this->get_name(),
                                 this->get_module()->get_instr_op_name(this->get_instr_type()),
                                 dynamic_cast<GetElementPtrInst *>(this->get_operand(0))->get_element_type()->print(),
                                 dynamic_cast<GetElementPtrInst *>(this->get_operand(0))->get_element_type()->print(),
                                 print_as_op(this->get_operand(0), false));
    } else
        load_inst += fmt::format("%{} = {} {}, {}", this->get_name(),
                                 this->get_module()->get_instr_op_name(this->get_instr_type()),
                                 std::regex_replace(this->get_operand(0)->get_type()->print(), to_load_replace, ""),
                                 print_as_op(this->get_operand(0), true));
    return load_inst;
}

AllocaInst::AllocaInst(Type *ty, BasicBlock *bb)
    : Instruction(new ArrayType(ty), Instruction::Alloca, 0, bb), alloca_ty_(ty) {}

AllocaInst *AllocaInst::create_alloca(Type *ty, BasicBlock *bb) { return new AllocaInst(ty, bb); }

Type *AllocaInst::get_alloca_type() const { return alloca_ty_; }

string AllocaInst::print() {
    string instr_ir;
    instr_ir += "%";
    instr_ir += this->get_name();
    instr_ir += " = ";
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
    instr_ir += " ";
    if (get_alloca_type()->is_class_type())
        instr_ir += ((Class *)get_alloca_type())->print();
    else
        instr_ir += get_alloca_type()->print();
    if (get_alloca_type()->is_class_anon()) {
        instr_ir += ", align 4";
    }
    return instr_ir;
}

ZextInst::ZextInst(OpID op, Value *val, Type *ty, BasicBlock *bb) : Instruction(ty, op, 1, bb), dest_ty_(ty) {
    set_operand(0, val);
}

ZextInst *ZextInst::create_zext(Value *val, Type *ty, BasicBlock *bb) {
    return new ZextInst(Instruction::ZExt, val, ty, bb);
}

Type *ZextInst::get_dest_type() const { return dest_ty_; }

string ZextInst::print() {
    return fmt::format("%{} = {} {} {} to {}", this->get_name(),
                       this->get_module()->get_instr_op_name(this->get_instr_type()),
                       this->get_operand(0)->get_type()->print(), print_as_op(this->get_operand(0), false),
                       this->get_dest_type()->print());
}

InsertElementInst::InsertElementInst(OpID op, Value *val, Type *ty, BasicBlock *bb)
    : Instruction(ty, op, 1, bb), dest_ty_(ty) {
    set_operand(0, val);
}

InsertElementInst *InsertElementInst::create_insert_element(Value *val, Type *ty, BasicBlock *bb) {
    return new InsertElementInst(Instruction::InElem, val, ty, bb);
}

Type *InsertElementInst::get_dest_type() const { return dest_ty_; }

string InsertElementInst::print() {
    return fmt::format("%{} = {} {} {}, {}", this->get_name(),
                       this->get_module()->get_instr_op_name(this->get_instr_type()),
                       this->get_operand(0)->get_type()->print(), print_as_op(this->get_operand(0), false),
                       this->get_dest_type()->print());
}

ExtractElementInst::ExtractElementInst(OpID op, Value *val, Type *ty, BasicBlock *bb)
    : Instruction(ty, op, 1, bb), dest_ty_(ty) {
    set_operand(0, val);
}

ExtractElementInst *ExtractElementInst::create_extract_element(Value *val, Type *ty, BasicBlock *bb) {
    return new ExtractElementInst(Instruction::ExElem, val, ty, bb);
}

Type *ExtractElementInst::get_dest_type() const { return dest_ty_; }

string ExtractElementInst::print() {
    return fmt::format("%{} = {} {} {}, {}", this->get_name(),
                       this->get_module()->get_instr_op_name(this->get_instr_type()),
                       this->get_operand(0)->get_type()->print(), print_as_op(this->get_operand(0), false),
                       this->get_dest_type()->print());
}

BitCastInst::BitCastInst(OpID op, Value *val, Type *ty, BasicBlock *bb) : Instruction(ty, op, 1, bb), dest_ty_(ty) {
    set_operand(0, val);
}

BitCastInst *BitCastInst::create_bitcast(Value *val, Type *ty, BasicBlock *bb) {
    return new BitCastInst(Instruction::BitCast, val, ty, bb);
}

Type *BitCastInst::get_dest_type() const { return dest_ty_; }

string BitCastInst::print() {
    if (dynamic_cast<Class *>(this->get_operand(0))) {
        if (this->get_operand(0)->get_type()->print() == "%$object$dispatchTable_type") {
            return fmt::format("%{} = {} %$object$prototype_type* {} to {}", this->get_name(),
                               this->get_module()->get_instr_op_name(this->get_instr_type()),
                               fmt::format("@{}", dynamic_cast<Class *>(this->get_operand(0))->prototype_label_),
                               this->get_dest_type()->print());
        } else {
            return fmt::format("%{} = {} {}* {} to {}", this->get_name(),
                               this->get_module()->get_instr_op_name(this->get_instr_type()),
                               this->get_operand(0)->get_type()->print(),
                               fmt::format("@{}", dynamic_cast<Class *>(this->get_operand(0))->prototype_label_),
                               this->get_dest_type()->print());
        }
    } else if (dynamic_cast<ConstantStr *>(this->get_operand(0)) ||
               dynamic_cast<GlobalVariable *>(this->get_operand(0)) &&
                   dynamic_cast<ConstantStr *>(dynamic_cast<GlobalVariable *>(this->get_operand(0))->get_init()))
        return fmt::format("%{} = {} %$str$prototype_type* {} to {}", this->get_name(),
                           this->get_module()->get_instr_op_name(this->get_instr_type()),
                           print_as_op(this->get_operand(0), false), this->get_dest_type()->print());
    else
        return fmt::format("%{} = {} {} {} to {}", this->get_name(),
                           this->get_module()->get_instr_op_name(this->get_instr_type()),
                           this->get_operand(0)->get_type()->print(), print_as_op(this->get_operand(0), false),
                           this->get_dest_type()->print());
}

TruncInst::TruncInst(OpID op, Value *val, Type *ty, BasicBlock *bb) : Instruction(ty, op, 1, bb), dest_ty_(ty) {
    set_operand(0, val);
}

TruncInst *TruncInst::create_trunc(Value *val, Type *ty, BasicBlock *bb) {
    return new TruncInst(Instruction::Trunc, val, ty, bb);
}

Type *TruncInst::get_dest_type() const { return dest_ty_; }

string TruncInst::print() {
    return fmt::format("%{} = {} {} {} to {}", this->get_name(),
                       this->get_module()->get_instr_op_name(this->get_instr_type()),
                       this->get_operand(0)->get_type()->print(), print_as_op(this->get_operand(0), false),
                       this->get_dest_type()->print());
}

GetElementPtrInst::GetElementPtrInst(Value *ptr, Value *idx, BasicBlock *bb)
    : Instruction(ArrayType::get(get_element_type(ptr, idx)), Instruction::GEP, 2, bb), idx(idx) {
    set_operand(0, ptr);
    set_operand(1, idx);
    element_ty_ = get_element_type(ptr, idx);
}

GetElementPtrInst::GetElementPtrInst(Value *ptr, Value *idx)
    : Instruction(ArrayType::get(get_element_type(ptr, idx)), Instruction::GEP, 2), idx(idx) {
    set_operand(0, ptr);
    set_operand(1, idx);
    element_ty_ = get_element_type(ptr, idx);
}

Type *GetElementPtrInst::get_element_type(Value *ptr, Value *idx) {
    if (dynamic_cast<Class *>(ptr) && dynamic_cast<Class *>(ptr)->anon_) {
        auto res = dynamic_cast<Class *>(ptr)->get_anon_type(dynamic_cast<ConstantInt *>(idx)->get_value());
        if (dynamic_cast<Class *>(res))
            return ArrayType::get(res);
        else
            return res;
    }
    if (dynamic_cast<GlobalVariable *>(ptr) &&
            dynamic_cast<ConstantStr *>(dynamic_cast<GlobalVariable *>(ptr)->init_val_) ||
        dynamic_cast<Class *>(ptr->get_type()) && ptr->get_type()->is_string_type()) {
        /** Shall be the string literal */
        if (dynamic_cast<GlobalVariable *>(ptr)) {
            if (((ConstantInt *)idx)->get_value() == 3) /** __len__ */
                return ptr->get_type()->get_module()->get_int32_type();
            else if (((ConstantInt *)idx)->get_value() == 4) /** __str__ */
                return ArrayType::get(ptr->get_type()->get_module()->get_str_type());
        }
    }
    if (dynamic_cast<Class *>(ptr->get_type())) {
        if (dynamic_cast<ConstantInt *>(idx)) {
            if (dynamic_cast<ConstantInt *>(idx)->get_value() > 2) {
                return dynamic_cast<Class *>(ptr->get_type())
                    ->get_offset_attr(dynamic_cast<ConstantInt *>(idx)->get_value() - 3);
            } else if (dynamic_cast<ConstantInt *>(idx)->get_value() == 2) { // dispatch table
                return ArrayType::get(LabelType::get(dynamic_cast<Class *>(ptr->get_type())->dispatch_table_label_,
                                                     dynamic_cast<Class *>(ptr->get_type()),
                                                     ptr->get_type()->get_module()));
            } else {
                return new IntegerType(32, ptr->get_type()->get_module());
            }
        } else {
            return dynamic_cast<Class *>(ptr->get_type());
        }
    }
    if (dynamic_cast<LabelType *>(ptr->get_type())) {
        if (dynamic_cast<ConstantInt *>(idx)) {
            /** Get class by name from */
            return dynamic_cast<LabelType *>(ptr->get_type())
                ->get_class()
                ->get_method()
                ->at(dynamic_cast<ConstantInt *>(idx)->get_value())
                ->get_type();
        }
    }
    if (dynamic_cast<AllocaInst *>(ptr) && dynamic_cast<AllocaInst *>(ptr)->get_alloca_type()->is_class_anon())
        return dynamic_cast<Class *>(dynamic_cast<AllocaInst *>(ptr)->get_alloca_type())
            ->get_anon_type(dynamic_cast<ConstantInt *>(idx)->get_value());
    /** Let class type to get their attributes by the function */
    if (dynamic_cast<LoadInst *>(ptr) && dynamic_cast<Class *>(dynamic_cast<LoadInst *>(ptr)->get_type())) {
        /** distinguish the type of the class */
        if (dynamic_cast<Class *>(((LoadInst *)ptr)->get_operand(0)))
            return ((Class *)((LoadInst *)ptr)->get_operand(0))
                ->get_attribute()
                ->at(((ConstantInt *)idx)->get_value())
                ->init_obj->get_type();
        /** Shall be the string literal */
        else if (dynamic_cast<GlobalVariable *>(((LoadInst *)ptr)->get_operand(0)))
            if (((ConstantInt *)idx)->get_value() == 3) /** __len__ */
                return ptr->get_type()->get_module()->get_int32_type();
            else if (((ConstantInt *)idx)->get_value() == 4) /** __str__ */
                return ptr->get_type()->get_module()->get_str_type();
            else
                return nullptr;
    }
    /** if the access item is symbolic, should not be conslist */
    if (ptr->get_type()->is_list_type() &&
        dynamic_cast<Class *>(dynamic_cast<ArrayType *>(ptr->get_type())->get_element_type()) &&
        dynamic_cast<Class *>(dynamic_cast<ArrayType *>(ptr->get_type())->get_element_type())
                ->get_name()
                .find("list") != string::npos) {
        /** Shall be the string literal */
        if (((ConstantInt *)idx)->get_value() == 3) /** __len__ */
            return ptr->get_type()->get_module()->get_int32_type();
        else if (((ConstantInt *)idx)->get_value() == 4) /** __list__ */ {
            auto union_ = ptr->get_type()->get_module()->get_union();
            auto union_front = union_.begin();
            std::advance(union_front, 3);
            return ArrayType::get(*union_front);
        }
    }
    Type *ty = ptr->get_type()->get_array_element_type(); // get dereference type
    if (ty == nullptr) { // let list/str iteration work
        ty = ptr->get_type();
        if (ty->is_ptr_type()) {
            ty = ((ArrayType *)ty)->get_element_type();
        }
    } else
        LOG(DEBUG) << "Debug of get_element_type" << ptr->get_type()->get_array_element_type()->is_integer_type()
                   << ptr->get_type()->get_array_element_type()->is_bool_type()
                   << ptr->get_type()->get_array_element_type()->is_string_type();
    if (ptr->get_type()->is_string_type()) {
        return IntegerType::get(8, ptr->get_type()->get_module());
    }
    if (!dynamic_cast<Class *>(ty) && dynamic_cast<BitCastInst *>(ptr) &&
        dynamic_cast<CallInst *>(dynamic_cast<BitCastInst *>(ptr)->get_operand(0)) &&
        dynamic_cast<Function *>(
            dynamic_cast<CallInst *>(dynamic_cast<BitCastInst *>(ptr)->get_operand(0))->get_operand(0))
                ->get_name() == "alloc") {
        ty = dynamic_cast<LabelType *>(ty)->get_class();
    }
    if (dynamic_cast<Class *>(ty)) {
        if (dynamic_cast<ConstantInt *>(idx)) {
            if (dynamic_cast<ConstantInt *>(idx)->get_value() > 2) {
                return dynamic_cast<Class *>(ty)->get_offset_attr(dynamic_cast<ConstantInt *>(idx)->get_value() - 3);
            } else if (dynamic_cast<ConstantInt *>(idx)->get_value() == 2) { // dispatch table
                return ArrayType::get(LabelType::get(dynamic_cast<Class *>(ty)->dispatch_table_label_,
                                                     dynamic_cast<Class *>(ty), ty->get_module()));
            } else {
                return new IntegerType(32, ty->get_module());
            }
        } else {
            return dynamic_cast<Class *>(ty);
        }
    }
    if (dynamic_cast<LabelType *>(ty)) {
        if (dynamic_cast<ConstantInt *>(idx)) {
            /** Get class by name from */
            if (dynamic_cast<ConstantInt *>(idx)->get_value() <
                dynamic_cast<LabelType *>(ty)->get_class()->get_method()->size())
                return dynamic_cast<LabelType *>(ty)
                    ->get_class()
                    ->get_method()
                    ->at(dynamic_cast<ConstantInt *>(idx)->get_value())
                    ->get_type();
            else
                return dynamic_cast<LabelType *>(ty)
                    ->get_class()
                    ->get_attribute()
                    ->at(dynamic_cast<ConstantInt *>(idx)->get_value() - 3)
                    ->get_type();
        }
    }
    if (ty->is_array_type()) {
        auto *arr_ty = dynamic_cast<ArrayType *>(ty);
        ty = arr_ty->get_element_type();
        if (ty->is_array_type())
            arr_ty = dynamic_cast<ArrayType *>(ty);
    }
    if (ty->is_integer_type())
        ty = new IntegerType(32, ty->get_module());
    else if (ty->is_bool_type())
        ty = new IntegerType(8, ty->get_module());
    else if (ty->is_string_type())
        ty = new StringType("", ty->get_module());

    return ty;
}

string GetElementPtrInst::print() {
    string instr_ir;
    auto op0_type = std::regex_replace(this->get_operand(0)->get_type()->print(), to_load_replace, "");
    if (dynamic_cast<GlobalVariable *>(this->get_operand(0)) &&
        dynamic_cast<GlobalVariable *>(this->get_operand(0))->init_val_ != nullptr &&
        (dynamic_cast<ConstantArray *>(dynamic_cast<GlobalVariable *>(this->get_operand(0))->init_val_) ||
         dynamic_cast<ArrayType *>(dynamic_cast<GlobalVariable *>(this->get_operand(0))->init_val_->get_type()) &&
             dynamic_cast<ArrayType *>(dynamic_cast<GlobalVariable *>(this->get_operand(0))->init_val_->get_type())
                     ->get_num_of_elements() != -1))
        instr_ir += fmt::format(
            "%{} = {} inbounds {}, {}* {}, i32 0, {}", this->get_name(),
            this->get_module()->get_instr_op_name(this->get_instr_type()),
            dynamic_cast<ArrayType *>(dynamic_cast<GlobalVariable *>(this->get_operand(0))->init_val_->get_type())
                ->print(),
            dynamic_cast<ArrayType *>(dynamic_cast<GlobalVariable *>(this->get_operand(0))->init_val_->get_type())
                ->print(),
            print_as_op(this->get_operand(0), false),
            dynamic_cast<ConstantInt *>(idx) ? dynamic_cast<ConstantInt *>(idx)->print() : print_as_op(idx, true));
    else if (dynamic_cast<GlobalVariable *>(this->get_operand(0)) &&
                 ((GlobalVariable *)this->get_operand(0))->init_val_ != nullptr &&
                 dynamic_cast<ConstantStr *>(dynamic_cast<GlobalVariable *>(this->get_operand(0))->init_val_) ||
             op0_type == "%$str$prototype_type")
        instr_ir += fmt::format(
            "%{} = {} %$str$prototype_type, %$str$prototype_type* {}, i32 0, {}", this->get_name(),
            this->get_module()->get_instr_op_name(this->get_instr_type()), print_as_op(this->get_operand(0), false),
            dynamic_cast<ConstantInt *>(idx) ? dynamic_cast<ConstantInt *>(idx)->print() : print_as_op(idx, true));
    else if (op0_type.ends_with("$prototype_type") || op0_type.starts_with("%$class"))
        instr_ir += fmt::format("%{} = {} {}, {}* {}, i32 0, {}", this->get_name(),
                                this->get_module()->get_instr_op_name(this->get_instr_type()), op0_type, op0_type,
                                print_as_op(this->get_operand(0), false),
                                dynamic_cast<ConstantInt *>(idx) ? dynamic_cast<ConstantInt *>(idx)->print()
                                                                 : print_as_op(idx, true));

    else if (op0_type.ends_with("$dispatchTable_type")) {
        instr_ir += fmt::format("%{} = {} {}, {}* {}, i32 0, {}", this->get_name(),
                                this->get_module()->get_instr_op_name(this->get_instr_type()), op0_type, op0_type,
                                print_as_op(this->get_operand(0), false),
                                dynamic_cast<ConstantInt *>(idx) ? dynamic_cast<ConstantInt *>(idx)->print()
                                                                 : print_as_op(idx, true));
    } else if (dynamic_cast<LoadInst *>(this->get_operand(0))) {
        instr_ir += fmt::format(
            "%{} = {} {}, {} {}, {}", this->get_name(), this->get_module()->get_instr_op_name(this->get_instr_type()),
            std::regex_replace(dynamic_cast<LoadInst *>(this->get_operand(0))->get_type()->print(),to_load_replace,""),
            dynamic_cast<LoadInst *>(this->get_operand(0))->get_type()->print(),
            print_as_op(this->get_operand(0), false),
            dynamic_cast<ConstantInt *>(idx) ? dynamic_cast<ConstantInt *>(idx)->print() : print_as_op(idx, true));
    } else if (this->get_operand(0)->get_type()->is_string_type() ||
               this->get_operand(0)->get_type()->is_ptr_type() &&
                   dynamic_cast<ArrayType *>(this->get_operand(0)->get_type())->get_element_type()->is_bool_type())
        instr_ir += fmt::format(
            "%{} = {} i8*, i8** {}, {}", this->get_name(),
            this->get_module()->get_instr_op_name(this->get_instr_type()), print_as_op(this->get_operand(0), false),
            dynamic_cast<ConstantInt *>(idx) ? dynamic_cast<ConstantInt *>(idx)->print() : print_as_op(idx, true));
    else if (dynamic_cast<AllocaInst *>(this->get_operand(0)) || dynamic_cast<BitCastInst *>(this->get_operand(0)) ||
             dynamic_cast<GetElementPtrInst *>(this->get_operand(0)) ||
             this->get_operand(0)->get_type()->is_list_type()) {
        instr_ir += fmt::format(
            "%{} = {} {}, {}* {}, {}", this->get_name(), this->get_module()->get_instr_op_name(this->get_instr_type()),
            op0_type, op0_type, print_as_op(this->get_operand(0), false),
            dynamic_cast<ConstantInt *>(idx) ? dynamic_cast<ConstantInt *>(idx)->print() : print_as_op(idx, true));
    } else
        instr_ir += fmt::format(
            "%{} = {} {}, {} {}, {}", this->get_name(), this->get_module()->get_instr_op_name(this->get_instr_type()),
            op0_type, op0_type.starts_with("[") ? op0_type + "*" : op0_type,
            (op0_type.starts_with("[") ? print_as_op(this->get_operand(0), false) + ", i32 0"
                                       : print_as_op(this->get_operand(0), false)),
            dynamic_cast<ConstantInt *>(idx) ? dynamic_cast<ConstantInt *>(idx)->print() : print_as_op(idx, true));

    return instr_ir;
}

Type *GetElementPtrInst::get_element_type() const { return element_ty_; }

GetElementPtrInst *GetElementPtrInst::create_gep(Value *ptr, Value *idx, BasicBlock *bb) {
    return new GetElementPtrInst(ptr, idx, bb);
}

GetElementPtrInst *GetElementPtrInst::create_gep(Value *ptr, Value *idx) { return new GetElementPtrInst(ptr, idx); }

Value *GetElementPtrInst::get_idx() const { return idx; }

PhiInst::PhiInst(std::vector<Value *> vals, std::vector<BasicBlock *> val_bbs, Type *ty, BasicBlock *bb)
    : Instruction(ty, OpID::PHI, 2 * vals.size()) {
    for (int i = 0; i < vals.size(); i++) {
        set_operand(2 * i, vals[i]);
        set_operand(2 * i + 1, val_bbs[i]);
    }
    this->set_parent(bb);
}

PhiInst *PhiInst::create_phi(Type *ty, BasicBlock *bb) {
    std::vector<Value *> vals;
    std::vector<BasicBlock *> val_bbs;
    return new PhiInst(vals, val_bbs, ty, bb);
}

string PhiInst::print() {
    string instr_ir;
    instr_ir += "%";
    instr_ir += this->get_name();
    instr_ir += " = ";
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
    instr_ir += " ";
    instr_ir += get_lval()->get_type()->print();
    instr_ir += " ";
    for (int i = 0; i < this->get_num_operand() / 2; i++) {
        if (i > 0)
            instr_ir += ", ";
        instr_ir += "[ ";
        instr_ir += print_as_op(this->get_operand(2 * i), false);
        instr_ir += ", ";
        instr_ir += print_as_op(this->get_operand(2 * i + 1), false);
        instr_ir += " ]";
    }
    if (this->get_num_operand() / 2 < this->get_parent()->get_pre_basic_blocks().size()) {
        for (int i = 0; i < this->get_parent()->get_pre_basic_blocks().size(); i++) {
            auto tmp = this->get_parent()->get_pre_basic_blocks().begin();
            std::advance(tmp, i);
            auto pre_bb = *tmp;
            if (std::find(this->get_operands().begin(), this->get_operands().end(), static_cast<Value *>(pre_bb)) ==
                this->get_operands().end()) {
                /** find a pre_bb is not in PHI */
                instr_ir += " [ undef, " + print_as_op(pre_bb, false) + " ]";
            }
            if (i != this->get_parent()->get_pre_basic_blocks().size() - 1)
                instr_ir += ", ";
        }
    }
    return instr_ir;
}

void VExtInst::assert_valid() {
    assert(get_operand(0)->get_type()->is_integer_type());
    if (get_operand(1) != nullptr) {
        assert(get_operand(1)->get_type()->is_integer_type());
    }
    if (get_operand(2) != nullptr)
        assert(get_operand(2)->get_type()->is_integer_type());
}

VExtInst::VExtInst(Value *v1, Value *v2, Value *v3, vv_type type, BasicBlock *parent)
    : type_(type), Instruction(Type::get_int32_type(parent->get_module()), Instruction::VExt, 3, parent) {
    set_operand(0, v1);
    set_operand(1, v2);
    set_operand(2, v3);
    assert_valid();
}

VExtInst::VExtInst(Value *v1, Value *v2, vv_type type, BasicBlock *parent)
    : type_(type), Instruction(Type::get_int32_type(parent->get_module()), Instruction::VExt, 3, parent) {
    set_operand(0, v1);
    set_operand(1, v2);
    assert_valid();
}

VExtInst::VExtInst(Value *v1, vv_type type, BasicBlock *parent)
    : type_(type), Instruction(Type::get_int32_type(parent->get_module()), Instruction::VExt, 2, parent) {
    /**
     *  VStore instruction for LLVM
     *  %5 = bitcast i64* %arrayidx to <4 x i64>*
     *  store <4 x i64> %4, <4 x i64>* %5, align
     */
    if (type_ == vv_type::VSE) {
        set_operand(0, v1);
        set_operand(0, v1);
    }
    if (type_ == vv_type::VLE) {
        for (int i = 0; i < int(vlen / 32); i++) {
            /** VLoad instruction for LLVM
             *  %0 = insertelement <4 x i64> zeroinitializer, i64 %i.0, i64 0
             *  %1 = insertelement <4 x i64> %0, i64 %i.0, i64 1
             *  %2 = insertelement <4 x i64> %1, i64 %i.0, i64 2
             *  %3 = insertelement <4 x i64> %2, i64 %i.0, i64 3
             **/
            set_operand(i, v1);
            i++;
        }
    }
    assert_valid();
}

string VExtInst::print() {
    string instr_ir;
    instr_ir += "%";
    instr_ir += this->get_name();
    instr_ir += " = ";
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
    instr_ir += " ";
    instr_ir += this->get_operand(0)->get_type()->print();
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), false);
    instr_ir += ", ";
    if (this->get_operand(0)->get_type() == (this->get_operand(1)->get_type())) {
        instr_ir += print_as_op(this->get_operand(1), false);
    } else {
        instr_ir += print_as_op(this->get_operand(1), true);
    }
    if (this->get_operand(0)->get_type() == (this->get_operand(2)->get_type())) {
        instr_ir += ", ";
        instr_ir += print_as_op(this->get_operand(1), false);
    } else {
        instr_ir += ", ";
        instr_ir += print_as_op(this->get_operand(1), true);
    }
    return instr_ir;
}

Accstart::Accstart(Module *m) : Instruction(Type::get_int32_type(m), Instruction::ACCSTART, 0) {}

Accstart *Accstart::create_accstart(Module *m) { return new Accstart(m); }

string Accstart::print() {
    string instr_ir;
    instr_ir += R"(tail call void asm sideeffect "{}", ""())";
    return instr_ir;
}

Accend::Accend(Module *m, Accstart *start) : Instruction(Type::get_void_type(m), Instruction::ACCEND, 1) {
    set_operand(0, start);
}

Accend *Accend::create_accend(Module *m, Accstart *start) { return new Accend(m, start); }

string Accend::print() {
    string instr_ir;
    instr_ir += R"(tail call void  "{}", ""())";
    return instr_ir;
}

AsmInst::AsmInst(Module *m, string asm_str, BasicBlock *bb)
    : asm_str_(std::move(asm_str)), Instruction(Type::get_void_type(m), Instruction::ASM, 1, bb) {
    set_operand(0, bb);
};

string AsmInst::print() {
    string instr_ir;
    instr_ir += fmt::format(R"(tail call void asm sideeffect "{}", ""())", asm_str_);
    return instr_ir;
}

AsmInst *AsmInst::create_asm(Module *m_, const string &asm_str, BasicBlock *bb) { return new AsmInst(m_, asm_str, bb); }

} // namespace lightir