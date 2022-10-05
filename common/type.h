/**
 * @file      type.h
 * @brief     List of assembler commands & registers & types
 * @version   0.1
 * @author    dragon-archer
 *
 * @copyright Copyright (c) 2022
 */

#ifndef _DAVM_COMMON_TYPE_H_
#define _DAVM_COMMON_TYPE_H_

#include <common/pch.h>

BEGIN_DA_NAMESPACE

using byte_t   = uint8_t;
using hword_t  = uint16_t;
using word_t   = uint32_t;
using dword_t  = uint64_t;
using sbyte_t  = int8_t;
using shword_t = int16_t;
using sword_t  = int32_t;
using sdword_t = int64_t;

using register_t  = uint64_t;
using sregister_t = int64_t;
using regid_t	  = uint32_t; // Actually 5-bit is enough, use 32-bit to speed up

using size_t	  = uint64_t;
using immediate_t = uint32_t;
using addr_t	  = uint64_t;

inline constexpr size_t BYTE_BITS	   = 8;
inline constexpr size_t HWORD_BITS	   = 16;
inline constexpr size_t WORD_BITS	   = 32;
inline constexpr size_t DWORD_BITS	   = 64;
inline constexpr size_t IMM_SHIFT_BITS = 10;
inline constexpr size_t IMM_SHORT_BITS = 12;
inline constexpr size_t IMM_LONG_BITS  = 20;

inline constexpr size_t BYTE_MASK  = 0xFF;
inline constexpr size_t HWORD_MASK = 0xFFFF;
inline constexpr size_t WORD_MASK  = 0xFFFFFFFF;
inline constexpr size_t DWORD_MASK = 0xFFFFFFFFFFFFFFFF;

#define DA_X_V             \
	DA_X(RET, INST_V, ret) \
	DA_X(HLT, INST_V, hlt)

#define DA_X_R1               \
	DA_X(PUSH, INST_R1, push) \
	DA_X(POP, INST_R1, pop)   \
	DA_X(CALL, INST_R1, call)

#define DA_X_R2 \
	DA_X(MOV, INST_R2, mov)

#define DA_X_R1I1                 \
	DA_X(LUI, INST_R1I1, lui)     \
	DA_X(AUIPC, INST_R1I1, auipc) \
	DA_X(JAL, INST_R1I1, jal)

#define DA_X_ARITH                \
	DA_X(ADD, INST_R3, add)       \
	DA_X(SUB, INST_R3, sub)       \
	DA_X(SLT, INST_R3, slt)       \
	DA_X(SLTU, INST_R3, sltu)     \
	DA_X(MUL, INST_R3, mul)       \
	DA_X(MULH, INST_R3, mulh)     \
	DA_X(MULHSU, INST_R3, mulhsu) \
	DA_X(MULHU, INST_R3, mulhu)   \
	DA_X(DIV, INST_R3, div)       \
	DA_X(DIVU, INST_R3, divu)     \
	DA_X(REM, INST_R3, rem)       \
	DA_X(REMU, INST_R3, remu)     \
	DA_X(SLL, INST_R3, sll)       \
	DA_X(SRL, INST_R3, srl)       \
	DA_X(SRA, INST_R3, sra)       \
	DA_X(AND, INST_R3, and)       \
	DA_X(OR, INST_R3, or)         \
	DA_X(XOR, INST_R3, xor)

#define DA_X_LOAD             \
	DA_X(LB, INST_R2I1, lb)   \
	DA_X(LH, INST_R2I1, lh)   \
	DA_X(LW, INST_R2I1, lw)   \
	DA_X(LD, INST_R2I1, ld)   \
	DA_X(LBU, INST_R2I1, lbu) \
	DA_X(LHU, INST_R2I1, lhu) \
	DA_X(LWU, INST_R2I1, lwu)

#define DA_X_SAVE           \
	DA_X(SB, INST_R2I1, sb) \
	DA_X(SH, INST_R2I1, sh) \
	DA_X(SW, INST_R2I1, sw) \
	DA_X(SD, INST_R2I1, sd)

#define DA_X_IMM                  \
	DA_X(ADDI, INST_R2I1, addi)   \
	DA_X(MULI, INST_R2I1, muli)   \
	DA_X(SLTI, INST_R2I1, slti)   \
	DA_X(SLTUI, INST_R2I1, sltui) \
	DA_X(ANDI, INST_R2I1, andi)   \
	DA_X(ORI, INST_R2I1, ori)     \
	DA_X(XORI, INST_R2I1, xori)

#define DA_X_IMM_SHIFT          \
	DA_X(SLLI, INST_R2I1, slli) \
	DA_X(SRLI, INST_R2I1, srli) \
	DA_X(SRAI, INST_R2I1, srai)

