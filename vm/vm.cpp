/**
 * @file      vm.cpp
 * @brief     Implemention of VM
 * @version   0.1
 * @author    dragon-archer
 *
 * @copyright Copyright (c) 2022
 */

#include <vm/pch.h>
#include <vm/vm.h>

BEGIN_DA_NAMESPACE

// Init the stack layout to
// |value |offset|
// |------|  0x0 | <- m_memory.end()
// |0x0000| -0x8 | <- rsp - 0x8			will be considered as saved pc by the vm
// |0x0000| -0x10| <- rbp, rsp			will be considered as saved rbp by the vm
// So that the VM will be automated halt when all programs end
void VM::init_stack() noexcept {
	std::memset(&m_context, 0, sizeof(m_context));
	DAVM_BP(m_context) = DAVM_CAST(register_t, &*(m_memory.end() - 2 * sizeof(register_t)));
	DAVM_SP(m_context) = DAVM_BP(m_context);
	DAVM_PC(m_context) = 0; // Set pc to 0 to avoid start before load
	DAVM_ZR(m_context) = 0; // Clear zero register

	*DAVM_CAST(register_t*, DAVM_SP(m_context) + sizeof(register_t)) = 0;
	*DAVM_CAST(register_t*, DAVM_SP(m_context))						 = 0;
}

int VM::one_step() noexcept {
	// Avoid execute outside program
	DA_IF_UNLIKELY((DAVM_PC(m_context) - DAVM_CAST(register_t, &*m_program.begin()))
				   >= (DAVM_CAST(register_t, &*m_program.end()) - DAVM_CAST(register_t, &*m_program.begin()))) {
		return 1;
	}
	word_t code = *DAVM_CAST(register_t*, DAVM_PC(m_context));
	DAVM_PC(m_context) += sizeof(word_t);
	switch(code & 0x7F) { // Bit 6 - 0 is opcode
	case I_G_ARITH: {
		const asm_cmd_r3_t cmd = *DAVM_CAST(asm_cmd_r3_t*, &code);
		asm_table_arith[cmd.op2](m_context, cmd.rd, cmd.ra, cmd.rb);
		return 0;
	}
	case I_G_LOAD: {
		const asm_cmd_r2i1_t cmd = *DAVM_CAST(asm_cmd_r2i1_t*, &code);
		asm_table_load[cmd.op2](m_context, cmd.rd, cmd.ra, cmd.imm);
		return 0;
	}
	case I_G_SAVE: {
		const asm_cmd_r2i1_t cmd = *DAVM_CAST(asm_cmd_r2i1_t*, &code);
		asm_table_save[cmd.op2](m_context, cmd.rd, cmd.ra, cmd.imm);
		return 0;
	}
	case I_G_IMM: {
		const asm_cmd_r2i1_t cmd = *DAVM_CAST(asm_cmd_r2i1_t*, &code);
		DA_IF_UNLIKELY(cmd.op2 == I_G_IMM_SHIFT) {
			const asm_cmd_imm_shift_t cmd = *DAVM_CAST(asm_cmd_imm_shift_t*, &code);
			asm_table_imm_shift[cmd.op3](m_context, cmd.rd, cmd.ra, cmd.imm);
		} else {
			asm_table_imm[cmd.op2](m_context, cmd.rd, cmd.ra, cmd.imm);
		}
		return 0;
	}
	default: { // Deal with unique id
		if(code & 0x08) { // void call
			const asm_cmd_v_t cmd = *DAVM_CAST(asm_cmd_v_t*, &code);
			asm_table_v[cmd.op2](m_context);
			return 0;
		} else if(code & 0x10) { // r1 call
			const asm_cmd_r1_t cmd = *DAVM_CAST(asm_cmd_r1_t*, &code);
			asm_table_r1[cmd.op2](m_context, cmd.rd);
			return 0;
		} else {
			return 2;
		}
	}
	}
	return 1;
}

END_DA_NAMESPACE
