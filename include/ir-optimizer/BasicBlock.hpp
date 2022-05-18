//
// Created by yiwei yang on 2/15/21.
//

#ifndef CHOCOPY_COMPILER_BASICBLOCK_HPP
#define CHOCOPY_COMPILER_BASICBLOCK_HPP

#pragma once

#include "Function.hpp"
#include "Module.hpp"
#include "Value.hpp"
#include <list>
#include <set>
#include <string>

using std::map;
using std::string;
using std::vector;
namespace lightir {
class Instruction;

class BasicBlock : public Value {
public:
    BasicBlock(Module *m, const string &name, Function *parent);
    /** Constructor that calls name completion. */
    static BasicBlock *create(Module *m, const string &name, Function *parent) {
        auto prefix = name.empty() ? "" : "label_";
        return new BasicBlock(m, prefix + name, parent);
    }

    Function *get_parent() { return parent_; }

    Module *get_module();

    /** API for Control Flow Graph. */
    list<BasicBlock *> &get_pre_basic_blocks() { return pre_bbs_; }
    list<BasicBlock *> &get_succ_basic_blocks() { return succ_bbs_; }
    void add_pre_basic_block(BasicBlock *bb) { pre_bbs_.push_back(bb); }
    void add_succ_basic_block(BasicBlock *bb) { succ_bbs_.push_back(bb); }

    void remove_pre_basic_block(BasicBlock *bb) { pre_bbs_.remove(bb); }
    void remove_succ_basic_block(BasicBlock *bb) { succ_bbs_.remove(bb); }

    /** Return the terminator instruction if the block is well formed or null, if the block is not well formed. */
    const Instruction *get_terminator() const;
    Instruction *get_terminator() {
        return const_cast<Instruction *>(static_cast<const BasicBlock *>(this)->get_terminator());
    }

    void add_instruction(Instruction *instr);
    void add_instr_begin(Instruction *instr);
    void delete_instr(Instruction *instr);
    void insert_instr(Instruction *pos, Instruction *insert);
    void set_mark(parser::AccOptions::AccType mar) { marked = mar; };

    bool empty() { return instr_list_.empty(); }
    int get_num_of_instr() { return instr_list_.size(); }
    list<Instruction *> &get_instructions() { return instr_list_; }

    void erase_from_parent();

    virtual string print() override;

private:
    list<BasicBlock *> pre_bbs_;
    list<BasicBlock *> succ_bbs_;
    list<Instruction *> instr_list_;
    Function *parent_;
    parser::AccOptions::AccType marked{};
};
} // namespace lightir
#endif // CHOCOPY_COMPILER_BASICBLOCK_HPP
