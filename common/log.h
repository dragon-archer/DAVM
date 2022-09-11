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

inline void print_vm(vm_context_t& context) {

}

END_DA_NAMESPACE

#endif // _DAVM_COMMON_LOG_H_
