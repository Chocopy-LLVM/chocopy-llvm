//
// Created by yiwei yang on 2/15/21.
//

#ifndef CHOCOPY_COMPILER_MODULE_HPP
#define CHOCOPY_COMPILER_MODULE_HPP

#pragma once

#ifdef LLVM
#include <llvm/IR/Module.h>
#endif
#include "Class.hpp"
#include "Function.hpp"
#include "GlobalVariable.hpp"
#include "Type.hpp"
#include "Value.hpp"
#include <chocopy_logging.hpp>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <utility>

using std::list;
using std::map;
using std::string;
namespace lightir {
class BasicBlock;
class Function;
class Instruction : public User {
public:
    enum OpID {
        // Terminator Instructions
        Ret,
        Br,
        // Standard unary operators
        Neg,
        Not,
        // Standard binary operators
        Add,
        Sub,
        Mul,
        Div,
        Rem,
        And,
        Or,
        // Memory operators
        Alloca,
        Load,
        Store,
        // Shift operators
        Shl, // <<
        AShr, // arithmetic >>
        LShr, // logical >>
        // Other operators
        ICmp,
        PHI,
        Call,
        GEP,
        ZExt, // zero extend
        InElem, // insert element
        ExElem, // extract element
        BitCast, // bitcast
        Trunc, // truncate
        VExt, // VExt SIMD
        ASM,
        // Acc Intrinsic
        ACCSTART,
        ACCEND
    };
    /** create instruction, auto insert to bb
     * ty here is result type */
    Instruction(Type *ty, OpID id, unsigned num_ops, BasicBlock *parent);
    Instruction(Type *ty, OpID id, unsigned num_ops);
    inline const BasicBlock *get_parent() const { return parent_; }
    inline BasicBlock *get_parent() { return parent_; }
    void set_parent(BasicBlock *parent) { this->parent_ = parent; }
    /** Return the function this instruction belongs to. */
    const Function *get_function() const;
    Function *get_function() { return const_cast<Function *>(static_cast<const Instruction *>(this)->get_function()); }

    Module *get_module();

    OpID get_instr_type() { return op_id_; }

    bool is_void() {
        return ((op_id_ == Ret) || (op_id_ == Br) || (op_id_ == Store) ||
                (op_id_ == Call && this->get_type()->is_void_type()));
    }

    bool is_phi() { return op_id_ == PHI; }
    bool is_store() { return op_id_ == Store; }
    bool is_alloca() { return op_id_ == Alloca; }
    bool is_ret() { return op_id_ == Ret; }
    bool is_load() { return op_id_ == Load; }
    bool is_br() { return op_id_ == Br; }

    bool is_add() { return op_id_ == Add; }
    bool is_sub() { return op_id_ == Sub; }
    bool is_rem() { return op_id_ == Rem; }
    bool is_mul() { return op_id_ == Mul; }
    bool is_div() { return op_id_ == Div; }
    bool is_and() { return op_id_ == And; }
    bool is_or() { return op_id_ == Or; }

    bool is_cmp() { return op_id_ == ICmp; }

    bool is_call() { return op_id_ == Call; }
    bool is_gep() { return op_id_ == GEP; }
    bool is_zext() { return op_id_ == ZExt; }

    bool is_shl() { return op_id_ == Shl; }
    bool is_ashr() { return op_id_ == AShr; }
    bool is_lshr() { return op_id_ == LShr; }

    bool is_vext() { return op_id_ == VExt; }
    bool is_acc() { return op_id_ == ACCEND || op_id_ == ACCSTART; }

    bool is_binary() {
        return (is_add() || is_sub() || is_mul() || is_div() || is_rem() || is_shl() || is_ashr() || is_lshr() ||
                is_and() || is_or()) &&
               (get_num_operand() == 2);
    }

    bool is_void_ret() {
        return ((op_id_ == Ret) || (op_id_ == Br) || (op_id_ == Store) ||
                (op_id_ == Call && this->get_type()->is_void_type()));
    }

    bool isTerminator() { return is_br() || is_ret(); }

    string print_comment() { return comment_; };

    void set_comment(std::string_view comment) { comment_ = comment; };

private:
    BasicBlock *parent_;
    OpID op_id_;
    unsigned num_ops_;
    string comment_;
};

class BinaryInst : public Instruction {
private:
    BinaryInst(Type *ty, OpID id, Value *v1, Value *v2, BasicBlock *bb);

public:
    /** create Add instruction, auto insert to bb */
    static BinaryInst *create_add(Value *v1, Value *v2, BasicBlock *bb, Module *m);

    /** create Sub instruction, auto insert to bb */
    static BinaryInst *create_sub(Value *v1, Value *v2, BasicBlock *bb, Module *m);

