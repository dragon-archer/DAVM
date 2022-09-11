/**
 * @file      asm.h
 * @brief     Assembler functions
 * @version   0.1
 * @author    dragon-archer
 *
 * @copyright Copyright (c) 2022
 */

#ifndef _DAVM_COMMON_ASM_H_
#define _DAVM_COMMON_ASM_H_

#include <common/pch.h>
#include <common/type.h>

BEGIN_DA_NAMESPACE

// Error handling

inline void asm_error_v(vm_context_t& context) noexcept {
	DAVM_PC(context) = 0; // Hlt the PC
}

// Asm functions

inline void asm_nop([[maybe_unused]] vm_context_t& context) noexcept { }

inline void asm_hlt(vm_context_t& context) noexcept {
	// Set pc to 0 to halt
	DAVM_PC(context) = 0;
}

// Stack operations
inline void asm_push(vm_context_t& context, regid_t rd) noexcept {
	DAVM_SP(context) -= sizeof(register_t);
	*DAVM_CAST(register_t*, DAVM_SP(context)) = context.x[rd];
}

inline void asm_pop(vm_context_t& context, regid_t rd) noexcept {
	context.x[rd] = *DAVM_CAST(register_t*, DAVM_SP(context));
	DAVM_SP(context) += sizeof(register_t);
}

// Actually equals to (in Intel style):
// PUSHQ	rip
// PUSHQ	rbp
// MOVQ		rbp, rsp
// MOVQ		rip, target
inline void asm_call(vm_context_t& context, regid_t target) noexcept {
	DAVM_SP(context) -= sizeof(register_t) * 2;
	*DAVM_CAST(register_t*, DAVM_SP(context) + sizeof(register_t)) = DAVM_PC(context);
	*DAVM_CAST(register_t*, DAVM_SP(context))					   = DAVM_BP(context);
	DAVM_BP(context)											   = DAVM_SP(context);
	DAVM_PC(context)											   = context.x[target];
}

// Actually equals to (in Intel style):
// MOVQ	rsp, rbp
// POPQ	rbp
// POPQ	rip
inline void asm_ret(vm_context_t& context) noexcept {
	DAVM_SP(context) = DAVM_BP(context);
	DAVM_BP(context) = *DAVM_CAST(register_t*, DAVM_SP(context));
	DAVM_PC(context) = *DAVM_CAST(register_t*, DAVM_SP(context) + sizeof(register_t));
	DAVM_SP(context) += sizeof(register_t) * 2;
}

// Arithmetical Operations
inline constexpr void asm_add(vm_context_t& context, regid_t rd, regid_t ra, regid_t rb) noexcept {
	context.x[rd] = context.x[ra] + context.x[rb];
}

inline constexpr void asm_sub(vm_context_t& context, regid_t rd, regid_t ra, regid_t rb) noexcept {
	context.x[rd] = context.x[ra] - context.x[rb];
}

inline constexpr void asm_sll(vm_context_t& context, regid_t rd, regid_t ra, regid_t rb) noexcept {
	context.x[rd] = context.x[ra] << context.x[rb];
}

inline constexpr void asm_srl(vm_context_t& context, regid_t rd, regid_t ra, regid_t rb) noexcept {
	context.x[rd] = context.x[ra] >> context.x[rb];
}

inline constexpr void asm_sra(vm_context_t& context, regid_t rd, regid_t ra, regid_t rb) noexcept {
	context.x[rd] = sregister_t(context.x[ra]) >> sregister_t(context.x[rb]);
}

inline constexpr void asm_mul(vm_context_t& context, regid_t rd, regid_t ra, regid_t rb) noexcept {
	context.x[rd] = context.x[ra] * context.x[rb];
}

