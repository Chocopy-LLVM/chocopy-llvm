//
// Created by yangy on 8/19/2021.
//

#ifndef CHOCOPY_COMPILER_CHOCOPY_OPTIMIZATION_HPP
#define CHOCOPY_COMPILER_CHOCOPY_OPTIMIZATION_HPP

#include "BasicBlock.hpp"
#include "IRBuilder.hpp"
#include "Module.hpp"
#include <fstream>
#include <list>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

using std::string;
/** https://stackoverflow.com/questions/5447675/how-to-convert-a-string-to-a-class-name */
template <class T, typename M_> void *constructor(M_ *module_) {
    return (void *)new T{module_};
}
template <typename M_> struct arg_to_pass {
    typedef void *(*constructor_t)(M_ *);
    typedef std::map<std::string, constructor_t> map_type;
    map_type m_classes;
    M_ *module;

    template <class T> void register_class(std::string const &n, M_ *&module_) {
        module = module_;
        m_classes.insert(std::make_pair(n, &constructor<T, M_>));
    }

    void *construct(std::string const &n) {
        auto i = m_classes.find(n);
        if (i == m_classes.end())
            return nullptr; // or throw or whatever you want
        return i->second(module);
    }
};

namespace lightir {
class Pass {
public:
    explicit Pass(Module *m) : m_(m) {}

    virtual void run() = 0;

    void setName(string name) { name_ = std::move(name); }

    string getName() { return name_; }

    void setPrintIR(bool print_ir) { print_ir_ = print_ir; }

    bool isPrintIR() const { return print_ir_; }

protected:
    Module *m_;
    string name_;
    bool print_ir_ = false;
};

class Analysis : public Pass {
public:
    explicit Analysis(Module *m) : Pass(m) {}
};

class Transform : public Pass {
public:
    explicit Transform(Module *m) : Pass(m) {}
};

class PassManager {
public:
    explicit PassManager(Module *m);

    template <typename PassTy> void add_pass(bool print_ir = false) {
        passes_.push_back(new PassTy(m_));
        passes_.back()->setName(typeid(PassTy).name());
        passes_.back()->setPrintIR(print_ir);
        LOG(DEBUG) << typeid(PassTy).name();
    }

    void add_pass(const string &name, bool print_ir = false);

    void run(bool print_ir = false);

private:
    std::vector<Pass *> passes_;
    Module *m_;
};

class Dominators : public Pass {
public:
    explicit Dominators(Module *m) : Pass(m) {}
    ~Dominators() = default;
    ;
    void run() override;
    void create_doms(Function *f);
    void create_reverse_post_order(Function *f);
    void create_idom(Function *f);
    void create_dominance_frontier(Function *f);
    void create_dom_tree_succ(Function *f);

    // for debug
    void print_idom(Function *f);
    void print_dominance_frontier(Function *f);

    /****************api about Dominator****************/

    void add_dom(BasicBlock *bb, BasicBlock *dom_bb) { doms_[bb].insert(dom_bb); }
    std::set<BasicBlock *> &get_doms(BasicBlock *bb) { return doms_[bb]; }
    void set_doms(BasicBlock *bb, std::set<BasicBlock *> &doms) {
        doms_[bb].clear();
        doms_[bb].insert(doms.begin(), doms.end());
    }

    BasicBlock *get_idom(BasicBlock *bb) { return idom_[bb]; }
    void set_idom(BasicBlock *bb, BasicBlock *idom) { idom_[bb] = idom; }

    void add_dominance_frontier(BasicBlock *bb, BasicBlock *dom_frontier_bb) {
        dom_frontier_[bb].insert(dom_frontier_bb);
    }
    std::set<BasicBlock *> &get_dominance_frontier(BasicBlock *bb) { return dom_frontier_[bb]; }
    void set_dominance_frontier(BasicBlock *bb, std::set<BasicBlock *> &df) {
        dom_frontier_[bb].clear();
        dom_frontier_[bb].insert(df.begin(), df.end());
    }

