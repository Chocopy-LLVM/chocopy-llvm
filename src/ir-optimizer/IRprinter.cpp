//
// Created by yiwei yang on 2/16/21.
//

#include "IRprinter.hpp"
#include <regex>
const std::regex to_const_replace("(.+?)+ ");

namespace lightir {

string print_as_op(Value *v, bool print_ty, const string &method_) {
    string op_ir;
    if (print_ty && !dynamic_cast<BasicBlock *>(v)) {
        op_ir += v->get_type()->print();
        op_ir += " ";
    }

    if (dynamic_cast<GlobalVariable *>(v)) {
        op_ir += "@" + v->get_name();
    } else if (dynamic_cast<Function *>(v)) {
        if (method_.empty()) {
            op_ir += "@" + v->get_name();
        } else {
            op_ir += "@" + method_ + "." + v->get_name();
        }
    } else if (dynamic_cast<ConstantStr *>(v)) {
        op_ir += v->print();
    } else if (dynamic_cast<Constant *>(v)) {
        op_ir += std::regex_replace(v->print(), to_const_replace, "");
    } else if(dynamic_cast<Class*>(v) && dynamic_cast<Class*>(v)->is_class_anon()) {
        op_ir += "%arg0";
    }else{
        op_ir += "%" + v->get_name();
    }

    return op_ir;
}

string print_cmp_type(CmpInst::CmpOp op) {
    switch (op) {
    case CmpInst::GE:
        return "sge";
    case CmpInst::GT:
        return "sgt";
    case CmpInst::LE:
        return "sle";
    case CmpInst::LT:
        return "slt";
    case CmpInst::EQ:
        return "eq";
    case CmpInst::NE:
        return "ne";
    default:
        break;
    }
    return "wrong cmpop";
}
} // namespace lightir