//
// Created by yangy on 8/22/2021.
//

#ifndef CHOCOPY_COMPILER_RISCVBACKEND_HPP
#define CHOCOPY_COMPILER_RISCVBACKEND_HPP

#include "InstGen.hpp"
#include <chocopy_cgen.hpp>
#include <filesystem>

using namespace lightir;

namespace cgen {
class InstGen;
/** https://riscv.org/wp-content/uploads/2015/01/riscv-calling.pdf */
vector<string> reg_name = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "fp", "s1" "a0",
                           "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4",
                           "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5",
                           "t6" };
/** for LLVM calling conventions */
const vector<InstGen::Reg> caller_save_regs = {InstGen::Reg(1),  InstGen::Reg(5),  InstGen::Reg(6),  InstGen::Reg(7),
                                               InstGen::Reg(10), InstGen::Reg(11), InstGen::Reg(12), InstGen::Reg(13),
                                               InstGen::Reg(14), InstGen::Reg(15), InstGen::Reg(16), InstGen::Reg(17),
                                               InstGen::Reg(28), InstGen::Reg(29), InstGen::Reg(30), InstGen::Reg(31)};
const vector<InstGen::Reg> callee_save_regs = {InstGen::Reg(2),  InstGen::Reg(8),  InstGen::Reg(9),  InstGen::Reg(18),
                                               InstGen::Reg(19), InstGen::Reg(20), InstGen::Reg(21), InstGen::Reg(22),
                                               InstGen::Reg(23), InstGen::Reg(24), InstGen::Reg(25), InstGen::Reg(26),
                                               InstGen::Reg(27)};
const vector<InstGen::Reg> allocate_regs = {InstGen::Reg(10), InstGen::Reg(11), InstGen::Reg(12), InstGen::Reg(13),
                                            InstGen::Reg(14), InstGen::Reg(15), InstGen::Reg(16), InstGen::Reg(17)};
const vector<InstGen::Reg> temp_regs = {InstGen::Reg(5),  InstGen::Reg(6),  InstGen::Reg(7), InstGen::Reg(28),
                                        InstGen::Reg(29), InstGen::Reg(30), InstGen::Reg(31)};

class RiscVBackEnd {
public:
    /** Error codes. */
    int ERROR_ARG = 1, ERROR_DIV_ZERO = 2, ERROR_OOB = 3, ERROR_NONE = 4, ERROR_OOM = 5, ERROR_NYI = 6;

    /** For linux backend. */
    bool LINUX= true;

    /** Size of heap memory. */
    int HEAP_SIZE_BYTES = 1024 * 1024 * 32;

    /** Ecall numbers for intrinsic routines. */
    int EXIT_ECALL = 10, EXIT2_ECALL = 17, PRINT_STRING_ECALL = 4, PRINT_CHAR_ECALL = 11, PRINT_INT_ECALL = 1,
        READ_STRING_ECALL = 8, FILL_LINE_BUFFER__ECALL = 18, SBRK_ECALL = 9;

    map<string, int> to_replace = {{"@sbrk", SBRK_ECALL},
                                   {"@print_string", PRINT_STRING_ECALL},
                                   {"@print_char", PRINT_CHAR_ECALL},
                                   {"@print_int", PRINT_INT_ECALL},
                                   {"@exit2", EXIT2_ECALL},
                                   {"@read_string", READ_STRING_ECALL},
                                   {"@fill_line_buffer", FILL_LINE_BUFFER__ECALL},
                                   {"@.__obj_size__", 4},
                                   {"@.__len__", 12},
                                   {"@.__int__", 12},
                                   {"@.__bool__", 12},
                                   {"@.__str__", 16},
                                   {"@.__elts__", 16},
                                   {"@error_div_zero", ERROR_DIV_ZERO},
                                   {"@error_arg", ERROR_ARG},
                                   {"@error_oob", ERROR_OOB},
                                   {"@error_none", ERROR_NONE},
                                   {"@error_oom", ERROR_OOM},
                                   {"@error_nyi", ERROR_NYI}};

