/**
 * @file      log.h
 * @brief     Print logs
 * @version   0.1
 * @author    dragon-archer
 *
 * @copyright Copyright (c) 2022
 */

#ifndef _DAVM_COMMON_LOG_H_
#define _DAVM_COMMON_LOG_H_

#include <common/pch.h>
#include <common/reflect.h>
#include <common/type.h>

BEGIN_DA_NAMESPACE

enum loglevel {
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
};

#ifdef NDEBUG
#define DEFAULT_LOG_LEVEL LOG_WARNING
#else
#define DEFAULT_LOG_LEVEL LOG_DEBUG
#endif

inline void print_registers(vm_context_t& context) {
	std::string s = "Special registers:\n";
	s += fmt::format("PC  : {0:#018X}\t", DAVM_PC(context));
	s += fmt::format("RA  : {0:#018X}\t", DAVM_RA(context));
	s += fmt::format("BP  : {0:#018X}\t", DAVM_BP(context));
	s += fmt::format("SP  : {0:#018X}\n", DAVM_SP(context));
	s += fmt::format("GP  : {0:#018X}\t", DAVM_GP(context));
	s += fmt::format("TP  : {0:#018X}\t", DAVM_TP(context));
	s += fmt::format("CP  : {0:#018X}\t", DAVM_CP(context));
	s += fmt::format("RV  : {0:#018X}\n", DAVM_RV(context));
	s += fmt::format("Normal registers:\n");
	for(int i = 8; i < 32; ++i) {
		s += fmt::format("X{0:02} : {1:#018X}", i, context.x[i]);
		s += ((i + 1) & 0b11) ? '\t' : '\n';
	}
	// Buffer the output
	std::printf("%s", s.c_str());
}

inline std::string dissemble_command(uint32_t code) {
	std::string ret = fmt::format("{0:#010X}\t", code);
	switch(code & 0x7F) {
	case I_G_ARITH: {
		const asm_cmd_r3_t cmd = *DAVM_CAST(asm_cmd_r3_t*, &code);
		ret += fmt::format("{0}\t{1}, {2}, {3}\n", asm_name_arith[cmd.op2], reg_name[cmd.rd], reg_name[cmd.ra], reg_name[cmd.rb]);
		break;
	}
	case I_G_SL: {
		const asm_cmd_r2i1_t cmd = *DAVM_CAST(asm_cmd_r2i1_t*, &code);
		ret += fmt::format("{0}\t{1}, {2}, {3}\n", asm_name_sl[cmd.op2], reg_name[cmd.rd], reg_name[cmd.ra], cmd.imm);
		break;
	}
	case I_G_IMM: {
		const asm_cmd_r2i1_t cmd = *DAVM_CAST(asm_cmd_r2i1_t*, &code);
		DA_IF_UNLIKELY(cmd.op2 == I_G_IMM_SHIFT) {
			const asm_cmd_imm_shift_t cmd = *DAVM_CAST(asm_cmd_imm_shift_t*, &code);
			ret += fmt::format("{0}\t{1}, {2}, {3}\n", asm_name_imm_shift[cmd.op2], reg_name[cmd.rd], reg_name[cmd.ra], cmd.imm);
		} else {
			ret += fmt::format("{0}\t{1}, {2}, {3}\n", asm_name_imm[cmd.op2], reg_name[cmd.rd], reg_name[cmd.ra], cmd.imm);
		}
		break;
	}
	default: { // Deal with unique id
		if(code & 0x08) { // void call
			const asm_cmd_v_t cmd = *DAVM_CAST(asm_cmd_v_t*, &code);
			ret += asm_name_v[cmd.op2];
		} else if(code & 0x10) { // r1 call
			const asm_cmd_r1_t cmd = *DAVM_CAST(asm_cmd_r1_t*, &code);
			ret += fmt::format("{0}\t{1}\n", asm_name_r1[cmd.op2], reg_name[cmd.rd]);
		} else {
			ret += "ERROR COMMAND\n";
		}
	}
	}
	return ret;
}

END_DA_NAMESPACE

#endif // _DAVM_COMMON_LOG_H_
