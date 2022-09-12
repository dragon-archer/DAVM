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

inline constexpr size_t BYTE_BITS  = 8;
inline constexpr size_t HWORD_BITS = 16;
inline constexpr size_t WORD_BITS  = 32;
inline constexpr size_t DWORD_BITS = 64;

inline constexpr size_t BYTE_MASK  = 0xFF;
inline constexpr size_t HWORD_MASK = 0xFFFF;
inline constexpr size_t WORD_MASK  = 0xFFFFFFFF;
inline constexpr size_t DWORD_MASK = 0xFFFFFFFFFFFFFFFF;

// List of all accepted assembler commands
// Most of commands are from RISC-V
enum inst_t : uint8_t {
	I_G_ARITH,
	I_G_SL,
	I_G_IMM,

	// v
	I_RET = 0x08,
	I_HLT,

	// r1
	I_PUSH = 0x10,
	I_POP,
	I_CALL,
};

enum inst_arith_t {
	I_ADD,
	I_SUB,
	I_SLT,
	I_SLTU,
	I_MUL,
	I_MULH,
	I_MULHSU,
	I_MULHU,
	I_DIV,
	I_DIVU,
	I_REM,
	I_REMU,
	I_SLL,
	I_SRL,
	I_SRA,
	I_AND,
	I_OR,
	I_XOR,
};

enum inst_sl_t {
	I_LB,
	I_LH,
	I_LW,
	I_LD,
	I_SB,
	I_SH,
	I_SW,
	I_SD,
};

enum inst_imm_t {
	I_ADDI,
	I_MULI,
	I_SLTI,
	I_SLTUI,
	I_ANDI,
	I_ORI,
	I_XORI,
	I_G_IMM_SHIFT,
};

enum inst_imm_shift_t {
	I_SLLI,
	I_SRLI,
	I_SRAI,
};

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
using asm_func_r2i1_t = void (*)(vm_context_t&, regid_t, regid_t, immediate_t) noexcept;

END_DA_NAMESPACE

#endif // _DAVM_COMMON_TYPE_H_
