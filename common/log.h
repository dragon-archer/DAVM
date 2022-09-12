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

inline void print_registers(vm_context_t& context) noexcept {
	std::string s;
	s += fmt::format("Special registers:\n");
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

END_DA_NAMESPACE

#endif // _DAVM_COMMON_LOG_H_