    RiscVBackEnd(int vlen, int vlmax);
    RiscVBackEnd(int vlen) : RiscVBackEnd(vlen, 128){};
    RiscVBackEnd() : RiscVBackEnd(64, 128){};
    const std::filesystem::path stdlib = "../src/cgen/stdlib/";
    const int word_size = 4;
    const int vlen = 64;
    const int vlmax;

    /**
     * Mark the start of a code/text section.
     */
    string start_code() { return fmt::format("\n.data\n"); }

    /** Emit label and body for the function NAME, taking the
     *  source from from the default library directory. */
    string emit_std_func(const InstGen::Addr &label);

    /** Emit the constant section containing the prototype FOR the class
     *  defined by CLASSINFO. */
    string emit_prototype(Class classInfo) {
        string asm_code;
        asm_code += emit_global_label(classInfo.prototype_label_);
        return asm_code;
    }

    /**
     * Emit a local label marker for LABEL. Invoke only once per
     * unique label.
     */
    static string emit_local_label(InstGen::Addr label, string comment) {
        if (!comment.empty())
            return fmt::format("{:<42}#{:<42}", label.get_name(), comment);
        else
            return fmt::format("{}:", label.get_name());
    }
    static string emit_local_label(string label, string comment) {
        if (!comment.empty())
            return fmt::format("{:<42}#{:<42}", label, comment);
        else
            return fmt::format("{}:", label);
    }

    /**
     * Emit a global label marker for LABEL. Invoke only once per
     * unique label.
     */
    static string emit_global_label(InstGen::Addr label) {
        return fmt::format("\n.global {}\n{}:\n", label.get_name(), label.get_name());
    }

    /** Define @NAME to have the value VALUE.  Here, NAME is assumed to be
     *  an identifier consisting of letters, digits, underscores, and any of
     *  the charcters '$' or '.', and that does not start with a digit.  Value
     *  may be a numeral or another symbol.
     */
    template <typename T> static string defineSym(string name, T value) { // deprecated
        if (name.substr(0, 1) == "@") {
            return fmt::format(".equiv {}, {}\n", name, value);
        } else {
            return fmt::format(".equiv @{}, {}\n", name, value);
        }
    }

    /**
     * Align the next instruction/word in memory to
     * a multiple of 2**POW bytes.
     */
    static string align_next(int pow) { return fmt::format(".align {}", pow); }

    /**
     * Emit an ecall instruction, with one-line comment COMMENT,
     * if non-null.
     */
    static string emit_ecall(string comment = "") { return fmt::format("{:<42}#{:<42}\n", "ecall", comment); }