    /** create mul instruction, auto insert to bb */
    static BinaryInst *create_mul(Value *v1, Value *v2, BasicBlock *bb, Module *m);

    /** create Div instruction, auto insert to bb */
    static BinaryInst *create_sdiv(Value *v1, Value *v2, BasicBlock *bb, Module *m);

    /** create mod instruction, auto insert to bb */
    static BinaryInst *create_rem(Value *v1, Value *v2, BasicBlock *bb, Module *m);

    /** create and instruction, auto insert to bb */
    static BinaryInst *create_and(Value *v1, Value *v2, BasicBlock *bb, Module *m);

    /** create or instruction, auto insert to bb */
    static BinaryInst *create_or(Value *v1, Value *v2, BasicBlock *bb, Module *m);

    string print() override;

private:
    void assert_valid();
};

class UnaryInst : public Instruction {
private:
    UnaryInst(Type *ty, OpID id, Value *v, BasicBlock *bb);

    UnaryInst(Type *ty, OpID id, BasicBlock *bb) : Instruction(ty, id, 1, bb) {}

public:
    /** create neg instruction, auto insert to bb */
    static UnaryInst *create_neg(Value *v, BasicBlock *bb, Module *m);

    /** create not instruction, auto insert to bb */
    static UnaryInst *create_not(Value *v, BasicBlock *bb, Module *m);

    string print() override;
};

class CmpInst : public Instruction {
public:
    enum CmpOp {
        EQ, /* == */
        NE, /* != */
        GT, /* >  */
        GE, /* >= */
        LT, /* <  */
        LE /* <= */
    };

private:
    CmpInst(Type *ty, CmpOp op, Value *lhs, Value *rhs, BasicBlock *bb);

public:
    static CmpInst *create_cmp(CmpOp op, Value *lhs, Value *rhs, BasicBlock *bb, Module *m);

    CmpOp get_cmp_op() { return cmp_op_; }

    string print() override;

private:
    CmpOp cmp_op_;

    void assert_valid();
};

class CallInst : public Instruction {
protected:
    CallInst(Function *func, vector<Value *> args, BasicBlock *bb);
    CallInst(Value *real_func, FunctionType *func, vector<Value *> args, BasicBlock *bb);

public:
    static CallInst *create(Function *func, vector<Value *> args, BasicBlock *bb);
    static CallInst *create(Value *real_func, FunctionType *func, std::vector<Value *> args, BasicBlock *bb);
    FunctionType *get_function_type() const;

    string print() override;
};

class BranchInst : public Instruction {
private:
    BranchInst(Value *cond, BasicBlock *if_true, BasicBlock *if_false, BasicBlock *bb);
    BranchInst(BasicBlock *if_true, BasicBlock *bb);

public:
    static BranchInst *create_cond_br(Value *cond, BasicBlock *if_true, BasicBlock *if_false, BasicBlock *bb);
    static BranchInst *create_br(BasicBlock *if_true, BasicBlock *bb);

    bool is_cond_br() const;
    bool is_cmp_br() const;

    string print() override;
};

class ReturnInst : public Instruction {
private:
    ReturnInst(Value *val, BasicBlock *bb);
    explicit ReturnInst(BasicBlock *bb);

public:
    static ReturnInst *create_ret(Value *val, BasicBlock *bb);
    static ReturnInst *create_void_ret(BasicBlock *bb);
    bool is_void_ret() const;

    string print() override;
};

class StoreInst : public Instruction {
private:
    StoreInst(Value *val, Value *ptr, BasicBlock *bb);

public:
    static StoreInst *create_store(Value *val, Value *ptr, BasicBlock *bb);

    Value *get_rval() { return this->get_operand(0); }
    Value *get_lval() { return this->get_operand(1); }

    string print() override;
};

class LoadInst : public Instruction {
private:
    LoadInst(Type *ty, Value *ptr, BasicBlock *bb);
    LoadInst(Value *ptr1, Value *ptr2, BasicBlock *bb);

public:
    static LoadInst *create_load(Type *ty, Value *ptr, BasicBlock *bb);
    static LoadInst *create_load(Value *ptr1, Value *ptr2, BasicBlock *bb);
    Value *get_lval() { return this->get_operand(0); }

    Type *get_load_type() const;

    string print() override;
};

class AllocaInst : public Instruction {
private:
    AllocaInst(Type *ty, BasicBlock *bb);

public:
    static AllocaInst *create_alloca(Type *ty, BasicBlock *bb);

    Type *get_alloca_type() const;

    string print() override;

private:
    Type *alloca_ty_;
};

class ZextInst : public Instruction {
private:
    ZextInst(OpID op, Value *val, Type *ty, BasicBlock *bb);

public:
    static ZextInst *create_zext(Value *val, Type *ty, BasicBlock *bb);