#define DA_X_BRANCH             \
	DA_X(JALR, INST_R2I1, jalr) \
	DA_X(BEQ, INST_R2I1, beq)   \
	DA_X(BNE, INST_R2I1, bne)   \
	DA_X(BLT, INST_R2I1, blt)   \
	DA_X(BGE, INST_R2I1, bge)   \
	DA_X(BLTU, INST_R2I1, bltu) \
	DA_X(BGEU, INST_R2I1, bgeu)

#define DA_X(name, ...) I_##name,
// List of all accepted assembler commands
// Most of commands are from RISC-V
enum inst_t : uint8_t {
	I_G_ARITH,
	I_G_LOAD,
	I_G_SAVE,
	I_G_IMM,
	I_G_BRANCH,

	// v
	I_DUMMY_V = 0x07,
	DA_X_V

		// r1
		I_DUMMY_R1
	= 0x0F,
	DA_X_R1

		// r2
		I_DUMMY_R2
	= 0x1F,
	DA_X_R2

		// r1i1
		I_DUMMY_R1I1
	= 0x3F,
	DA_X_R1I1
};

enum inst_arith_t {
	DA_X_ARITH
};

enum inst_load_t {
	DA_X_LOAD
};

enum inst_save_t {
	DA_X_SAVE
};

enum inst_imm_t {
	DA_X_IMM
		I_G_IMM_SHIFT,
};

enum inst_imm_shift_t {
	DA_X_IMM_SHIFT
};

enum inst_branch_t {
	DA_X_BRANCH
};

#undef DA_X

struct vm_context_t {
	/**
	 * ID	   |Alias  |Desc
	 * x0		pc		Program Counter
	 * x1		ra		Return Address
	 * x2		bp		Base Pointer
	 * x3		sp		Stack Pointer
	 * x4		gp		Global Pointer
	 * x5		tp		Thread Pointer
	 * x6		cp		Context Pointer
	 * x7		rv		Return Value
	 * x8  - x15		Function Arguments
	 * x16 - x30		Temp Registers
	 * x31		zr		Zero Register (should be read only)
	 */
	register_t x[32];
};

// Macros for easier access to special registers
// No brackets to avoid change lvalue to rvalue
#define DAVM_PC(context) context.x[0]
#define DAVM_RA(context) context.x[1]
#define DAVM_BP(context) context.x[2]
#define DAVM_SP(context) context.x[3]
#define DAVM_GP(context) context.x[4]
#define DAVM_TP(context) context.x[5]
#define DAVM_CP(context) context.x[6]
#define DAVM_RV(context) context.x[7]
#define DAVM_ZR(context) context.x[31]

#define DAVM_CAST(type, x) reinterpret_cast<type>(x)

using asm_func_v_t	  = void (*)(vm_context_t&) noexcept;
using asm_func_r1_t	  = void (*)(vm_context_t&, regid_t) noexcept;
using asm_func_r2_t	  = void (*)(vm_context_t&, regid_t, regid_t) noexcept;
using asm_func_r3_t	  = void (*)(vm_context_t&, regid_t, regid_t, regid_t) noexcept;
using asm_func_r1i1_t = void (*)(vm_context_t&, regid_t, immediate_t) noexcept;
using asm_func_r2i1_t = void (*)(vm_context_t&, regid_t, regid_t, immediate_t) noexcept;

struct asm_cmd_v_t {
	uint32_t op2 : 3;
	uint32_t op : 4;
	uint32_t unused : 25;
};

struct asm_cmd_r1_t {
	uint32_t op2 : 3;
	uint32_t op : 4;
	uint32_t rd : 5;
	uint32_t unused : 20;
};

struct asm_cmd_r2_t {
	uint32_t op : 7;
	uint32_t rd : 5;
	uint32_t ra : 5;
	uint32_t unused : 15;
};

struct asm_cmd_r3_t {
	uint32_t op : 7;
	uint32_t rd : 5;
	uint32_t ra : 5;
	uint32_t rb : 5;
	uint32_t op2 : 5;
	uint32_t unused : 5;
};

struct asm_cmd_r1i1_t {
	uint32_t op : 7;
	uint32_t rd : 5;
	uint32_t imm : 20;
};

struct asm_cmd_r2i1_t {
	uint32_t op : 7;
	uint32_t rd : 5;
	uint32_t ra : 5;
	uint32_t op2 : 3;
	uint32_t imm : 12;
};

struct asm_cmd_imm_shift_t {
	uint32_t op : 7;
	uint32_t rd : 5;
	uint32_t ra : 5;
	uint32_t op2 : 3;
	uint32_t op3 : 2;
	uint32_t imm : 10;
};

enum inst_type_t {
	INST_V,
	INST_R1,
	INST_R2,
	INST_R3,
	INST_R1I1,
	INST_R2I1,
};

END_DA_NAMESPACE

#endif // _DAVM_COMMON_TYPE_H_