    // successor blocks of this node in dominance tree
    std::set<BasicBlock *> get_dom_tree_succ_blocks(BasicBlock *bb) { return dom_tree_succ_blocks_[bb]; }
    void add_dom_tree_succ_block(BasicBlock *bb, BasicBlock *dom_tree_succ_bb) {
        dom_tree_succ_blocks_[bb].insert(dom_tree_succ_bb);
    }
    /****************api about Dominator****************/

private:
    void post_order_visit(BasicBlock *bb, std::set<BasicBlock *> &visited);
    BasicBlock *intersect(BasicBlock *b1, BasicBlock *b2);

    std::list<BasicBlock *> reverse_post_order_;
    std::map<BasicBlock *, int> post_order_id_; // the root has highest ID

    std::map<BasicBlock *, std::set<BasicBlock *>> doms_; // dominance set
    std::map<BasicBlock *, BasicBlock *> idom_; // immediate dominance
    std::map<BasicBlock *, std::set<BasicBlock *>> dom_frontier_; // dominance frontier set
    std::map<BasicBlock *, std::set<BasicBlock *>> dom_tree_succ_blocks_;
};

struct CFGnode {
    std::unordered_set<CFGnode *> succs;
    std::unordered_set<CFGnode *> preds;
    BasicBlock *BB;
    int index;
    int lowlink;
    int onStack;
};

using BBset_t = std::unordered_set<BasicBlock *>;

class LoopFind : public Pass {
    std::unordered_set<BBset_t *> loops;
    std::unordered_map<BasicBlock *, BBset_t *> base2Loop;
    std::unordered_map<BBset_t *, BasicBlock *> loop2Base;
    std::unordered_map<BasicBlock *, BasicBlock *> BB2Base; // Default to map lowest BB.

    int indexCount{};
    std::vector<CFGnode *> stack;

    static void buildCFG(Function *func, std::unordered_set<CFGnode *> &);

    static CFGnode *findLoopBase(std::unordered_set<CFGnode *> *, std::unordered_set<CFGnode *> &);

    bool stronglyConnComponents(std::unordered_set<CFGnode *> &, std::unordered_set<std::unordered_set<CFGnode *> *> &);

    void traverse(CFGnode *, std::unordered_set<std::unordered_set<CFGnode *> *> &);

    std::string _printCFG(std::unordered_set<CFGnode *> &);

public:
    explicit LoopFind(Module *m) : Pass(m) {}

    LoopFind(const LoopFind &) = delete;

    LoopFind &operator=(const LoopFind &) = delete;

    ~LoopFind();

    BBset_t *getBaseLoop(BasicBlock *BB);

    BasicBlock *getLoopCond(BBset_t *set);

    BBset_t *getParentLoop(BBset_t *set);

    BasicBlock *getLoopOut(BBset_t *set);

    BasicBlock *getLoopEntry(BBset_t *set) { return getLoopCond(set); }

    bool isBaseLoop(BBset_t *);

    auto begin() { return loops.begin(); }

    auto end() { return loops.end(); }

    void run() override;
};

class LoopInvariant : public Pass {
    std::vector<std::pair<BasicBlock *, std::list<Instruction *>::iterator>> invariant;
    std::unique_ptr<LoopFind> finder;
    int areaCount{};

    // std::unordered_set<Value*> definedInLoop;
    void findInvariants(BBset_t *);

    void buildPrehead();

    void appendTo();

    void moveInvariantsOut(BBset_t *);

    bool definedOut();

    bool definedIn_invariant();