    Type *get_dest_type() const;

    string print() override;

private:
    Type *dest_ty_;
};

class InsertElementInst : public Instruction {
private:
    InsertElementInst(OpID op, Value *val, Type *ty, BasicBlock *bb);

public:
    static InsertElementInst *create_insert_element(Value *val, Type *ty, BasicBlock *bb);

    Type *get_dest_type() const;

    string print() override;

private:
    Type *dest_ty_;
};

class ExtractElementInst : public Instruction {
private:
    ExtractElementInst(OpID op, Value *val, Type *ty, BasicBlock *bb);

public:
    static ExtractElementInst *create_extract_element(Value *val, Type *ty, BasicBlock *bb);

    Type *get_dest_type() const;

    string print() override;

private:
    Type *dest_ty_;
};

class BitCastInst : public Instruction {
private:
    BitCastInst(OpID op, Value *val, Type *ty, BasicBlock *bb);

public:
    static BitCastInst *create_bitcast(Value *val, Type *ty, BasicBlock *bb);

    Type *get_dest_type() const;

    string print() override;

private:
    Type *dest_ty_;
};

class TruncInst : public Instruction {
private:
    TruncInst(OpID op, Value *val, Type *ty, BasicBlock *bb);

public:
    static TruncInst *create_trunc(Value *val, Type *ty, BasicBlock *bb);

    Type *get_dest_type() const;

    string print() override;

private:
    Type *dest_ty_;
};

class GetElementPtrInst : public Instruction {
public:
    GetElementPtrInst(Value *ptr, Value *idx, BasicBlock *bb);
    GetElementPtrInst(Value *ptr, Value *idx);
    GetElementPtrInst(Type *ty, unsigned num_ops, BasicBlock *bb, Type *elem_ty)
        : Instruction(ty, Instruction::GEP, num_ops, bb), element_ty_(elem_ty) {}

    static Type *get_element_type(Value *ptr, Value *idx);
    static GetElementPtrInst *create_gep(Value *ptr, Value *idx, BasicBlock *bb);
    static GetElementPtrInst *create_gep(Value *ptr, Value *idx);
    Type *get_element_type() const;
    Value *get_idx() const;

    string print() override;

private:
    Value *idx{};
    Type *element_ty_;
};

class PhiInst : public Instruction {
private:
    PhiInst(vector<Value *> vals, vector<BasicBlock *> val_bbs, Type *ty, BasicBlock *bb);
    PhiInst(Type *ty, OpID op, unsigned num_ops, BasicBlock *bb) : Instruction(ty, op, num_ops, bb) {}
    Value *l_val_{};

public:
    static PhiInst *create_phi(Type *ty, BasicBlock *bb);
    Value *get_lval() { return l_val_; }
    void set_lval(Value *l_val) { l_val_ = l_val; }
    void add_phi_pair_operand(Value *val, Value *pre_bb) {
        this->add_operand(val);
        this->add_operand(pre_bb);
    }
    string print() override;
};

class VExtInst : public Instruction {
public:
    enum vv_type { VLE = 0, VSETVLI, VADD, VMUL, VDIV, VREM, VSHR, VASHR, VLSHR, VSE };

    /** Default value for riscv32
     * This part refer to the LLVM clang/test/CodeGen/RISCV/rvv-intrinsics-overloaded */
    int vlen = 256;
    int elen = 64;
    vv_type type_;