    /**
     * Emit a load-address instruction with destination RD and source
     * LABEL.  COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_la(const InstGen::Reg &rd, InstGen::Addr label, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("la {}, {}", rd.get_name(), label.get_name()), comment);
    }

    /**
     * Emit a load-immediate pseudo-op to set RD to IMM.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_li(const InstGen::Reg &rd, int imm, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("li {}, {}", rd.get_name(), imm), comment);
    }

    /**
     * Emit a load-upper-immediate instruction to set the upper 20 bits
     * of RD to IMM, where 0 <= IMM < 2**20. COMMENT is an optional
     * one-line comment (null if missing).
     */
    static string emit_lui(const InstGen::Reg &rd, int imm, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("lui {}, {}", rd.get_name(), imm), comment);
    }

    /**
     * Emit a move instruction to set RD to the contents of RS.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_mv(const InstGen::Reg &rd, const InstGen::Reg &rs, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("mv {}, {}", rs.get_name(), rs.get_name()), comment);
    }
    /**
     * Emit a jump-register (computed jump) instruction to the address in
     * RS.  COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_jr(const InstGen::Reg &rs, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("jr {}", rs.get_name()), comment);
    }

    /**
     * Emit a jump (unconditional jump) instruction to LABEL.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_j(string label, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("j {}", label), comment);
    }

    /**
     * Emit a jump-and-link instruction to LABEL.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_jal(InstGen::Addr label, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("jal {}", label.get_name()), comment);
    }

    /**
     * Emit a computed-jump-and-link instruction to the address in RS.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_jalr(const InstGen::Reg &rs, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("jalr {}", rs.get_name()), comment);
    }

    /**
     * Emit an add-immediate instruction performing RD = RS + IMM.
     * Requires -2048 <= IMM < 2048. COMMENT is an optional one-line
     * comment (null if missing).
     */
    static string emit_addi(const InstGen::Reg &rd, const InstGen::Reg &rs, int imm, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("addi {}, {}, {}", rd.get_name(), rs.get_name(), imm),
                           comment);
    }

    /**
     * Emit an add-immediate instruction performing RD = RS + IMM.
     * Here, IMM is a string generally containing a symbolic assembler
     * constant (see defineSym) representing an integer value, or an
     * expression of the form @NAME+NUM or @NAME-NUM.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_addi(const InstGen::Reg &rd, const InstGen::Reg &rs, string imm, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("addi {}, {}, {}", rd.get_name(), rs.get_name(), imm),
                           comment);
    }

    /**
     * Emit an add instruction performing RD = RS1 + RS2 mod 2**32.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_add(const InstGen::Reg &rd, const InstGen::Reg &rs1, const InstGen::Value &rs2,
                           string comment = "") {
        return fmt::format("{:<42}#{:<42}\n",
                           fmt::format("add {}, {}, {}", rd.get_name(), rs1.get_name(), rs2.get_name()), comment);
    }

    /**
     * Emit a subtract instruction performing RD = RS1 - RS2 mod 2**32.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_sub(const InstGen::Reg &rd, const InstGen::Reg &rs1, const InstGen::Value &rs2,
                           string comment = "") {
        return fmt::format("{:<42}#{:<42}\n",
                           fmt::format("sub {}, {}, {}", rd.get_name(), rs1.get_name(), rs2.get_name()), comment);
    }

    /**
     * Emit a multiply instruction performing RD = RS1 * RS2 mod 2**32.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_mul(const InstGen::Reg &rd, const InstGen::Reg &rs1, const InstGen::Reg &rs2,
                           string comment = "") {
        return fmt::format("{:<42}#{:<42}\n",
                           fmt::format("mul {}, {}, {}", rd.get_name(), rs1.get_name(), rs2.get_name()), comment);
    }

    /**
     * Emit a signed integer divide instruction performing
     * RD = RS1 / RS2 mod 2**32, rounding the result toward 0.
     * If RS2 == 0, sets RD to -1. If RS1 == -2**31 and RS2 == -1,
     * sets RD to -2**31.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_div(const InstGen::Reg &rd, const InstGen::Reg &rs1, const InstGen::Reg &rs2,
                           string comment = "") {
        return fmt::format("{:<42}#{:<42}\n",
                           fmt::format("div {}, {}, {}", rd.get_name(), rs1.get_name(), rs2.get_name()), comment);
    }

    /**
     * Emit a remainder instruction: RD = RS1 rem RS2 defined so that
     * (RS1 / RS2) * RS2 + (RS1 rem RS2) == RS1, where / is as for
     * emitDIV.  COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_rem(const InstGen::Reg &rd, const InstGen::Reg &rs1, const InstGen::Reg &rs2,
                           string comment = "") {
        return fmt::format("{:<42}#{:<42}\n",
                           fmt::format("rem {}, {}, {}", rd.get_name(), rs1.get_name(), rs2.get_name()), comment);
    }

    /**
     * Emit an xor instruction: RD = RS1 ^ RS2. COMMENT is an optional
     * one-line comment (null if missing).
     */
    static string emit_xor(const InstGen::Reg &rd, const InstGen::Reg &rs1, const InstGen::Reg &rs2,
                           string comment = "") {
        return fmt::format("{:<42}#{:<42}\n",
                           fmt::format("xor {}, {}, {}", rd.get_name(), rs1.get_name(), rs2.get_name()), comment);
    }

    /**
     * Emit an xor-immediate instruction: RD = RS ^ IMM, where
     * -2048 <= IMM < 2048.  COMMENT is an optional
     * one-line comment (null if missing).
     */
    static string emit_xori(const InstGen::Reg &rd, const InstGen::Reg &rs, int imm, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("xori {}, {}, {}", rd.get_name(), rs.get_name(), imm),
                           comment);
    }

    /**
     * Emit a bitwise and instruction: RD = RS1 & RS2.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_and(const InstGen::Reg &rd, const InstGen::Reg &rs1, const InstGen::Reg &rs2,
                           string comment = "") {
        return fmt::format("{:<42}#{:<42}\n",
                           fmt::format("and {}, {}, {}", rd.get_name(), rs1.get_name(), rs2.get_name()), comment);
    }

    /**
     * Emit a bitwise and-immediate instruction: RD = RS & IMM, where
     * -2048 <= IMM < 2048.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_andi(const InstGen::Reg &rd, const InstGen::Reg &rs, int imm, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("andi {}, {}, {}", rd.get_name(), rs.get_name(), imm),
                           comment);
    }

    /**
     * Emit a bitwise or instruction: RD = RS1 | RS2.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_or(const InstGen::Reg &rd, const InstGen::Reg &rs1, const InstGen::Reg &rs2,
                          string comment = "") {
        return fmt::format("{:<42}#{:<42}\n",
                           fmt::format("or {}, {}, {}", rd.get_name(), rs1.get_name(), rs2.get_name()), comment);
    }

    /**
     * Emit a bitwise or-immediate instruction: RD = RS | IMM, where
     * -2048 <= IMM < 2048.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_ori(const InstGen::Reg &rd, const InstGen::Reg &rs, int imm, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("ori {}, {}, {}", rd.get_name(), rs.get_name(), imm),
                           comment);
    }

    /**
     * Emit a logical left shift instruction: RD = RS1 << (RS2 & 0x31).
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_sll(const InstGen::Reg &rd, const InstGen::Reg &rs1, const InstGen::Reg &rs2,
                           string comment = "") {
        return fmt::format("{:<42}#{:<42}\n",
                           fmt::format("sll {}, {}, {}", rd.get_name(), rs1.get_name(), rs2.get_name()), comment);
    }

    /**
     * Emit a logical left shift instruction: RD = RS << (IMM & 0x31).
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_slli(const InstGen::Reg &rd, const InstGen::Reg &rs, int imm, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("slli {}, {}, {}", rd.get_name(), rs.get_name(), imm),
                           comment);
    }

    /**
     * Emit a logical right shift instruction: RD = RS1 >>> (RS2 & 0x31).
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_srl(const InstGen::Reg &rd, const InstGen::Reg &rs1, const InstGen::Reg &rs2,
                           string comment = "") {
        return fmt::format("{:<42}#{:<42}\n",
                           fmt::format("srl {}, {}, {}", rd.get_name(), rs1.get_name(), rs2.get_name()), comment);
    }

    /**
     * Emit a logical right shift instruction: RD = RS >>> (IMM & 0x31).
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_srli(const InstGen::Reg &rd, const InstGen::Reg &rs, int imm, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("srli {}, {}, {}", rd.get_name(), rs.get_name(), imm),
                           comment);
    }

    /**
     * Emit an arithmetic right shift instruction: RD = RS1 >> (RS2 & 0x31).
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_sra(const InstGen::Reg &rd, const InstGen::Reg &rs1, const InstGen::Reg &rs2,
                           string comment = "") {
        return fmt::format("{:<42}#{:<42}\n",
                           fmt::format("sra {}, {}, {}", rd.get_name(), rs1.get_name(), rs2.get_name()), comment);
    }

    /**
     * Emit an arithmetic right shift instruction: RD = RS >> (IMM & 0x31).
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_srai(const InstGen::Reg &rd, const InstGen::Reg &rs, int imm, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("srai {}, {}, {}", rd.get_name(), rs.get_name(), imm),
                           comment);
    }

    /**
     * Emit a load-word instruction: RD = MEMORY[RS + IMM]:4, where
     * -2048 <= IMM < 2048.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_lw(const InstGen::Reg &rd, const InstGen::Reg &rs, int imm, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("lw {}, {}({})", rd.get_name(), imm, rs.get_name()), comment);
    }

    /**
     * Emit a load-word instruction: RD = MEMORY[RS + IMM]:4, where
     * -2048 <= IMM < 2048.  Here, IMM is symbolic constant expression
     * (see emitADDI).  COMMENT is an optional one-line
     * comment (null if missing).
     */
    static string emit_lw(const InstGen::Reg &rd, const InstGen::Reg &rs, string imm, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("lw {}, {}({})", rd.get_name(), imm, rs.get_name()), comment);
    }

    /**
     * Emit a store-word instruction: MEMORY[RS1 + IMM]:4 = RS2, where
     * -2048 <= IMM < 2048.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_sw(const InstGen::Reg &rs2, const InstGen::Reg &rs1, int imm, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("sw {}, {}({})", rs2.get_name(), imm, rs1.get_name()),
                           comment);
    }

    /**
     * Emit a load-word instruction for globals: RD = MEMORY[LABEL]:4.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_lw(const InstGen::Reg &rs, InstGen::Addr label, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("lw {}, {}", rs.get_name(), label.get_name()), comment);
    }

    /**
     * Emit a store-word instruction for globals: MEMORY[LABEL]:4 = RS,
     * using TMP as a temporary register.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_sw(const InstGen::Reg &rs, InstGen::Addr label, const InstGen::Reg &tmp, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n",
                           fmt::format("sw {}, {}, {}", rs.get_name(), label.get_name(), tmp.get_name()), comment);
    }

    /**
     * Emit a load-byte instruction: RD = MEMORY[RS + IMM]:1, where
     * -2048 <= IMM < 2048.  Sign extends the byte loaded.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_lb(const InstGen::Reg &rd, const InstGen::Reg &rs, const int &imm, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("lb {}, {}, {}", rd.get_name(), imm, rs.get_name()), comment);
    }

    /**
     * Emit a load-byte-unsigned instruction: RD = MEMORY[RS + IMM]:1, where
     * -2048 <= IMM < 2048.  Zero-extends the byte loaded.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_lbu(const InstGen::Reg &rd, const InstGen::Reg &rs, const int &imm, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("lbu {}, {}, {}", rd.get_name(), imm, rs.get_name()),
                           comment);
    }

    /**
     * Emit a store-byte instruction: MEMORY[RS1 + IMM]:1 = RS2, where
     * -2048 <= IMM < 2048.  Assigns the low-order byte of RS2 to memory.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_sb(const InstGen::Reg &rs2, const InstGen::Reg &rs1, const int &imm, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("sb {}, {}]({}) ", rs2.get_name(), imm, rs1.get_name()),
                           comment);
    }

    /**
     * Emit a branch-if-equal instruction: if RS1 == RS2 goto LABEL.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_beq(const InstGen::Reg &rs1, const InstGen::Reg &rs2, InstGen::Addr label, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n",
                           fmt::format("beq {}, {}, {}", rs1.get_name(), rs2.get_name(), label.get_name()), comment);
    }

    /**
     * Emit a branch-if-unequal instruction: if RS1 != RS2 goto LABEL.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_bne(const InstGen::Reg &rs1, const InstGen::Reg &rs2, InstGen::Addr label, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n",
                           fmt::format("bne {}, {}, {}", rs1.get_name(), rs2.get_name(), label.get_name()), comment);
    }

    /**
     * Emit a branch-if-greater-or-equal (signed) instruction:
     * if RS1 >= RS2 goto LABEL.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_bge(const InstGen::Reg &rd, const InstGen::Reg &rs, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("bge {}, {}", rd.get_name(), rs.get_name()), comment);
    };

    /**
     * Emit a branch-if-greater-or-equal (unsigned) instruction:
     * if RS1 >= RS2 goto LABEL.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_bgeu(const InstGen::Reg &rd, const InstGen::Reg &rs, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("bgeu {}, {}", rd.get_name(), rs.get_name()), comment);
    };

    /**
     * Emit a branch-if-less-than (signed) instruction:
     * if RS1 < RS2 goto LABEL.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_blt(const InstGen::Reg &rd, const InstGen::Reg &rs, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("blt {}, {}", rd.get_name(), rs.get_name()), comment);
    };

    /**
     * Emit a branch-if-less-than (unsigned) instruction:
     * if RS1 < RS2 goto LABEL.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_bltu(const InstGen::Reg &rd, const InstGen::Reg &rs, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("bltu {}, {}", rd.get_name(), rs.get_name()), comment);
    };

    /**
     * Emit a branch-if-zero instruction: if RS == 0 goto LABEL.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_bnqz(const InstGen::Reg &rd, const InstGen::Reg &rs, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("bnqz {}, {}", rd.get_name(), rs.get_name()), comment);
    };

    /**
     * Emit a branch-if-not-zero instruction: if RS != 0 goto LABEL.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_bnez(const InstGen::Reg &rd, const InstGen::Reg &rs, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("bnez {}, {}", rd.get_name(), rs.get_name()), comment);
    };

    /**
     * Emit a branch-if-less-than-zero instruction: if RS < 0 goto LABEL.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_bltz(const InstGen::Reg &rd, const InstGen::Reg &rs, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("bltz {}, {}", rd.get_name(), rs.get_name()), comment);
    };

    /**
     * Emit a branch-if-greater-than-zero instruction: if RS > 0 goto LABEL.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_bgtz(const InstGen::Reg &rd, const InstGen::Reg &rs, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("bgtz {}, {}", rd.get_name(), rs.get_name()), comment);
    };

    /**
     * Emit a branch-if-less-than-equal-to-zero instruction:
     * if RS <= 0 goto LABEL.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_blez(const InstGen::Reg &rd, const InstGen::Reg &rs, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("blez {}, {}", rd.get_name(), rs.get_name()), comment);
    };

    /**
     * Emit a branch-if-greater-than-equal-to-zero instruction:
     * if RS >= 0 goto LABEL.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_bgez(const InstGen::Reg &rd, const InstGen::Reg &rs, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("bgez {}, {}", rd.get_name(), rs.get_name()), comment);
    };

    /**
     * Emit a set-less-than instruction: RD = 1 if RS1 < RS2 else 0.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_slt(const InstGen::Reg &rd, const InstGen::Reg &rs1, const InstGen::Reg &rs2,
                           string comment = "") {
        return fmt::format("{:<42}#{:<42}\n",
                           fmt::format("slt {}, {}, {}", rd.get_name(), rs1.get_name(), rs2.get_name()), comment);
    };

    /**
     * Emit a set-if-zero instruction: RD = 1 if RS == 0 else 0.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_seqz(const InstGen::Reg &rd, const InstGen::Reg &rs, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("seqz {}, {}", rd.get_name(), rs.get_name()), comment);
    };

    /**
     * Emit a set-if-not-zero instruction: RD = 1 if RS != 0 else 0.
     * COMMENT is an optional one-line comment (null if missing).
     */
    static string emit_snez(const InstGen::Reg &rd, const InstGen::Reg &rs, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("snez {}, {}", rd.get_name(), rs.get_name()), comment);
    };

    static string emit_vload(const InstGen::Reg &rd, const InstGen::Reg &rs, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("vload {}, {}", rd.get_name(), rs.get_name()), comment);
    };
    static string emit_vstore(const InstGen::Reg &rd, const InstGen::Reg &rs, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("vstore {}, {}", rd.get_name(), rs.get_name()), comment);
    };
    static string emit_vadd_vv(const InstGen::Reg &rd, const InstGen::Reg &rs, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("vadd.vv {}, {}", rd.get_name(), rs.get_name()), comment);
    };
    static string emit_vdiv_vv(const InstGen::Reg &rd, const InstGen::Reg &rs, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("vdiv.vv {}, {}", rd.get_name(), rs.get_name()), comment);
    };
    static string emit_vmul_vv(const InstGen::Reg &rd, const InstGen::Reg &rs, string comment = "") {
        return fmt::format("{:<42}#{:<42}\n", fmt::format("vmul.vv {}, {}", rd.get_name(), rs.get_name()), comment);
    };

    static string emit_epilogue(const std::vector<InstGen::Reg> &reg_list, string comment = "") {
        std::string asm_code;
        // TODO make epilogue
        for (auto &i : reg_list) {
            asm_code += fmt::format("{}", 1);
        }
        asm_code += emit_j("label_2");
        return asm_code;
    }
};
} // namespace cgen
#endif // CHOCOPY_COMPILER_RISCVBACKEND_HPP
