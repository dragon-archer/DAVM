/**
 * @file      reflect.h
 * @brief     Reflections for asm commands
 * @version   0.1
 * @author    dragon-archer
 *
 * @copyright Copyright (c) 2022
 */

#ifndef _DAVM_COMMON_REFLECT_H_
#define _DAVM_COMMON_REFLECT_H_

#include <common/pch.h>
#include <common/type.h>

BEGIN_DA_NAMESPACE

// Enum value to string

DA_MAYBE_UNUSED static constexpr const char* reg_name[] = {
	"PC",
	"RA",
	"BP",
	"SP",
	"GP",
	"TP",
	"CP",
	"RV",
	"X08",
	"X09",
	"X10",
	"X11",
	"X12",
	"X13",
	"X14",
	"X15",
	"X16",
	"X17",
	"X18",
	"X19",
	"X20",
	"X21",
	"X22",
	"X23",
	"X24",
	"X25",
	"X26",
	"X27",
	"X28",
	"X29",
	"X30",
	"ZR",
};

#define DA_X(name, ...) #name,

DA_MAYBE_UNUSED static constexpr const char* asm_name_v[] = {
	DA_X_V

	"ERROR VOID COMMAND",
	"ERROR VOID COMMAND",
	"ERROR VOID COMMAND",
	"ERROR VOID COMMAND",
	"ERROR VOID COMMAND",
	"ERROR VOID COMMAND",
};

DA_MAYBE_UNUSED static constexpr const char* asm_name_r1[] = {
	DA_X_R1

	"ERROR R1 COMMAND",
	"ERROR R1 COMMAND",
	"ERROR R1 COMMAND",
	"ERROR R1 COMMAND",
	"ERROR R1 COMMAND",
};

DA_MAYBE_UNUSED static constexpr const char* asm_name_r2[] = {
	DA_X_R2

	"ERROR R2 COMMAND",
	"ERROR R2 COMMAND",
	"ERROR R2 COMMAND",
	"ERROR R2 COMMAND",
	"ERROR R2 COMMAND",
	"ERROR R2 COMMAND",
	"ERROR R2 COMMAND",
};

DA_MAYBE_UNUSED static constexpr const char* asm_name_r2i1[] = {
	DA_X_R1I1

	"ERROR R2I1 COMMAND",
	"ERROR R2I1 COMMAND",
	"ERROR R2I1 COMMAND",
	"ERROR R2I1 COMMAND",
	"ERROR R2I1 COMMAND",
};

DA_MAYBE_UNUSED static constexpr const char* asm_name_arith[] = {
	DA_X_ARITH

	"ERROR ARITH COMMAND",
	"ERROR ARITH COMMAND",
	"ERROR ARITH COMMAND",
	"ERROR ARITH COMMAND",
	"ERROR ARITH COMMAND",
	"ERROR ARITH COMMAND",
	"ERROR ARITH COMMAND",
	"ERROR ARITH COMMAND",
	"ERROR ARITH COMMAND",
	"ERROR ARITH COMMAND",
	"ERROR ARITH COMMAND",
	"ERROR ARITH COMMAND",
	"ERROR ARITH COMMAND",
	"ERROR ARITH COMMAND",
};

DA_MAYBE_UNUSED static constexpr const char* asm_name_load[] = {
	DA_X_LOAD

	"ERROR LOAD COMMAND"
};

DA_MAYBE_UNUSED static constexpr const char* asm_name_save[] = {
	DA_X_SAVE

	"ERROR SAVE COMMAND",
	"ERROR SAVE COMMAND",
	"ERROR SAVE COMMAND",
	"ERROR SAVE COMMAND",
};

DA_MAYBE_UNUSED static constexpr const char* asm_name_imm[] = {
	DA_X_IMM

	"ERROR IMM COMMAND",
};

DA_MAYBE_UNUSED static constexpr const char* asm_name_imm_shift[] = {
	DA_X_IMM_SHIFT

	"ERROR IMM SHIFT COMMAND",
};

#undef DA_X
#define DA_X(name, type, ...) { #name, type << 16 | I_##name },

// String to enum value
DA_MAYBE_UNUSED static const std::map<std::string, uint32_t> asm_id = {
	// clang-format off
	DA_X_V
	DA_X_R1
	DA_X_R2
	DA_X_R1I1
	DA_X_ARITH
	DA_X_LOAD
	DA_X_SAVE
	DA_X_IMM
	DA_X_IMM_SHIFT
	// clang-format on
};

#undef DA_X

END_DA_NAMESPACE

#endif // _DAVM_COMMON_REFLECT_H_