    static VExtInst *create_vlw(Value *v1, BasicBlock *parent) { return new VExtInst(v1, vv_type::VLE, parent); }
    static VExtInst *create_vsw(Value *v1, BasicBlock *parent) { return new VExtInst(v1, vv_type::VSE, parent); }
    static VExtInst *create_vadd(Value *v1, Value *v2, BasicBlock *parent) {
        return new VExtInst(v1, v2, vv_type::VADD, parent);
    }
    static VExtInst *create_vdiv(Value *v1, Value *v2, Value *v3, BasicBlock *parent) {
        return new VExtInst(v1, v2, v3, vv_type::VDIV, parent);
    }
    static VExtInst *create_vrem(Value *v1, Value *v2, Value *v3, BasicBlock *parent) {
        return new VExtInst(v1, v2, v3, vv_type::VREM, parent);
    }
    static VExtInst *create_vmul(Value *v1, Value *v2, Value *v3, BasicBlock *parent) {
        return new VExtInst(v1, v2, v3, vv_type::VMUL, parent);
    }
    static VExtInst *create_vsetvli(Value *v1, Value *v2, Value *v3, BasicBlock *parent) {
        return new VExtInst(v1, v2, v3, vv_type::VSETVLI, parent);
    }
    /** For final riscv instruction
     *   vsetvli t0, a0, e32
     *   vlw.v v0, (a1)
     *   vadd.vs v2, v0, a2
     *   vsw.v v2, (a1)
     *
     *   For intermediate riscv instruction
     *       define <vscale x 4 x i32> @load_add(<vscale x 4 x i32>* %0, <vscale x 4 x i32> %1, i32 %2) #0 {
     *       entry:
     *           %a = call <vscale x 4 x i32> @llvm.riscv.vle.nxv4i32.i32(<vscale x 4 x i32>* %0, i32 %2)
     *           %b = call <vscale x 4 x i32> @llvm.riscv.vadd.nxv4i32.nxv4i32.i32(<vscale x 4 x i32> %a, <vscale x 4 x
     *  i32> %1, i32 %2) ret <vscale x 4 x i32> %b
     *       }
     *
     *       ; Function Attrs: nounwind readnone
     *       declare <vscale x 4 x i32> @llvm.riscv.vadd.nxv4i32.nxv4i32.i32(<vscale x 4 x i32>, <vscale x 4 x i32>,
     *i32) #4
     *
     *       define void @main() {
     *       label0:
     *           %0 = insertelement <vscale x 4 x i32> undef, i32 5, i32 0
     *           %1 = alloca <vscale x 4 x i32>
     *           %op1 = call <vscale x 4 x i32> @load_add(<vscale x 4 x i32>* %1,<vscale x 4 x i32> %0, i32 4)
     *           %op2 = extractelement <vscale x 4 x i32> %op1, i32 0
     *           %op3 = call %$int$prototype_type @makeint(i32 %op2)
     *           call void @print_int(%$int$prototype_type %op3)
     *           ret void
     *       }
     **/

    string print() override;

private:
    VExtInst(Value *v1, Value *v2, Value *v3, vv_type type_, BasicBlock *parent);
    VExtInst(Value *v1, Value *v2, vv_type type, BasicBlock *parent);
    VExtInst(Value *v1, vv_type type, BasicBlock *parent);
    void assert_valid();
};

class Accstart : public Instruction {
    /** %thread_id = MTSTART %num_threads */
private:
    explicit Accstart(Module *m);

public:
    Accstart *create_accstart(Module *m);
    string print() override;
};

class Accend : public Instruction {
    /** %thread_id = MTSTART %num_threads */
private:
    Accend(Module *m, Accstart *start);

public:
    Accend *create_accend(Module *m, Accstart *start);
    string print() override;
};

class AsmInst : public Instruction {
    /** tail call void asm sideeffect " addi    s0, sp, 16\0Alui     a0, 2", ""() */
private:
    AsmInst(Module *m, string asm_str, BasicBlock *bb);
    string asm_str_;

public:
    string print() override;
    string get_asm() { return asm_str_; };
    static AsmInst *create_asm(Module *m, const string &asm_str, BasicBlock *bb);
};

class Function;
class GlobalVariable;
class Class;

class Module {
public:
    explicit Module(string name);
    virtual ~Module();

    Type *get_void_type();
    Type *get_label_type();
    Type *get_class_type(int id_);
    IntegerType *get_int1_type();
    IntegerType *get_int32_type();
    StringType *get_str_type();
    ArrayType *get_array_type(Type *contained, unsigned num_elements);
    ArrayType *get_array_type(Type *contained);
    ArrayType *get_array_type();

    void add_function(Function *f);
    list<Function *> get_functions();
    void add_global_variable(GlobalVariable *g);
    list<GlobalVariable *> get_global_variable();
    void add_class(Class *c);
    list<Class *> get_class();
    void add_union(Union *u);
    list<Union *> get_union();
    string get_instr_op_name(Instruction::OpID instr) { return instr_id2string_[instr]; }
    void set_print_name();
    void add_class_type(Type *);
    virtual string print();
    string module_name_; /* Human-readable identifier for the module */
    string source_file_name_; /* Original source file name for module, for test and debug */
    int vectorize_num = 4;
    int thread_num = 4;
    bool is_declaration_ = false;

private:
    list<GlobalVariable *> global_list_; /* The Global Variables in the module */
    list<Function *> function_list_; /* The Functions in the module */
    list<Class *> class_list_; /* The Functions in the module */
    list<Union *> union_list_; /* The Functions in the module */
    map<Instruction::OpID, string> instr_id2string_; /* Instruction from opid to string */
    IntegerType *int1_ty_;
    IntegerType *int32_ty_;
    StringType *str_ty_;
    Type *label_ty_;
    map<int, Type *> obj_ty_;
    Type *void_ty_;
    map<pair<Type *, int>, ArrayType *> array_map_;
};
} // namespace lightir
#endif // CHOCOPY_COMPILER_MODULE_HPP
