/**
 * @file      vm.h
 * @brief     Declartion of VM
 * @version   0.1
 * @author    dragon-archer
 *
 * @copyright Copyright (c) 2022
 */

#ifndef _DAVM_VM_VM_H_
#define _DAVM_VM_VM_H_

#include <vm/pch.h>

BEGIN_DA_NAMESPACE

inline constexpr size_t VM_DEFAULT_MEMORY = 64 * 1024 * 1024; // 64M

class VM {
	using string_t = std::string;
	using array_t  = std::vector<byte_t>;

private:
	vm_context_t m_context; // Internal context
	array_t		 m_program; // Program byte code
	array_t		 m_memory; // Memory, shared by heap and stack
	array_t		 m_rodata; // Read only data

public:
	VM()
		: m_memory(VM_DEFAULT_MEMORY) {
		init_stack();
	}

	bool load(string_t filename);

	int run(size_t target = 0);

public: // Access
	vm_context_t& context() noexcept {
		return m_context;
	}

	array_t& program() noexcept {
		return m_program;
	}

	array_t& memory() noexcept {
		return m_memory;
	}

	array_t& rodata() noexcept {
		return m_rodata;
	}

public: //
	/**
	 * @brief  Execute one instruction
	 * @return Excuete status
	 * @retval 0 Success
	 * @retval 1 Error: pc out of program
	 * @retval 2 Error: invalid code
	 */
	int one_step() noexcept;

private:
	void init_stack() noexcept;
};

END_DA_NAMESPACE

#endif // _DAVM_VM_VM_H_
