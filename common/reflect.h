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

[[maybe_unused]] static constexpr const char* reg_name[] = {
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

[[maybe_unused]] static constexpr const char* asm_name_v[] = {
	"RET",
	"HLT",

	"ERROR VOID COMMAND",
	"ERROR VOID COMMAND",
	"ERROR VOID COMMAND",
	"ERROR VOID COMMAND",
	"ERROR VOID COMMAND",
	"ERROR VOID COMMAND",
};

[[maybe_unused]] static constexpr const char* asm_name_r1[] = {
	"PUSH",
	"POP",
	"CALL",

	"ERROR R1 COMMAND",
	"ERROR R1 COMMAND",
	"ERROR R1 COMMAND",
	"ERROR R1 COMMAND",
	"ERROR R1 COMMAND",
};

[[maybe_unused]] static constexpr const char* asm_name_arith[] = {
	"ADD",
	"SUB",
	"SLT",
	"SLTU",
	"MUL",
	"MULH",
	"MULHSU",
	"MULHU",
	"DIV",
	"DIVU",
	"REM",
	"REMU",
	"SLL",
	"SRL",
	"SRA",
	"AND",
	"OR",
	"XOR",

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

[[maybe_unused]] static constexpr const char* asm_name_sl[] = {
	"LB",
	"LH",
	"LW",
	"LD",
	"SB",
	"SH",
	"SW",
	"SD",
};

[[maybe_unused]] static constexpr const char* asm_name_imm[] = {
	"ADDI",
	"MULI",
	"SLTI",
	"SLTUI",
	"ANDI",
	"ORI",
	"XORI",

	"ERROR IMM COMMAND",
};

[[maybe_unused]] static constexpr const char* asm_name_imm_shift[] = {
	"SLLI",
	"SRLI",
	"SRAI",

	"ERROR IMM SHIFT COMMAND",
};

// String to enum value

END_DA_NAMESPACE

#endif // _DAVM_COMMON_REFLECT_H_