inline constexpr void asm_mulh(vm_context_t& context, regid_t rd, regid_t ra, regid_t rb) noexcept {
	context.x[rd] = (sregister_t(context.x[ra]) >> WORD_BITS) * (sregister_t(context.x[rb]) >> WORD_BITS)
		+ (((sregister_t(context.x[ra]) >> WORD_BITS) * (sregister_t(context.x[rb]) & WORD_MASK)) >> WORD_BITS)
		+ (((sregister_t(context.x[rb]) >> WORD_BITS) * (sregister_t(context.x[ra]) & WORD_MASK)) >> WORD_BITS);
}

inline constexpr void asm_mulhsu(vm_context_t& context, regid_t rd, regid_t ra, regid_t rb) noexcept {
	context.x[rd] = (sregister_t(context.x[ra]) >> WORD_BITS) * (context.x[rb] >> WORD_BITS)
		+ (((sregister_t(context.x[ra]) >> WORD_BITS) * (context.x[rb] & WORD_MASK)) >> WORD_BITS)
		+ (((context.x[rb] >> WORD_BITS) * (sregister_t(context.x[ra]) & WORD_MASK)) >> WORD_BITS);
}

inline constexpr void asm_mulhu(vm_context_t& context, regid_t rd, regid_t ra, regid_t rb) noexcept {
	context.x[rd] = (context.x[ra] >> WORD_BITS) * (context.x[rb] >> WORD_BITS)
		+ (((context.x[ra] >> WORD_BITS) * (context.x[rb] & WORD_MASK)) >> WORD_BITS)
		+ (((context.x[rb] >> WORD_BITS) * (context.x[ra] & WORD_MASK)) >> WORD_BITS);
}

inline constexpr void asm_div(vm_context_t& context, regid_t rd, regid_t ra, regid_t rb) noexcept {
	context.x[rd] = sregister_t(context.x[ra]) / sregister_t(context.x[rb]);
}

inline constexpr void asm_divu(vm_context_t& context, regid_t rd, regid_t ra, regid_t rb) noexcept {
	context.x[rd] = context.x[ra] / context.x[rb];
}

inline constexpr void asm_rem(vm_context_t& context, regid_t rd, regid_t ra, regid_t rb) noexcept {
	context.x[rd] = sregister_t(context.x[ra]) % sregister_t(context.x[rb]);
}

inline constexpr void asm_remu(vm_context_t& context, regid_t rd, regid_t ra, regid_t rb) noexcept {
	context.x[rd] = context.x[ra] % context.x[rb];
}

// Bit operations
inline constexpr void asm_and(vm_context_t& context, regid_t rd, regid_t ra, regid_t rb) noexcept {
	context.x[rd] = context.x[ra] & context.x[rb];
}

inline constexpr void asm_or(vm_context_t& context, regid_t rd, regid_t ra, regid_t rb) noexcept {
	context.x[rd] = context.x[ra] | context.x[rb];
}

inline constexpr void asm_xor(vm_context_t& context, regid_t rd, regid_t ra, regid_t rb) noexcept {
	context.x[rd] = context.x[ra] ^ context.x[rb];
}

inline constexpr void asm_slt(vm_context_t& context, regid_t rd, regid_t ra, regid_t rb) noexcept {
	context.x[rd] = sregister_t(context.x[ra]) < sregister_t(context.x[rb]);
}

inline constexpr void asm_sltu(vm_context_t& context, regid_t rd, regid_t ra, regid_t rb) noexcept {
	context.x[rd] = context.x[ra] < context.x[rb];
}

// Immediate operations
inline constexpr void asm_addi(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	context.x[rd] = context.x[ra] + sregister_t(imm);
}

inline constexpr void asm_muli(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	context.x[rd] = context.x[ra] * sregister_t(imm);
}

inline constexpr void asm_slti(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	context.x[rd] = sregister_t(context.x[ra]) < sregister_t(imm);
}

inline constexpr void asm_sltui(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	context.x[rd] = context.x[ra] < imm;
}

inline constexpr void asm_andi(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	context.x[rd] = context.x[ra] & imm;
}

inline constexpr void asm_ori(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	context.x[rd] = context.x[ra] | imm;
}

