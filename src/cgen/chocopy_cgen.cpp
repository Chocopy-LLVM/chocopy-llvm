#include <chocopy_cgen.hpp>
#include <chocopy_lightir.hpp>
#if __cplusplus > 202000L && !defined(__clang__)
#include <ranges>
#endif
#include <regex>
#include <utility>

/** From String to codegen, care about class */
#ifdef LLVM
using namespace llvm;
extern Module chocopy_m;
#else
using namespace lightir;
#endif

namespace cgen {
const std::regex to_replace("0A(\t|)");

string_view InstGen::Addr::get_name() const {
    if (str.empty())
        return fmt::format("[{}, #{}]", reg.get_name(), std::to_string(this->offset));
    else
        return str;
}

string InstGen::set_value(const Reg &target, const Constant &source) {
    string asm_code;
    /** switch different instruction to load a word constant */
    return asm_code;
};
string InstGen::instConst(string (*inst)(const Reg &, const Reg &, const Value &, string), const InstGen::Reg &target,
                          const InstGen::Reg &op1, const InstGen::Constant &op2) {
    string asm_code;
    int val = op2.getValue();
    if (target == op1 && op2.getValue() == 0 && (inst == RiscVBackEnd::emit_add || inst == RiscVBackEnd::emit_sub)) {
        return asm_code;
    } else if (0 <= val && val <= imm_8_max) {
        asm_code += inst(target, op1, op2, "");
    } else {
        asm_code += set_value(InstGen::Reg(5), op2);
        asm_code += inst(target, op1, InstGen::Reg(5), "");
    }
    return asm_code;
}
string InstGen::instConst(string (*inst)(const Reg &, const Reg &, const Reg &, string), const InstGen::Reg &target,
                          const InstGen::Reg &op1, const InstGen::Constant &op2) {
    string asm_code;
    int val = op2.getValue();
    if (target == op1 && op2.getValue() == 0 &&
        (inst == RiscVBackEnd::emit_sll || inst == RiscVBackEnd::emit_srl || inst == RiscVBackEnd::emit_or)) {
        return asm_code;
    } else {
        asm_code += set_value(InstGen::Reg(5), op2);
        asm_code += inst(target, op1, InstGen::Reg(5), "");
    }
    return asm_code;
}

string InstGen::instConst(string (*inst)(const Reg &, const Reg &, int, string), const InstGen::Reg &target,
                          const InstGen::Reg &op1, const InstGen::Constant &op2) {
    string asm_code;
    int val = op2.getValue();
    if (target == op1 && op2.getValue() == 0 &&
        (inst == RiscVBackEnd::emit_slli || inst == RiscVBackEnd::emit_srli || inst == RiscVBackEnd::emit_ori)) {
        return asm_code;
    } else { // imm
        asm_code += inst(target, op1, val, "");
    }
    return asm_code;
}

string InstGen::instConst(string (*inst)(const Reg &, const Value &, string), const InstGen::Reg &op1,
                          const InstGen::Constant &op2) {
    string asm_code;
    int val = op2.getValue();
    if (0 <= val && val <= imm_8_max) {
        asm_code += inst(op1, op2, "");
    } else {
        asm_code += set_value(InstGen::Reg(5), op2);
        asm_code += inst(op1, InstGen::Reg(5), "");
    }
    return asm_code;
}

string CodeGen::generateModuleCode(map<Value *, int> rm) {
    string asm_code;
    this->register_mapping = std::move(rm);

    /** Symbolic assembler constants defined here (to add others, override
     * initAsmConstants in an extension of CodeGenBase):
     * ecalls:
     *   @sbrk
     *   @fill_line_buffer
     *   @read_string
     *   @print_string
     *   @print_char
     *   @print_int
     *   @exit2
     * Exit codes:
     *   @error_div_zero: Division by 0.
     *   @error_arg: Bad argument.
     *   @error_oob: Out of bounds.
     *   @error_none: Attempt to access attribute of None.
     *   @error_oom: Out of memory.
     *   @error_nyi: Unimplemented operation.
     * Data-structure byte offsets:
     *   @.__obj_size__: Offset of size of object.
     *   @.__len__: Offset of length in chars or words.
     *   @.__str__: Offset of string data.
     *   @.__elts__: Offset of first list item.
     *   @.__int__: Offset of integer value.
     *   @.__bool__: Offset of boolean (1/0) value.
     */

    /** Define @-constants to be used in assembly code. */
#ifdef EMIT
    asm_code += backend->defineSym("sbrk", backend->SBRK_ECALL);
    asm_code += backend->defineSym("print_string", backend->PRINT_STRING_ECALL);
    asm_code += backend->defineSym("print_char", backend->PRINT_CHAR_ECALL);
    asm_code += backend->defineSym("print_int", backend->PRINT_INT_ECALL);
    asm_code += backend->defineSym("exit2", backend->EXIT2_ECALL);
    asm_code += backend->defineSym("read_string", backend->READ_STRING_ECALL);
    asm_code += backend->defineSym("fill_line_buffer", backend->FILL_LINE_BUFFER__ECALL);

    asm_code += backend->defineSym(".__obj_size__", 4);
    asm_code += backend->defineSym(".__len__", 12);
    asm_code += backend->defineSym(".__int__", 12);
    asm_code += backend->defineSym(".__bool__", 12);
    asm_code += backend->defineSym(".__str__", 16);
    asm_code += backend->defineSym(".__elts__", 16);

    asm_code += backend->defineSym("error_div_zero", backend->ERROR_DIV_ZERO);
    asm_code += backend->defineSym("error_arg", backend->ERROR_ARG);
    asm_code += backend->defineSym("error_oob", backend->ERROR_OOB);
    asm_code += backend->defineSym("error_none", backend->ERROR_NONE);
    asm_code += backend->defineSym("error_oom", backend->ERROR_OOM);
    asm_code += backend->defineSym("error_nyi", backend->ERROR_NYI);
#endif
    asm_code = replaceStringLiteral(asm_code);

    for (auto &classInfo : this->module->get_class()) {
        backend->emit_prototype(*classInfo);
    }

    asm_code += generateHeaderCode();

    asm_code += CodeGen::generateGOT();

    for (auto &func : this->module->get_functions()) {
        if (func->get_basic_blocks().size()) {
            asm_code += CodeGen::generateFunctionCode(func);
        }
    }
    asm_code += backend->start_code();
    asm_code += generateClassCode();
    asm_code += generateGlobalVarsCode();

    asm_code += generateFooterCode();

    return asm_code;
}

string CodeGen::generateModuleCode(bool autoAlloc) {
    return generateModuleCode(autoAlloc ? regAlloc() : map<Value *, int>());
}

std::map<Value *, int> CodeGen::regAlloc() {
/* The regAlloc algorithm Your Code Here */
}

string CodeGen::generateFunctionCode(Function *func) {
    string asm_code;

    return asm_code;
}

void CodeGen::allocateStackSpace(Function *func) {
}

CodeGen::CodeGen(shared_ptr<Module> module) : module(move(module)), backend(new RiscVBackEnd()) {}

string CodeGen::generateFunctionEntryCode(Function *func) {
    string asm_code;
    asm_code += CodeGen::getLabelName(func, 0) + ":";
    asm_code += CodeGen::comment("function preprocess");
    return asm_code;
}
string CodeGen::generateFunctionExitCode(Function *func) {
    std::string asm_code;
    return asm_code;
}

string CodeGen::generateClassCode() {
    string class_asm;
    return class_asm;
}

string CodeGen::generateHeaderCode() {
    string asm_code;
    return asm_code;
}
string CodeGen::generateFooterCode() {
    string asm_code;
    return asm_code;
}
string CodeGen::generateFunctionPostCode(Function *func) {
    std::string asm_code;
    auto tmp_asm_code = CodeGen::getLabelName(func, 1) + ":";
    asm_code += CodeGen::comment(tmp_asm_code, "function postcode");
    return asm_code;
}
string CodeGen::generateBasicBlockCode(BasicBlock *bb) {
    std::string asm_code;
    for (auto &inst : bb->get_instructions()) {
        asm_code += CodeGen::generateInstructionCode(inst);
    }
    return asm_code;
}
string CodeGen::generateInstructionCode(Instruction *inst) {
    std::string asm_code;
    auto &ops = inst->get_operands();
    switch (inst->get_instr_type()) {
        case lightir::Instruction::OpID::Ret:
        case lightir::Instruction::OpID::Br:
        case lightir::Instruction::OpID::Neg:
        case lightir::Instruction::OpID::Not:
        case lightir::Instruction::OpID::Add:
        case lightir::Instruction::OpID::Sub:
        case lightir::Instruction::OpID::Mul:
        case lightir::Instruction::OpID::Div:
        case lightir::Instruction::OpID::Rem:
        case lightir::Instruction::OpID::And:
        case lightir::Instruction::OpID::Or:
        case lightir::Instruction::OpID::Alloca:
        case lightir::Instruction::OpID::Load:
        case lightir::Instruction::OpID::Store:
        case lightir::Instruction::OpID::Shl:
        case lightir::Instruction::OpID::AShr:
        case lightir::Instruction::OpID::LShr:
        case lightir::Instruction::OpID::ICmp:
        case lightir::Instruction::OpID::PHI:
        case lightir::Instruction::OpID::Call:
        case lightir::Instruction::OpID::GEP:
        case lightir::Instruction::OpID::ZExt:
        case lightir::Instruction::OpID::InElem:
        case lightir::Instruction::OpID::ExElem:
        case lightir::Instruction::OpID::BitCast:
        case lightir::Instruction::OpID::Trunc:
        case lightir::Instruction::OpID::VExt:
        case lightir::Instruction::OpID::ASM:
        case lightir::Instruction::OpID::ACCSTART:
        case lightir::Instruction::OpID::ACCEND:
            break;
    }
    return asm_code;
}
string CodeGen::getLabelName(BasicBlock *bb) { return "." + bb->get_parent()->get_name() + "_" + bb->get_name(); }
string CodeGen::getLabelName(Function *func, int type) {
    const std::vector<std::string> name_list = {"pre", "post"};
    return "." + func->get_name() + "_" + name_list.at(type);
}
/* The CGen Your Code Here */
string CodeGen::generateFunctionCall(Instruction *inst, const string &func_name, vector<Value *> ops, int return_reg,
                                     int sp_ofs) {
    std::string asm_code;
    return asm_code;
}
vector<InstGen::Reg> CodeGen::getAllRegisters(Function *func) {
    std::set<InstGen::Reg> registers;
    for (auto &arg : func->get_args()) {
        if (this->register_mapping.count(arg) && this->register_mapping.at(arg) <= InstGen::max_reg_id) {
            registers.insert(InstGen::Reg(this->register_mapping.at(arg)));
        }
    }
    for (auto &bb : func->get_basic_blocks()) {
        for (auto &inst : bb->get_instructions()) {
            if (this->register_mapping.count(inst) && this->register_mapping.at(inst) <= InstGen::max_reg_id) {
                registers.insert(InstGen::Reg(this->register_mapping.at(inst)));
            }
        }
    }
    for (auto &reg : temp_regs) { // used as temp regs
        registers.insert(reg);
    }
    return std::vector<InstGen::Reg>(registers.begin(), registers.end());
}
vector<InstGen::Reg> CodeGen::getCallerSaveRegisters(Function *func) {
    std::set<InstGen::Reg> registers;
    for (auto &reg : CodeGen::getAllRegisters(func)) {
        if (std::count(caller_save_regs.begin(), caller_save_regs.end(), reg) &&
            !std::count(temp_regs.begin(), temp_regs.end(), reg)) {
            registers.insert(reg);
        }
    }
    return std::vector<InstGen::Reg>(registers.begin(), registers.end());
}
vector<InstGen::Reg> CodeGen::getCalleeSaveRegisters(Function *func) {
    std::set<InstGen::Reg> registers;
    for (auto &reg : CodeGen::getAllRegisters(func)) {
        if (std::count(callee_save_regs.begin(), callee_save_regs.end(), reg)) {
            registers.insert(reg);
        }
    }
    return std::vector<InstGen::Reg>(registers.begin(), registers.end());
}
bool CodeGen::isSameMapping(Value *a, Value *b) {
    if (this->register_mapping.count(a) && this->register_mapping.count(b)) {
        return this->register_mapping.at(a) == this->register_mapping.at(b);
    }
    if (this->stack_mapping.count(a) && this->stack_mapping.count(b)) {
        return this->stack_mapping.at(a) == this->stack_mapping.at(b);
    }
    return false;
}

std::string CodeGen::virtualRegMove(Value *target, Value *source, int sp_ofs) {
    std::string asm_code;
    if (CodeGen::isSameMapping(target, source)) {
        return asm_code;
    }
    int alu_op0 = this->register_mapping.count(target) ? this->register_mapping.at(target) : op_reg_0;
    asm_code += CodeGen::assignToSpecificReg(source, alu_op0, sp_ofs);
    asm_code += CodeGen::getFromSpecificReg(target, alu_op0, sp_ofs);
    return asm_code;
}
string CodeGen::assignToSpecificReg(Value *val, int target, int sp_ofs) {
    std::string asm_code;
    auto val_const = dynamic_cast<ConstantInt *>(val);
    auto val_global = dynamic_cast<GlobalVariable *>(val);
    if (val_const) {
        int imm = val_const->get_value();
        asm_code += InstGen::set_value(InstGen::Reg(target), InstGen::Constant(imm));
    } else if (val_global) {
        asm_code +=
            //                        InstGen::get_address(InstGen::Reg(op_reg_2), InstGen::Label(".global",
            //                        CodeGen::queryGOT(val_global) * 4));
            asm_code += backend->emit_lw(InstGen::Reg(target), InstGen::Addr(InstGen::Reg(op_reg_2), 0));
    } else if (register_mapping.count(val) && register_mapping.at(val) <= InstGen::max_reg_id) {
        auto source = register_mapping.at(val);
        asm_code += backend->emit_mv(InstGen::Reg(target), InstGen::Reg(source));
    } else if (allocated.count(val)) {
        auto offset = stack_mapping.at(val) + sp_ofs;
        asm_code += InstGen::instConst(backend->emit_add, InstGen::Reg(target), InstGen::Reg("sp"), InstGen::Constant(offset));
    } else if (stack_mapping.count(val)) {
        auto offset = stack_mapping.at(val) + sp_ofs;
        asm_code += backend->emit_lw(InstGen::Reg(target), InstGen::Addr(InstGen::Reg("sp"), offset));
    } else {
        LOG(ERROR) << "Function assignToSpecificReg exception!";
        exit(EXIT_FAILURE);
    }
    return asm_code;
}
string CodeGen::getFromSpecificReg(Value *val, int source, int sp_ofs) {
    std::string asm_code;
    return asm_code;
}
string CodeGen::generateGOT() {
    string asm_code;
    this->GOT.clear();
    for (auto &global_var : this->module->get_global_variable()) {
        if (global_var->init_val_ != nullptr) {
            int count = this->GOT.size();
            if (!GOT.count(global_var)) {
                this->GOT[global_var] = count;
            }
        }
    }
    std::vector<Value *> vecGOT;
    vecGOT.resize(this->GOT.size());
    for (auto &i : GOT) {
        vecGOT[i.second] = i.first;
    }
    for (auto &i : vecGOT) {
        asm_code += ".globl " + i->get_name() + "\n";
    }
    for (auto &i : vecGOT) {
        asm_code += ".long " + i->get_name() + "\n";
    }
    return asm_code;
}
string CodeGen::generateGlobalVarsCode() {
    string asm_code;
    for (auto &global_var : this->module->get_global_variable()) {
        if (global_var->init_val_ != nullptr) {
            asm_code += global_var->get_name() + ":\n";
            if (reinterpret_cast<Type *>(!global_var->get_type()->get_ptr_element_type()) ==
                global_var->get_operands().at(0)->get_type()) {
                asm_code += "  .zero " + std::to_string(global_var->get_type()->get_size()) + "\n";
            } else {
                asm_code +=
                    CodeGen::generateInitializerCode(dynamic_cast<Constant *>(global_var->get_operands().at(0)));
            }
        }
    }
    return asm_code;
}
string CodeGen::generateInitializerCode(Constant *init) {
    string asm_code;
    auto array_init = dynamic_cast<ConstantArray *>(init);
    auto str_init = dynamic_cast<ConstantStr *>(init);
    if (array_init) {
        auto length = dynamic_cast<ArrayType *>(array_init->get_type())->get_num_of_elements();
        for (int i = 0; i < length; i++) {
            asm_code += CodeGen::generateInitializerCode(array_init->get_element_value(i));
        }
    } else if (str_init) {
        //        CodeGen::generate
        string str = str_init->get_value();
        asm_code += "  .word " + std::to_string(str_init->get_type()->get_type_id()) + "\n";
        size_t objsize = str.size() / 4 + 5;
        asm_code += "  .word " + std::to_string(objsize) += "\n";
        asm_code += "  .word $str$dispatchTable\n";
        asm_code += "  .word " + std::to_string(str.size()) += "\n";
        asm_code += "  .string \"" + str += "\"\n";
        asm_code += "  .align 2\n";
        asm_code += "\n";

    } else {
        auto val = CodeGen::getConstIntVal(init);
        if (!val.second) {
            LOG(ERROR) << "function generateInitializerCode exception!";
            exit(EXIT_FAILURE);
        }
        asm_code += "  .long " + std::to_string(val.first) + "\n";
    }
    return asm_code;
}
pair<int, bool> CodeGen::getConstIntVal(Value *val) {
    auto const_val = dynamic_cast<ConstantInt *>(val);
    auto inst_val = dynamic_cast<Instruction *>(val);
    if (const_val) {
        return std::make_pair(const_val->get_value(), true);
    } else if (inst_val) {
        auto op_list = inst_val->get_operands();
        if (dynamic_cast<BinaryInst *>(val)) {
            auto val_0 = CodeGen::getConstIntVal(op_list.at(0));
            auto val_1 = CodeGen::getConstIntVal(op_list.at(1));
            if (val_0.second && val_1.second) {
                int ret = 0;
                bool flag = true;
                switch (inst_val->get_instr_type()) {
                case Instruction::Add:
                    ret = val_0.first + val_1.first;
                    break;
                case Instruction::Sub:
                    ret = val_0.first - val_1.first;
                    break;
                case Instruction::And:
                    ret = val_0.first & val_1.first;
                    break;
                case Instruction::Or:
                    ret = val_0.first | val_1.first;
                    break;
                case Instruction::Mul:
                    ret = val_0.first * val_1.first;
                    break;
                case Instruction::Div:
                    ret = val_0.first / val_1.first;
                    break;
                case Instruction::Rem:
                    ret = val_0.first % val_1.first;
                    break;
                default:
                    flag = false;
                    break;
                }
                return std::make_pair(ret, flag);
            } else {
                return std::make_pair(0, false);
            }
        }
        if (dynamic_cast<UnaryInst *>(val)) {
            auto val_0 = CodeGen::getConstIntVal(op_list.at(0));
            if (val_0.second) {
                int ret = 0;
                bool flag = true;
                switch (inst_val->get_instr_type()) {
                case Instruction::Not:
                    ret = !val_0.first;
                    break;
                case Instruction::Neg:
                    ret = -val_0.first;
                    break;
                default:
                    flag = false;
                    break;
                }
                return std::make_pair(ret, flag);
            } else {
                return std::make_pair(0, false);
            }
        }
    }
    LOG(ERROR) << "Function getConstIntVal exception!";
    exit(EXIT_FAILURE);
}
int CodeGen::queryGOT(Value *val) { return this->GOT.at(val); }
string CodeGen::comment(const string &s) {
    std::string asm_code;
    asm_code += fmt::format("# {}\n", s);
    return asm_code;
}
string CodeGen::comment(const string &t, const string &s) {
    std::string asm_code;
    asm_code += fmt::format("{:<42}{:<42}\n", t, fmt::format("# {}", s));
    return asm_code;
}
string CodeGen::generateVext(int vlen, int elen, lightir::VExtInst::vv_type type, const InstGen::Reg &len) {
    static int counter;
    std::string asm_code;
    switch (type) {
    case VExtInst::vv_type::VADD:
    case VExtInst::vv_type::VLE:
    case VExtInst::vv_type::VSETVLI:
    case VExtInst::vv_type::VDIV:
    case VExtInst::vv_type::VREM:
    case VExtInst::vv_type::VMUL:
    case VExtInst::vv_type::VSE:
        break;
    }
    return asm_code;
};

RiscVBackEnd::RiscVBackEnd(int vlen, int vlmax) : vlen(vlen), vlmax(64) {
    /** emplace_back vector reg */
    for (int i = 0; i < vlen / word_size / 4; i++)
        reg_name.emplace_back(fmt::format("x{}", i));
}

string cgen::CodeGen::replaceStringLiteral(const string &str) {
    const string find_left("STRING[");
    const string find_right("]");
    string result(str);
    size_t find_len;
    size_t left_pos, right_pos, from = 0;
    while (string::npos != (left_pos = str.find(find_left, from)) &&
           string::npos != (right_pos = str.find(find_right, left_pos))) {
        find_len = right_pos - left_pos - find_left.size();
        auto const_str = str.substr(left_pos + find_left.size() + 1, find_len - 2);
        auto const_str_label = this->makeConstStr(const_str);
        result = replace_all(result, fmt::format("STRING[\"{}\"]", const_str), std::string(const_str_label.get_name()));
        from = right_pos;
    }
    return result;
}

InstGen::Addr cgen::CodeGen::makeConstStr(const string &str) {
    auto const_str = lightir::StringType::get(str, module.get());
    lightir::GlobalVariable::create(fmt::format("const_{}", module->get_global_variable().size()), module.get(),
                                    const_str, true,
                                    lightir::ConstantStr::get(str, module->get_global_variable().size(), module.get()));
    return {fmt::format("const_{}", module->get_global_variable().size() - 1)};
};

string CodeGen::virtualRegMove(vector<Value *> target, vector<Value *> source, int sp_ofs) {
    std::string asm_code;
    assert(target.size() == source.size());
    int sz = target.size();
    std::list<std::pair<Value *, Value *>> L;
    for (int i = 0; i < sz; i++) {
        L.emplace_back(target.at(i), source.at(i));
    }
    for (auto it = L.begin(); it != L.end(); it++) {
        for (auto it2 = L.begin(); it2 != L.end(); it2++) {
            if (it2 != it && CodeGen::isSameMapping(it2->first, it->first)) {
                LOG(ERROR) << "virtualRegMove race condition";
                exit(EXIT_FAILURE);
            }
        }
    }
    Value *tg_val = nullptr;
    while (!L.empty()) {
        bool flag = true;
        for (auto it = L.begin(); it != L.end(); it++) {
            bool ok = true;
            for (auto it2 = L.begin(); it2 != L.end(); it2++) {
                if (it2 != it && CodeGen::isSameMapping(it2->second, it->first)) {
                    ok = false;
                }
            }
            if (ok) {
                asm_code += CodeGen::virtualRegMove(it->first, it->second, sp_ofs);
                L.erase(it);
                flag = false;
                break;
            }
        }
        if (flag) {
            if (tg_val != nullptr) {
                asm_code += CodeGen::getFromSpecificReg(tg_val, op_reg_0, sp_ofs);
            }
            auto it = L.begin();
            asm_code += CodeGen::assignToSpecificReg(it->second, op_reg_0, sp_ofs);
            tg_val = it->first;
            L.erase(it);
        }
    }
    if (tg_val != nullptr) {
        asm_code += CodeGen::getFromSpecificReg(tg_val, op_reg_0, sp_ofs);
    }
    return asm_code;
}

} // namespace cgen

