/**
 * @file      pch.h
 * @brief     Precompiled header
 * @version   0.1
 * @author    dragon-archer
 *
 * @copyright Copyright (c) 2022
 */

#ifndef _DAVM_COMMON_PCH_H_
#define _DAVM_COMMON_PCH_H_

// Turn off clang format to keep indent inside macros
// clang-format off

// Headers
#include <cassert>
#include <cstdint>
#include <map>
#include <string>

// Namespace
#ifndef NO_DA_NAMESPACE
	#ifndef DA_NAMESPACE
		#define DA_NAMESPACE da
	#endif
	#ifndef BEGIN_DA_NAMESPACE
		#define BEGIN_DA_NAMESPACE namespace da {
	#endif
	#ifndef END_DA_NAMESPACE
		#define END_DA_NAMESPACE }
	#endif
	#ifndef _DA_NS
		#define _DA_NS ::da::
	#endif
#endif

// Detect C++ version
// TODO: Add MSVC support
#if __cplusplus < 201703L
	#error "DA VM must be compiled with at least C++ 17"
#elif __cplusplus < 202002L
	#define DA_CPP_17 1
	#define DA_CPP_20 0
#else
	#define DA_CPP_17 1
	#define DA_CPP_20 1
#endif

// Feature-test macros
#if defined(__has_builtin)
	#define DA_HAS_BUILTIN(x) __has_builtin(x)
#else
	#define DA_HAS_BUILTIN(x) 0
#endif

#if defined(__has_cpp_attribute)
	#define DA_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
	#define DA_HAS_CPP_ATTRIBUTE(x) 0
#endif

#if defined(__has_include)
	#define DA_HAS_INCLUDE(x) __has_include(x)
#else
	#define DA_HAS_INCLUDE(x) 0
#endif

// If macros
#if DA_HAS_BUILTIN(__builtin_expect)
	#define DA_IF_LIKELY(x)   if(__builtin_expect(!!(x), 1))
	#define DA_IF_UNLIKELY(x) if(__builtin_expect(!!(x), 0))
#elif DA_HAS_CPP_ATTRIBUTE(likely)
	#define DA_IF_LIKELY(x)   if(x) [[likely]]
	#define DA_IF_UNLIKELY(x) if(x) [[unlikely]]
#else
	#define DA_IF_LIKELY(x)   if(x)
	#define DA_IF_UNLIKELY(x) if(x)
#endif

// Conditional headers

#if DA_HAS_INCLUDE(<format>) // Try to use standard format first
	#include <format>
	BEGIN_DA_NAMESPACE
	namespace fmt {
		using std::format;
	}
	END_DA_NAMESPACE
#elif DA_HAS_INCLUDE(<fmt/format.h>) // Then try to use system installed / user specified
	#define FMT_HEADER_ONLY
	#define FMT_STATIC_THOUSANDS_SEPARATOR // Avoid include <locale>
	#include <fmt/format.h>
	#include <fmt/color.h>
	BEGIN_DA_NAMESPACE
	namespace fmt = ::fmt;
	END_DA_NAMESPACE
#elif DA_HAS_INCLUDE(<thirdparty/fmt/include/fmt/format.h>) // Otherwise try to directly use fmtlib
	#define FMT_HEADER_ONLY
	#define FMT_STATIC_THOUSANDS_SEPARATOR // Avoid include <locale>
	#include <thirdparty/fmt/include/fmt/format.h>
	#include <thirdparty/fmt/include/fmt/color.h>
	BEGIN_DA_NAMESPACE
	namespace fmt = ::fmt;
	END_DA_NAMESPACE
#else
	#error "No format library provided, try to make the compiler find <format> or <fmt/format.h>"
#endif

// Optional headers

// Detect endian
#if DA_HAS_INCLUDE(<bit>) && DA_CPP_20 // Has <bit> & std::endian
	#include <bit>
	#define DA_BIG_ENDIAN (std::endian::native == std::endian::big)
	#define DA_LITTLE_ENDIAN (std::endian::native == std::endian::little)
#elif defined(__BYTE_ORDER__) // No C++ 20 support, try to use macro directly
	#define DA_BIG_ENDIAN (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
	#define DA_LITTLE_ENDIAN (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#else // TODO: Use other method to detect endian instead of assuming little endian
	#define DA_BIG_ENDIAN 0
	#define DA_LITTLE_ENDIAN 1
#endif

BEGIN_DA_NAMESPACE

using ::std::int8_t;
using ::std::int16_t;
using ::std::int32_t;
using ::std::int64_t;

using ::std::uint8_t;
using ::std::uint16_t;
using ::std::uint32_t;
using ::std::uint64_t;

END_DA_NAMESPACE

// Restore clang format
// clang-format on

#endif // _DAVM_COMMON_PCH_H_