inline constexpr void asm_xori(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	context.x[rd] = context.x[ra] ^ imm;
}

inline constexpr void asm_slli(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	context.x[rd] = context.x[ra] << imm;
}

inline constexpr void asm_srli(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	context.x[rd] = context.x[ra] >> imm;
}

inline constexpr void asm_srai(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	context.x[rd] = sregister_t(context.x[ra]) >> imm;
}

// Load & Save
// 7 bit opcode, 5 bit rd, 5 bit ra, 3 bit opcode2, 12 bit signed offset
inline void asm_lb(vm_context_t& context, regid_t rd, regid_t ra, immediate_t off) noexcept {
	context.x[rd] = *reinterpret_cast<byte_t*>(context.x[ra] + sregister_t(off));
}

inline void asm_lh(vm_context_t& context, regid_t rd, regid_t ra, immediate_t off) noexcept {
	context.x[rd] = *reinterpret_cast<hword_t*>(context.x[ra] + sregister_t(off));
}

inline void asm_lw(vm_context_t& context, regid_t rd, regid_t ra, immediate_t off) noexcept {
	context.x[rd] = *reinterpret_cast<word_t*>(context.x[ra] + sregister_t(off));
}

inline void asm_ld(vm_context_t& context, regid_t rd, regid_t ra, immediate_t off) noexcept {
	context.x[rd] = *reinterpret_cast<dword_t*>(context.x[ra] + sregister_t(off));
}

inline void asm_sb(vm_context_t& context, regid_t rd, regid_t ra, immediate_t off) noexcept {
	*reinterpret_cast<byte_t*>(context.x[rd]) = byte_t(context.x[ra] + sregister_t(off));
}

inline void asm_sh(vm_context_t& context, regid_t rd, regid_t ra, immediate_t off) noexcept {
	*reinterpret_cast<hword_t*>(context.x[rd]) = hword_t(context.x[ra] + sregister_t(off));
}

inline void asm_sw(vm_context_t& context, regid_t rd, regid_t ra, immediate_t off) noexcept {
	*reinterpret_cast<word_t*>(context.x[rd]) = word_t(context.x[ra] + sregister_t(off));
}

inline void asm_sd(vm_context_t& context, regid_t rd, regid_t ra, immediate_t off) noexcept {
	*reinterpret_cast<dword_t*>(context.x[rd]) = dword_t(context.x[ra] + sregister_t(off));
}

// Function tables
// Use [[maybe_unused]] attribute to avoid warning

[[maybe_unused]] static constexpr asm_func_v_t asm_table_v[] = {
	asm_ret,
	asm_hlt,
};

[[maybe_unused]] static constexpr asm_func_r1_t asm_table_r1[] = {
	asm_push,
	asm_pop,
	asm_call,
};

[[maybe_unused]] static constexpr asm_func_r3_t asm_table_arith[] = {
	asm_add,
	asm_sub,
	asm_slt,
	asm_sltu,

	asm_mul,
	asm_mulh,
	asm_mulhsu,
	asm_mulhu,

	asm_div,
	asm_divu,
	asm_rem,
	asm_remu,

	asm_sll,
	asm_srl,
	asm_sra,
	asm_and,
	asm_or,
	asm_xor,
};

[[maybe_unused]] static constexpr asm_func_r2i1_t asm_table_sl[] = {
	asm_lb,
	asm_lh,
	asm_lw,
	asm_ld,

	asm_sb,
	asm_sh,
	asm_sw,
	asm_sd,
};

[[maybe_unused]] static constexpr asm_func_r2i1_t asm_table_imm[] = {
	asm_addi,
	asm_muli,
	asm_slti,
	asm_sltui,
	asm_andi,
	asm_ori,
	asm_xori,
};

[[maybe_unused]] static constexpr asm_func_r2i1_t asm_table_imm_shift[] = {
	asm_slli,
	asm_srli,
	asm_srai,
};

END_DA_NAMESPACE

#endif // _DAVM_COMMON_ASM_H_