#ifdef PA4
int main(int argc, char *argv[]) {
    string target_path;
    string input_path;
    string IR;
    string asm_code;
    vector<string> passes;

    bool emit = false;
    bool run = false;
    bool assem = false;

    for (int i = 1; i < argc; ++i) {
        if (argv[i] == "-h"s || argv[i] == "--help"s) {
            print_help(argv[0]);
            return 0;
        } else if (argv[i] == "-o"s) {
            if (target_path.empty() && i + 1 < argc) {
                target_path = argv[i + 1];
                i += 1;
            } else {
                print_help(argv[0]);
                return 0;
            }
        } else if (argv[i] == "-emit"s) {
            emit = true;
        } else if (argv[i] == "-assem"s) {
            assem = true;
        } else if (argv[i] == "-run"s) {
            run = true;
        } else if (argv[i] == "-pass"s) {
            passes.push_back(argv[i + 1]);
            i += 1;
        } else {
            if (input_path.empty()) {
                input_path = argv[i];
                target_path = replace_all(input_path, ".py", "");
            } else {
                print_help(argv[0]);
                return 0;
            }
        }
    }

    parser::Program *tree = parse(input_path.c_str());

    auto error = new vector<parser::Err *>();

    auto *declarationAnalyzer = new semantic::DeclarationAnalyzer(error);

    tree->accept(*declarationAnalyzer);
    semantic::SymbolTable *globalScope = declarationAnalyzer->getGlobals();

    if (!error->empty()) {
        tree->add_error(error);
    } else {
        auto *typeChecker = new semantic::TypeChecker(globalScope, error);
        tree->accept(*typeChecker);
    }

    std::shared_ptr<lightir::Module> m;
    if (!error->empty()) {
        tree->add_error(error);
    } else {
        cJSON *a = tree->toJSON();
        char *out = cJSON_Print(a);
        LOG(INFO) << "ChocoPy Language Server:\n" << out << "\n";

        auto *LightWalker = new lightir::LightWalker(globalScope);
        tree->accept(*LightWalker);
        m = LightWalker->get_module();
        m->source_file_name_ = input_path;
        lightir::PassManager PM(m.get());
        for (auto &&pass : passes)
            PM.add_pass(pass);
        PM.run();

        IR = m->print();
        std::ofstream output_stream;
        auto output_file = target_path + ".ll";

        output_stream.open(output_file, std::ios::out);
        output_stream << fmt::format("; ModuleID = '{}'\n", m->module_name_);
        output_stream << "source_filename = \"" + m->source_file_name_ + "\"\n\n";
        output_stream << IR;
        output_stream.close();

        cgen::CodeGen code_generator(m);
        asm_code = code_generator.generateModuleCode(true);
        if (assem) {
            cout << "RiscV Asm:\n";

            cout << asm_code;
        }
        std::ofstream output_stream1;
        auto output_file1 = target_path + ".s";
        output_stream1.open(output_file1, std::ios::out);
        output_stream1 << asm_code;
        output_stream1.close();
        if (assem) {
            auto command_string = "cat " + target_path + ".s ";
            int re_code = std::system(command_string.c_str());
            LOG(INFO) << command_string << re_code;
        }
    }
#ifdef LLVM
    llvmGetPassPluginInfo(m);
#endif
    if (!emit) {
        cout << "\nLLVM IR:\n; ModuleID = 'chocopy'\nsource_filename = \"\"" << input_path << "\"\"\n\n" << IR;
    }

    if (run) {
#if defined(WIN32) || defined(_WIN32) || defined(__APPLE__)
#ifdef RV64
        auto command_string_0 = "riscv64-unknown-elf-gcc -mabi=lp64 -g -ggdb -static  -march=rv64imac -o" +
                                target_path + " " + target_path + ".s -L./ -lchocopy_stdlib";
#else
        auto command_string_0 = "riscv64-unknown-elf-gcc -mabi=ilp32 -g -ggdb -static -march=rv32imac -o" +
                                target_path + " " + target_path + ".s -L./ -lchocopy_stdlib";
#endif
        int re_code_0 = std::system(command_string_0.c_str());
        LOG(INFO) << command_string_0 << re_code_0;
#ifdef RV64
        auto command_string_1 = "spike --isa=rv64imac /opt/homebrew/Cellar/riscv-pk/master/bin/pk " + target_path;
#else
            auto command_string_1 =
                "spike --isa=rv32gcv0p10 /opt/homebrew/Cellar/riscv-pk-32/master/bin/pk " + target_path;
#endif
        int re_code_1 = std::system(command_string_1.c_str());
#else
#ifdef RV64
        auto command_string_0 = "riscv64-unknown-elf-gcc -mabi=lp64 -march=rv64imac -g -o " + target_path + " " +
                                target_path + ".s -L./ -lchocopy_stdlib";
#else
            auto command_string_0 =
                "riscv64-unknown-elf-gcc -mabi=ilp32 -march=rv32imac -g -o " + target_path + " " + target_path +
                                ".s -L./ -L/Users/yiweiyang/project/bak/cmake-build-debug-kali-gcc -lchocopy_stdlib";
#endif
        int re_code_0 = std::system(command_string_0.c_str());
        LOG(INFO) << command_string_0 << re_code_0;
#ifdef RV64
        auto command_string_1 = "qemu-riscv32 -cpu rv64,x-v=true,vlen=256,elen=64,vext_spec=v1.0 " + target_path;
#else
        auto command_string_1 = "qemu-riscv32 " + target_path;
#endif
        int re_code_1 = std::system(command_string_1.c_str());
#endif
        LOG(INFO) << command_string_1 << re_code_1;
    }
result:
    return 0;
}
#endif


