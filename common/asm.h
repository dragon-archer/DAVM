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
#include <common/log.h>
#include <common/type.h>

BEGIN_DA_NAMESPACE

template<size_t Bits, typename NT, typename OT>
DA_ALWAYS_INLINE constexpr NT sext(OT x) noexcept {
	return ((x & (1 << (Bits - 1)) ? (x | (-(NT(1) << Bits))) : NT(x)));
}

template<size_t Bits, typename OT>
DA_ALWAYS_INLINE constexpr sregister_t sext_r(OT x) noexcept {
	return sext<Bits, sregister_t, OT>(x);
}

template<typename OT>
DA_ALWAYS_INLINE constexpr sregister_t sext_s(OT x) noexcept {
	return sext<IMM_SHORT_BITS, sregister_t, OT>(x);
}

template<typename OT>
DA_ALWAYS_INLINE constexpr sregister_t sext_l(OT x) noexcept {
	return sext<IMM_LONG_BITS, sregister_t, OT>(x);
}

// Error handling

inline void asm_error_v(vm_context_t& context) noexcept {
	fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::red),
			   "From asm_error_v:\n"
			   "ERROR: This function should not be called!\n");
	print_registers(context);
	DAVM_PC(context) = 0; // Halt the PC
}

inline void asm_error_r1(vm_context_t& context, regid_t rd) noexcept {
	fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::red),
			   "From asm_error_r1 (with rd = {}):\n"
			   "ERROR: This function should not be called!\n",
			   rd);
	print_registers(context);
	DAVM_PC(context) = 0; // Halt the PC
}

inline void asm_error_r2(vm_context_t& context, regid_t rd, regid_t ra) noexcept {
	fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::red),
			   "From asm_error_r3 (with rd = {}, ra = {}):\n"
			   "ERROR: This function should not be called!\n",
			   rd, ra);
	print_registers(context);
	DAVM_PC(context) = 0; // Halt the PC
}

inline void asm_error_r3(vm_context_t& context, regid_t rd, regid_t ra, regid_t rb) noexcept {
	fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::red),
			   "From asm_error_r3 (with rd = {}, ra = {}, rb = {}):\n"
			   "ERROR: This function should not be called!\n",
			   rd, ra, rb);
	print_registers(context);
	DAVM_PC(context) = 0; // Halt the PC
}

inline void asm_error_r1i1(vm_context_t& context, regid_t rd, immediate_t imm) noexcept {
	fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::red),
			   "From asm_error_r3 (with rd = {}, imm = {}):\n"
			   "ERROR: This function should not be called!\n",
			   rd, imm);
	print_registers(context);
	DAVM_PC(context) = 0; // Halt the PC
}

inline void asm_error_r2i1(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::red),
			   "From asm_error_r3 (with rd = {}, ra = {}, imm = {}):\n"
			   "ERROR: This function should not be called!\n",
			   rd, ra, imm);
	print_registers(context);
	DAVM_PC(context) = 0; // Halt the PC
}

// Asm functions

inline void asm_nop(DA_MAYBE_UNUSED vm_context_t& context) noexcept { }

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
// MOVQ		rip, [target]
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
	context.x[rd] = sregister_t(context.x[ra]) >> context.x[rb];
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
	context.x[rd] = context.x[ra] + sext_s(imm);
}

inline constexpr void asm_muli(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	context.x[rd] = context.x[ra] * sext_s(imm);
}

