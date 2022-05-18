#include "Value.hpp"
#include "Type.hpp"
#include "User.hpp"
#include <cassert>
#include <utility>
namespace lightir {

Value::Value(Type *ty, std::string name) : type_(ty), name_(std::move(name)) {}

void Value::add_use(Value *val, unsigned arg_no) { use_list_.emplace_back(val, arg_no); }

std::string Value::get_name() { return name_; }

void Value::replace_all_use_with(Value *new_val) {
    for (auto use : use_list_) {
        auto val = dynamic_cast<User *>(use.val_);
        assert(val && "new_val is not a user");
        val->set_operand(use.arg_no_, new_val);
    }
}

void Value::remove_use(Value *val) {
    auto is_val = [val](const Use &use) { return use.val_ == val; };
    use_list_.remove_if(is_val);
}
} // namespace lightir