#include "User.hpp"
#include <cassert>
#include <chocopy_logging.hpp>

namespace lightir {

User::User(Type *ty, const std::string &name, unsigned num_ops) : Value(ty, name), num_ops_(num_ops) {
    operands_.resize(num_ops_, nullptr);
}

std::vector<Value *> &User::get_operands() { return operands_; }

Value *User::get_operand(unsigned i) const { return operands_[i]; }

void User::set_operand(unsigned i, Value *v) {
    assert(i < num_ops_ && "set_operand out of index");
    operands_[i] = v;
    v->add_use(this, i);
}

void User::add_operand(Value *v) {
    operands_.push_back(v);
    v->add_use(this, num_ops_);
    num_ops_++;
}

unsigned User::get_num_operand() const { return num_ops_; }

void User::remove_use_of_ops() {
    for (auto op : operands_) {
        op->remove_use(this);
    }
}

void User::remove_operands(int index1, int index2) {
    for (int i = index1; i <= index2; i++) {
        operands_[i]->remove_use(this);
    }
    operands_.erase(operands_.begin() + index1, operands_.begin() + index2 + 1);
    LOG(DEBUG) << operands_.size();
    num_ops_ = operands_.size();
}
} // namespace lightir