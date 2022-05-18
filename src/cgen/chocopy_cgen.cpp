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
const std::regex to_replace("0A");

const InstGen::Reg sp = InstGen::Reg(2);
const InstGen::Reg ra = InstGen::Reg(1);

string_view InstGen::Addr::get_name() const {
    if (str.empty())
        return fmt::format("[{}, #{}]", reg.get_name(), std::to_string(this->offset));
    else
        return str;
}

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
    asm_code += backend->emit_std_func("alloc");
    asm_code += backend->emit_std_func("alloc2");
    asm_code += backend->emit_std_func("abort");
    asm_code += backend->emit_std_func("heap.init");
    asm_code += backend->emit_std_func("print");

    asm_code = replaceStringLiteral(asm_code);

    for (auto &classInfo : this->module->get_class()) {
        backend->emit_prototype(*classInfo);
    }

    // error code
    asm_code += generateErrorCode("error.None", "Operation on None");
    asm_code += generateErrorCode("error.Div", "Division by zero");
    asm_code += generateErrorCode("error.OOB", "Index out of bounds");

    asm_code += backend->start_code();

    asm_code += CodeGen::generateGlobalVarsCode();

    // asm_code += CodeGen::generateGOT();

    for (auto &func : this->module->get_functions()) {
        if (func->get_basic_blocks().size()) {
            asm_code += CodeGen::generateFunctionCode(func);
        }
    }

    return asm_code;
}

string CodeGen::generateModuleCode(bool autoAlloc) {
    return generateModuleCode(autoAlloc ? regAlloc() : map<Value *, int>());
}

std::map<Value *, int> CodeGen::regAlloc() {
/* The regAlloc algorithm Your Code Here */
   
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
/* The Function emit Your Code Here */

    return asm_code;
}
string CodeGen::generateFunctionPostCode(Function *func) {
    std::string asm_code;
    auto tmp_asm_code = CodeGen::getLabelName(func, 1) + ":\n";
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
/* The Instruction emit Your Code Here */
  
    return asm_code;
}
string CodeGen::getLabelName(BasicBlock *bb) { return "." + bb->get_parent()->get_name() + "_" + bb->get_name(); }
string CodeGen::getLabelName(Function *func, int type) {
    const std::vector<std::string> name_list = {"pre", "post"};
    return "." + func->get_name() + "_" + name_list.at(type);
}

/* The CGen Your Code Here */

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
        if(assem){
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
                "spike --isa=rv32gcv0p10 /opt/homebrew/Cellar/riscv-pk_32/master/bin/pk " + target_path;
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
        if (O3 = true) {
            PM.add_pass<lightir::Dominators>();
            PM.add_pass<lightir::Mem2Reg>();
            PM.add_pass<lightir::LoopFind>();
            PM.add_pass<lightir::Vectorization>();
            PM.add_pass<lightir::Multithread>();
            PM.add_pass<lightir::ActiveVars>();
            PM.add_pass<lightir::ConstPropagation>();
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
                "spike --isa=rv32gcv0p10 /opt/homebrew/Cellar/riscv-pk_32/master/bin/pk " + target_path;
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