    BasicBlock *buildBB();

public:
    explicit LoopInvariant(Module *m);
    void run() override;
};

class ActiveVars : public Pass {
public:
    explicit ActiveVars(Module *m) : Pass(m) {}
    void run() override;
    std::string print();

private:
    Function *func_{};
    std::map<BasicBlock *, std::set<Value *>> live_in, live_out, def, use, tempin, tempout;
    std::set<Value *> alldef, alluse;
    bool need_iterate{};
    std::map<BasicBlock *, std::set<Value *>> maptouse, maptodefine;
    std::map<BasicBlock *, std::map<Value *, BasicBlock *>> allphi;
};

class ConstFolder {
public:
    explicit ConstFolder(Module *m) : module_(m) {}
    ConstantInt *compute(Instruction::OpID op, ConstantInt *value1, ConstantInt *value2);
    ConstantInt *compute(CmpInst::CmpOp op, ConstantInt *v1, ConstantInt *v2);

private:
    Module *module_;
};

class ConstPropagation : public Pass {
public:
    explicit ConstPropagation(Module *m) : Pass(m) {}
    void run() override;
};

class Mem2Reg : public Transform {
private:
    Function *func_{};
    Dominators *dominators_{};

public:
    explicit Mem2Reg(Module *m) : Transform(m) {}
    ~Mem2Reg() = default;
    ;
    void run() override;
    void generate_phi();
    void re_name(BasicBlock *bb);
    void remove_alloca();
};

/**
 * case 1:
 * a:[int] b:[int] c[int]  // int is i32 in riscv32
 * for i in [1,2,..N]:
 *     a[i] = b[i] + c[i]
 *            ||
 *            v
 * for i in [1,2,..N/vector_num]:
 *     a[i+0] = b[i+0] + c[i+0]
 *     a[i+1] = b[i+1] + c[i+1]
 *              ...
 *     a[i+3] = b[i+vector_num-1] + c[i+vector_num-1]
 *            ||
 *            v
 * for i in [1,2,..N/vector_num]:
 *      a[i..i+vector_num-1] = b[i..i+vector_num-1] + c[i..i+vector_num-1]
 *
 * 1. a b c can be the same
 * 2. either of them can be vector constant.
 * 3. the minimized case can be
 * for i in [1,2,..N]:
 *       a[i] = a[i] + 4
 *            ||
 *            v
 * %1 = load <4 x i32>, <4 x i32>* bitcast ([4 x i32]* @a to <4 x i32>*), align 16, !tbaa !4
 * %2 = add nsw <4 x i32> %1, <i32 1, i32 1, i32 1, i32 1>
 * store <4 x i32> %2, <4 x i32>* bitcast ([4 x i32]* @a to <4 x i32>*), align 16, !tbaa !4
 * %3 = extractelement <4 x i32> %2, i64 0
 *            ||
 *            v
 * lui     s1, %hi(a)
 * addi    a0, s1, %lo(a)
 * vsetivli        zero, 4, e32, mf2, ta, mu
 * vle32.v v8, (a0)
 * vadd.vi v8, v8, 1
 * vse32.v v8, (a0)
 * vmv.x.s a1, v8
 * a:
 * .word   1                               # 0x1
 * .word   2                               # 0x2
 * .word   3                               # 0x3
 * .word   4                               # 0x4
 * .size   a, 16
 *
 * case 2:
 * a,b,c,d,e,f,g,h,x:int
 * if x>0:
 *      a = e + 4
 *      b = f + 4
 *      c = g + 4
 *      d = h + 4
 *            ||
 *            v
 * if x>0:
 *      {a,b,c,d} = {e,f,g,h} + {4,4,4,4}
 *            ||
 *            v
 * %1 = load <4 x i32>, <4 x i32>* bitcast ([4 x i32]* @a to <4 x i32>*), align 16, !tbaa !4
 * %2 = add nsw <4 x i32> %1, <i32 1, i32 1, i32 1, i32 1>
 * store <4 x i32> %2, <4 x i32>* bitcast ([4 x i32]* @a to <4 x i32>*), align 16, !tbaa !4
 * %3 = extractelement <4 x i32> %2, i64 0
 */
class Vectorization : public Transform {
public:
    const int MIN_LOOP_ITERS = 8;
    int vectorize_num = 4;

public:
    explicit Vectorization(Module *m) : Transform(m) {}
    void run() override;
    bool tryVectorizate(BasicBlock *cond, BasicBlock *body);
    void vectorizate(std::tuple<Value *, Value *, Value *> vector_value, Instruction *op, Instruction *iter) const;

private:
    int vectorizate_loops = 0;
};

class Multithread : public Pass {
public:
    int thread_num = 8;
    std::unique_ptr<LoopFind> finder;
    Value *start{}, *end{};
    Instruction *accu{};
    Instruction *comparer{};
    Instruction *phi{};
    int accuValue = -1;
    int multithreadCount = 0;
    bool findAccumulator(BBset_t *);
    void wrapMultithreading(BasicBlock *loopBase, BBset_t *loop);

public:
    explicit Multithread(Module *m) : Pass(m) {}
    ~Multithread() = default;
    void run() override;
};
} // namespace lightir

#endif // CHOCOPY_COMPILER_CHOCOPY_OPTIMIZATION_HPP
