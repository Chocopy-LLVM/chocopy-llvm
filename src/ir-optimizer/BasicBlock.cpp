#include "BasicBlock.hpp"
#include "Function.hpp"
#include "Module.hpp"
#include <cassert>

#include "IRprinter.hpp"

namespace lightir {
BasicBlock::BasicBlock(Module *m, const string &name = "", Function *parent = nullptr)
    : Value(Type::get_label_type(m), name), parent_(parent) {
    assert(parent && "currently parent should not be nullptr");
    parent_->add_basic_block(this);
}

Module *BasicBlock::get_module() { return get_parent()->get_parent(); }

void BasicBlock::add_instruction(Instruction *instr) { instr_list_.push_back(instr); }

void BasicBlock::add_instr_begin(Instruction *instr) { instr_list_.push_front(instr); }

void BasicBlock::insert_instr(Instruction *pos, Instruction *insert) {
    insert->set_parent(pos->get_parent());
    for (auto instr = instr_list_.begin(); instr != instr_list_.end(); instr++) {
        if (*instr == pos) {
            instr_list_.insert(instr, insert);
            return;
        }
    }
}

void BasicBlock::delete_instr(Instruction *instr) {
    instr_list_.remove(instr);
    instr->remove_use_of_ops();
}

const Instruction *BasicBlock::get_terminator() const {
    if (instr_list_.empty()) {
        return nullptr;
    }
    switch (instr_list_.back()->get_instr_type()) {
    case Instruction::Ret:
    case Instruction::Br:
        return instr_list_.back();

    default:
        return nullptr;
    }
}

void BasicBlock::erase_from_parent() { this->get_parent()->remove(this); }

string BasicBlock::print() {
    string bb_ir;
    bb_ir += "\n";
    bb_ir += this->get_name();
    bb_ir += ":";
    /** print prebb */
    if (!this->get_pre_basic_blocks().empty()) {
        bb_ir += fmt::format("{:<48}; preds = ", "");
    }
    for (auto bb : this->get_pre_basic_blocks()) {
        if (bb != *this->get_pre_basic_blocks().begin())
            bb_ir += ", ";
        bb_ir += print_as_op(bb, false);
    }

    /** print pre-basic block */
    if (!this->get_parent()) {
        bb_ir += "\n";
        bb_ir += "; Error: Block without parent!";
    }
    bb_ir += "\n";
    for (auto instr : this->get_instructions()) {
        bb_ir += "  ";
        bb_ir += instr->print();
        bb_ir += "\n";
    }

    return bb_ir;
}
} // namespace lightir