#ifdef LLVM
llvm::PassPluginLibraryInfo llvmGetPassPluginInfo(const std::shared_ptr<Module> &m) {
    chocopy_m = m;
    return {LLVM_PLUGIN_API_VERSION, "ChoocoPy", "v0.1", [](llvm::PassBuilder &PB) {
                PB.registerPipelineParsingCallback([](llvm::StringRef, llvm::ModulePassManager &MPM,
                                                      llvm::ArrayRef<llvm::PassBuilder::PipelineElement>) {
                    MPM.addPass(cgen::CodeGen());
                    return true;
                });
            }};
}
#endif

#ifdef ALL
int main(int argc, char *argv[]) {
    string target_path;
    string input_path;
    string IR;
    string asm_code;
    vector<string> passes;

    bool O0 = false;
    bool O3 = false;
    bool assem = false;

    for (int i = 1; i < argc; ++i) {
        if (argv[i] == "-h"s || argv[i] == "--help"s) {
            print_help_all(argv[0]);
            return 0;
        } else if (argv[i] == "-o"s) {
            if (target_path.empty() && i + 1 < argc) {
                target_path = argv[i + 1];
                i += 1;
            } else {
                print_help(argv[0]);
                return 0;
            }
        } else if (argv[i] == "-O3"s) {
            O3 = true;
        } else if (argv[i] == "-O0"s) {
            O0 = true;
        } else if (argv[i] == "-assem"s) {
            assem = true;
        } else {
            if (input_path.empty()) {
                input_path = argv[i];
                target_path = replace_all(input_path, ".py", "");
            } else {
                print_help(argv[0]);
                return 0;
            }
        }
    }

    parser::Program *tree = parse(input_path.c_str());

    auto error = new vector<parser::Err *>();

    auto *declarationAnalyzer = new semantic::DeclarationAnalyzer(error);

    tree->accept(*declarationAnalyzer);
    semantic::SymbolTable *globalScope = declarationAnalyzer->getGlobals();

    if (!error->empty()) {
        tree->add_error(error);
    } else {
        auto *typeChecker = new semantic::TypeChecker(globalScope, error);
        tree->accept(*typeChecker);
    }

    std::shared_ptr<lightir::Module> m;
    if (!error->empty()) {
        tree->add_error(error);
    } else {
        cJSON *a = tree->toJSON();
        char *out = cJSON_Print(a);
        LOG(INFO) << "ChocoPy Language Server:\n" << out << "\n";

        auto *LightWalker = new lightir::LightWalker(globalScope);
        tree->accept(*LightWalker);
        m = LightWalker->get_module();
        m->source_file_name_ = input_path;
        lightir::PassManager PM(m.get());
        if (O3 == true) {
            // PM.add_pass<lightir::Dominators>();
            // PM.add_pass<lightir::Mem2Reg>();
            // PM.add_pass<lightir::LoopFind>();
            // PM.add_pass<lightir::Vectorization>();
            // PM.add_pass<lightir::Multithread>();
            // PM.add_pass<lightir::ActiveVars>();
            // PM.add_pass<lightir::ConstPropagation>();
        }
        PM.run();

        IR = m->print();
        std::ofstream output_stream;
        auto output_file = target_path + ".ll";

        output_stream.open(output_file, std::ios::out);
        output_stream << fmt::format("; ModuleID = '{}'\n", m->module_name_);
        output_stream << "source_filename = \"" + m->source_file_name_ + "\"\n\n";
        output_stream << IR;
        output_stream.close();

        cgen::CodeGen code_generator(m);
        asm_code = code_generator.generateModuleCode(true);
        std::ofstream output_stream1;
        auto output_file1 = target_path + ".s";
        output_stream1.open(output_file1, std::ios::out);
        output_stream1 << asm_code;
        output_stream1.close();
        if (assem) {
            auto command_string = "cat " + target_path + ".s ";
            int re_code = std::system(command_string.c_str());
            LOG(INFO) << command_string << re_code;
        }
    }

#if defined(WIN32) || defined(_WIN32) || defined(__APPLE__)
#ifdef RV64
            auto command_string_0 = "riscv64-unknown-elf-gcc -mabi=lp64 -g -ggdb -static  -march=rv64imac -o" +
                                    target_path + " " + target_path + ".s -L./ -lchocopy_stdlib";
#else
            auto command_string_0 = "riscv64-unknown-elf-gcc -mabi=ilp32 -g -ggdb -static -march=rv32imac -o" +
                                    target_path + " " + target_path + ".s -L./ -lchocopy_stdlib";
#endif
            int re_code_0 = std::system(command_string_0.c_str());
            LOG(INFO) << command_string_0 << re_code_0;
#ifdef RV64
            auto command_string_1 = "spike --isa=rv64imac /opt/homebrew/Cellar/riscv-pk/master/bin/pk " + target_path;
#else
            auto command_string_1 =
                "spike --isa=rv32gcv0p10 /opt/homebrew/Cellar/riscv-pk-32/master/bin/pk " + target_path;
#endif
            int re_code_1 = std::system(command_string_1.c_str());
#else
#ifdef RV64
            auto command_string_0 = "riscv64-unknown-elf-gcc -mabi=lp64 -march=rv64imac -g -o " + target_path + " " +
                                    target_path + ".s -L./ -lchocopy_stdlib";
#else
            auto command_string_0 =
                "riscv64-unknown-elf-gcc -mabi=ilp32 -march=rv32imac -g -o " + target_path + " " + target_path +
                ".s -L./ -L/Users/yiweiyang/project/bak/cmake-build-debug-kali-gcc -lchocopy_stdlib";
#endif
            int re_code_0 = std::system(command_string_0.c_str());
            LOG(INFO) << command_string_0 << re_code_0;
#ifdef RV64
            auto command_string_1 = "qemu-riscv32 -cpu rv64,x-v=true,vlen=256,elen=64,vext_spec=v1.0 " + target_path;
#else
            auto command_string_1 = "qemu-riscv32 " + target_path;
#endif
            int re_code_1 = std::system(command_string_1.c_str());
#endif
            LOG(INFO) << command_string_1 << re_code_1;
result:
    return 0;
}
#endif