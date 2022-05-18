//
// Created by yiwei yang on 2/15/21.
//

#ifndef CHOCOPY_COMPILER_IRBUILDER_HPP
#define CHOCOPY_COMPILER_IRBUILDER_HPP

#include "BasicBlock.hpp"
#include "Value.hpp"
#include <utility>

namespace lightir {
class IRBuilder {
private:
    BasicBlock *BB_;
    Module *m_;

public:
    IRBuilder(BasicBlock *bb, Module *m) : BB_(bb), m_(m){};
    ~IRBuilder() = default;
    Module *get_module() { return m_; }
    BasicBlock *get_insert_block() { return this->BB_; }
    void set_insert_point(BasicBlock *bb) { this->BB_ = bb; }
    BinaryInst *create_iadd(Value *lhs, Value *rhs) { return BinaryInst::create_add(lhs, rhs, this->BB_, m_); }
    BinaryInst *create_isub(Value *lhs, Value *rhs) { return BinaryInst::create_sub(lhs, rhs, this->BB_, m_); }
    BinaryInst *create_imul(Value *lhs, Value *rhs) { return BinaryInst::create_mul(lhs, rhs, this->BB_, m_); }
    BinaryInst *create_isdiv(Value *lhs, Value *rhs) { return BinaryInst::create_sdiv(lhs, rhs, this->BB_, m_); }
    BinaryInst *create_irem(Value *lhs, Value *rhs) { return BinaryInst::create_rem(lhs, rhs, this->BB_, m_); }
    BinaryInst *create_iand(Value *lhs, Value *rhs) { return BinaryInst::create_and(lhs, rhs, this->BB_, m_); }
    BinaryInst *create_ior(Value *lhs, Value *rhs) { return BinaryInst::create_or(lhs, rhs, this->BB_, m_); }
    UnaryInst *create_inot(Value *lhs) { return UnaryInst::create_not(lhs, this->BB_, m_); }
    UnaryInst *create_ineg(Value *lhs) { return UnaryInst::create_neg(lhs, this->BB_, m_); }
    PhiInst *create_phi(Type *lhs) { return PhiInst::create_phi(lhs, this->BB_); }

    CmpInst *create_icmp_eq(Value *lhs, Value *rhs) {
        return CmpInst::create_cmp(CmpInst::EQ, lhs, rhs, this->BB_, m_);
    }
    CmpInst *create_icmp_ne(Value *lhs, Value *rhs) {
        return CmpInst::create_cmp(CmpInst::NE, lhs, rhs, this->BB_, m_);
    }
    CmpInst *create_icmp_gt(Value *lhs, Value *rhs) {
        return CmpInst::create_cmp(CmpInst::GT, lhs, rhs, this->BB_, m_);
    }
    CmpInst *create_icmp_ge(Value *lhs, Value *rhs) {
        return CmpInst::create_cmp(CmpInst::GE, lhs, rhs, this->BB_, m_);
    }
    CmpInst *create_icmp_lt(Value *lhs, Value *rhs) {
        return CmpInst::create_cmp(CmpInst::LT, lhs, rhs, this->BB_, m_);
    }
    CmpInst *create_icmp_le(Value *lhs, Value *rhs) {
        return CmpInst::create_cmp(CmpInst::LE, lhs, rhs, this->BB_, m_);
    }

    CallInst *create_call(Value *func, vector<Value *> args) {
        return CallInst::create(dynamic_cast<Function *>(func), std::move(args), this->BB_);
    }
    CallInst *create_call(Value *real_func, Type *func, vector<Value *> args) {
        return CallInst::create(real_func, dynamic_cast<FunctionType *>(func), std::move(args), this->BB_);
    }

    BranchInst *create_br(BasicBlock *if_true) { return BranchInst::create_br(if_true, this->BB_); }
    BranchInst *create_cond_br(Value *cond, BasicBlock *if_true, BasicBlock *if_false) {
        return BranchInst::create_cond_br(cond, if_true, if_false, this->BB_);
    }

    ReturnInst *create_ret(Value *val) { return ReturnInst::create_ret(val, this->BB_); }
    GetElementPtrInst *create_gep(Value *ptr, Value *idx) { return GetElementPtrInst::create_gep(ptr, idx, this->BB_); }

    ReturnInst *create_void_ret() { return ReturnInst::create_void_ret(this->BB_); }

    StoreInst *create_store(Value *val, Value *ptr) { return StoreInst::create_store(val, ptr, this->BB_); }
    LoadInst *create_load(Type *ty, Value *ptr) { return LoadInst::create_load(ty, ptr, this->BB_); }
    LoadInst *create_load(Value *ptr1, Value *ptr2) { return LoadInst::create_load(ptr1, ptr2, this->BB_); }
    LoadInst *create_load(Value *ptr) { return LoadInst::create_load(ptr->get_type(), ptr, this->BB_); }

    AllocaInst *create_alloca(Type *ty) { return AllocaInst::create_alloca(ty, this->BB_); }
    ZextInst *create_zext(Value *val, Type *ty) { return ZextInst::create_zext(val, ty, this->BB_); }
    InsertElementInst *create_insertelement(Value *val, Type *ty) {
        return InsertElementInst::create_insert_element(val, ty, this->BB_);
    }
    ExtractElementInst *create_exsertelement(Value *val, Type *ty) {
        return ExtractElementInst::create_extract_element(val, ty, this->BB_);
    }
    BitCastInst *create_bitcast(Value *val, Type *ty) { return BitCastInst::create_bitcast(val, ty, this->BB_); }
    TruncInst *create_trunc(Value *val, Type *ty) { return TruncInst::create_trunc(val, ty, this->BB_); }
    AsmInst *create_asm(const string &asm_str) { return AsmInst::create_asm(m_, asm_str, this->BB_); }
    void delete_instr(Instruction *lnstr) { this->BB_->delete_instr(lnstr); }
};
} // namespace lightir

#endif // CHOCOPY_COMPILER_IRBUILDER_HPP
