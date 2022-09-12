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
	const word_t rd = (code >> 7) & 0x1F; // Bit 11 - 7 is rd
	switch(code & 0x7F) { // Bit 6 - 0 is opcode
	case I_G_ARITH: {
		const word_t ra	 = (code >> 12) & 0x1F; // Bit 16 - 12 is ra
		const word_t rb	 = (code >> 17) & 0x1F; // Bit 21 - 17 is rb
		const word_t op2 = (code >> 22) & 0x1F; // Bit 26 - 22 is op2
		// Bit 31 - 27 is unused
		asm_table_arith[op2](m_context, rd, ra, rb);
		return 0;
	}
	case I_G_SL: {
		const word_t	  ra  = (code >> 12) & 0x1F; // Bit 16 - 12 is ra
		const word_t	  op2 = (code >> 17) & 0x07; // Bit 19 - 17 is op2
		const immediate_t off = code >> 20; // Bit 31 - 20 is off
		asm_table_sl[op2](m_context, rd, ra, off);
		return 0;
	}
	case I_G_IMM: {
		const word_t ra	 = (code >> 12) & 0x1F; // Bit 16 - 12 is ra
		const word_t op2 = (code >> 17) & 0x07; // Bit 19 - 17 is op2
		DA_IF_UNLIKELY(op2 == I_G_IMM_SHIFT) {
			const word_t	  op3 = (code >> 20) & 0x03; // Bit 21 - 20 is op3
			const immediate_t imm = code >> 22; // Bit 31 - 22 is imm
			asm_table_imm_shift[op3](m_context, rd, ra, imm);
		} else {
			const immediate_t imm = code >> 20; // Bit 31 - 20 is imm
			asm_table_imm[op2](m_context, rd, ra, imm);
		}
		return 0;
	}
	default: { // Deal with unique id
		if(code & 0x08) { // void call
		}
	}
	}
	return 0;
}

END_DA_NAMESPACE