inline constexpr void asm_slti(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	context.x[rd] = sregister_t(context.x[ra]) < sext_s(imm);
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

// Load
// 7 bit opcode, 5 bit rd, 5 bit ra, 3 bit opcode2, 12 bit signed offset
inline void asm_lb(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	context.x[rd] = sext_r<BYTE_BITS>(*reinterpret_cast<byte_t*>(context.x[ra] + sext_s(imm)));
}

inline void asm_lh(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	context.x[rd] = sext_r<HWORD_BITS>(*reinterpret_cast<hword_t*>(context.x[ra] + sext_s(imm)));
}

inline void asm_lw(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	context.x[rd] = sext_r<WORD_BITS>(*reinterpret_cast<word_t*>(context.x[ra] + sext_s(imm)));
}

inline void asm_ld(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	context.x[rd] = *reinterpret_cast<dword_t*>(context.x[ra] + sext_s(imm));
}

inline void asm_lbu(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	context.x[rd] = *reinterpret_cast<byte_t*>(context.x[ra] + sext_s(imm));
}

inline void asm_lhu(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	context.x[rd] = *reinterpret_cast<hword_t*>(context.x[ra] + sext_s(imm));
}

inline void asm_lwu(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	context.x[rd] = *reinterpret_cast<word_t*>(context.x[ra] + sext_s(imm));
}

// Save
inline void asm_sb(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	*reinterpret_cast<byte_t*>(context.x[rd]) = byte_t(context.x[ra] + sext_s(imm));
}

inline void asm_sh(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	*reinterpret_cast<hword_t*>(context.x[rd]) = hword_t(context.x[ra] + sext_s(imm));
}

inline void asm_sw(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	*reinterpret_cast<word_t*>(context.x[rd]) = word_t(context.x[ra] + sext_s(imm));
}

inline void asm_sd(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	*reinterpret_cast<dword_t*>(context.x[rd]) = dword_t(context.x[ra] + sext_s(imm));
}

// R2
inline void asm_mov(vm_context_t& context, regid_t rd, regid_t ra) noexcept {
	context.x[rd] = context.x[ra];
}

// R1I1
inline void asm_lui(vm_context_t& context, regid_t rd, immediate_t imm) noexcept {
	context.x[rd] += imm << 12;
}

inline void asm_auipc(vm_context_t& context, regid_t rd, immediate_t imm) noexcept {
	context.x[rd] = DAVM_PC(context) + (imm << 12);
}

inline void asm_jal(vm_context_t& context, regid_t rd, immediate_t imm) noexcept {
	context.x[rd] = DAVM_PC(context);
	DAVM_PC(context) += sext_l(imm) << 1;
}

// Branch
inline void asm_jalr(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	const register_t cra = context.x[ra]; // In case rd == ra
	context.x[rd]		 = DAVM_PC(context);
	DAVM_PC(context)	 = cra + (sext_s(imm) << 1);
}

inline void asm_beq(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	if(context.x[rd] == context.x[ra]) {
		DAVM_PC(context) += sext_s(imm) << 1;
	}
}

inline void asm_bne(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	if(context.x[rd] != context.x[ra]) {
		DAVM_PC(context) += sext_s(imm) << 1;
	}
}

inline void asm_blt(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	if(sregister_t(context.x[rd]) < sregister_t(context.x[ra])) {
		DAVM_PC(context) += sext_s(imm) << 1;
	}
}

inline void asm_bge(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	if(sregister_t(context.x[rd]) >= sregister_t(context.x[ra])) {
		DAVM_PC(context) += sext_s(imm) << 1;
	}
}

inline void asm_bltu(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	if(context.x[rd] < context.x[ra]) {
		DAVM_PC(context) += sext_s(imm) << 1;
	}
}

inline void asm_bgeu(vm_context_t& context, regid_t rd, regid_t ra, immediate_t imm) noexcept {
	if(context.x[rd] >= context.x[ra]) {
		DAVM_PC(context) += sext_s(imm) << 1;
	}
}

// Function tables
// Use DA_MAYBE_UNUSED attribute to avoid warning
// Pad with asm_error_* to reduce compare when use

#define DA_X(big, type, small...) asm_##small,

DA_MAYBE_UNUSED static constexpr asm_func_v_t asm_table_v[] = {
	DA_X_V

	asm_error_v,
	asm_error_v,
	asm_error_v,
	asm_error_v,
	asm_error_v,
	asm_error_v,
};

DA_MAYBE_UNUSED static constexpr asm_func_r1_t asm_table_r1[] = {
	DA_X_R1

	asm_error_r1,
	asm_error_r1,
	asm_error_r1,
	asm_error_r1,
	asm_error_r1,
};

DA_MAYBE_UNUSED static constexpr asm_func_r2_t asm_table_r2[] = {
	DA_X_R2
};

DA_MAYBE_UNUSED static constexpr asm_func_r1i1_t asm_table_r1i1[] = {
	DA_X_R1I1
};

DA_MAYBE_UNUSED static constexpr asm_func_r3_t asm_table_arith[] = {
	DA_X_ARITH

	asm_error_r3,
	asm_error_r3,
	asm_error_r3,
	asm_error_r3,
	asm_error_r3,
	asm_error_r3,
	asm_error_r3,
	asm_error_r3,
	asm_error_r3,
	asm_error_r3,
	asm_error_r3,
	asm_error_r3,
	asm_error_r3,
	asm_error_r3,
};

DA_MAYBE_UNUSED static constexpr asm_func_r2i1_t asm_table_load[] = {
	DA_X_LOAD

	asm_error_r2i1,
};

DA_MAYBE_UNUSED static constexpr asm_func_r2i1_t asm_table_save[] = {
	DA_X_SAVE

	asm_error_r2i1,
	asm_error_r2i1,
	asm_error_r2i1,
	asm_error_r2i1,
};

DA_MAYBE_UNUSED static constexpr asm_func_r2i1_t asm_table_imm[] = {
	DA_X_IMM
	asm_error_r2i1,
};

DA_MAYBE_UNUSED static constexpr asm_func_r2i1_t asm_table_imm_shift[] = {
	DA_X_IMM_SHIFT

	asm_error_r2i1,
};

DA_MAYBE_UNUSED static constexpr asm_func_r2i1_t asm_table_branch[] = {
	DA_X_BRANCH
	asm_error_r2i1,
};

END_DA_NAMESPACE

#endif // _DAVM_COMMON_ASM_H_
