//
// Created by yiwei yang on 2/16/21.
//

#ifndef CHOCOPY_COMPILER_IRPRINTER_HPP
#define CHOCOPY_COMPILER_IRPRINTER_HPP

#include "BasicBlock.hpp"
#include "Constant.hpp"
#include "Function.hpp"
#include "GlobalVariable.hpp"
#include "Module.hpp"
#include "Type.hpp"
#include "User.hpp"
#include "Value.hpp"
#include <chocopy_logging.hpp>
#include <fmt/format.h>

namespace lightir {

string print_as_op(Value *v, bool print_ty, const string &method_ = "");
string print_cmp_type(lightir::CmpInst::CmpOp op);

} // namespace lightir

#endif // CHOCOPY_COMPILER_IRPRINTER_HPP
