/**
 * @file      base64.h
 * @brief     Encoder and decoder for base64
 * @version   0.1
 * @author    dragon-archer
 *
 * @copyright Copyright (c) 2022
 */

#ifndef _DAVM_COMMON_BASE64_H_
#define _DAVM_COMMON_BASE64_H_

#include <common/pch.h>

BEGIN_DA_NAMESPACE

inline static constexpr char b64padding_char = '=';

static constexpr char b64table_enc[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
										 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
										 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
										 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };

// clang-format off
static constexpr uint8_t b64table_dec[] = { // 0xFF is invalid, 0x7F is padding, 0x00 - 0x3F is valid
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F, // '+' & '/'
	0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, // '0' - '9' & '='
	0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, // 'A' - 'O'
	0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 'P' - 'Z'
	0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, // 'a' - 'o'
	0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 'p' - 'z'
	// Just for compatibility
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};
// clang-format on

/**
 * @brief  Calculated the length of the stream after encoding
 * @param  original The original length of the stream
 * @return The length of the encoded stream
 * @note   This function DO NOT actually do the encoding, but just calculate the length, so call to it is O(1)
 */
inline constexpr size_t b64enc_len(size_t original) noexcept {
	return ((original + 2) / 3) << 2; // ceil(original / 3) * 4
}

/**
 * @brief  Calculated the length of the stream after decoding
 * @param  encoded The length of the encoded stream
 * @return The maximum possible length of the decoded stream
 * @note   This function DO NOT actually do the decoding, but just calculate the length, so call to it is O(1)
 * @note   The decoded string may not take up so much length
 */
inline constexpr size_t b64dec_len(size_t encoded) noexcept {
	return (encoded >> 1) + (encoded >> 2); // encoded / 4 * 3
}

/**
 * @brief  Encode at most @param n bytes from @param src to @param dest
 * @param  dest The place where the result writes to
 * @param  src  The source data stream to encode
 * @param  n    Length of the data stream
 * @note   Make sure there are enough space in @param dest, you may use @ref b64enc_len to calculate the space
 * @note   It's recommended that @param dest is at least 4-byte aligned, otherwise it may be slow or even fail in some rare cases
 */
constexpr void b64enc(char* dest, const uint8_t* src, size_t n) noexcept {
	uint32_t code = 0;
	while(n > 2) {
		n -= 3;
		code = (*src << 16)
			| (*(src + 1) << 8)
			| *(src + 2);
		src += 3;
		// Encode 4 bytes at one step, check the endian
		if constexpr(DA_LITTLE_ENDIAN) {
			*(uint32_t*)dest = (b64table_enc[code & 0x3F] << 24)
				| (b64table_enc[(code >> 6) & 0x3F] << 16)
				| (b64table_enc[(code >> 12) & 0x3F] << 8)
				| b64table_enc[code >> 18];
		} else {
			*(uint32_t*)dest = (b64table_enc[code >> 18] << 24)
				| (b64table_enc[(code >> 12) & 0x3F] << 16)
				| (b64table_enc[(code >> 6) & 0x3F] << 8)
				| b64table_enc[code & 0x3F];
		}
		dest += 4;
	}
	if(n == 1) {
		*dest = b64table_enc[*src >> 2];
		++dest;
		*dest = b64table_enc[(*src & 0x03) << 4];
		++dest;
		// 2 padding chars
		*dest = b64padding_char;
		++dest;
		*dest = b64padding_char;
	} else if(n == 2) {
		code = (uint32_t(*src) << 8)
			| uint32_t(*(src + 1));
		*dest = b64table_enc[code >> 10];
		++dest;
		*dest = b64table_enc[(code >> 4) & 0x3F];
		++dest;
		*dest = b64table_enc[(code & 0x0F) << 2];
		++dest;
		// 1 padding char
		*dest = b64padding_char;
	}
}

/**
 * @brief  Decode at most @param n chars from @param src to @param dest
 * @param  dest The place where the result writes to
 * @param  src  The encoded stream
 * @param  n    Length of the encoded stream
 * @return The real count that writes to @param dest
 * @retval -1 Unexpected end of stream
 * @retval -2 Unexpected token
 * @note   Make sure there are enough space in @param dest, you may use @ref b64dec_len to calculate the space
 */
constexpr size_t b64dec(uint8_t* dest, const char* src, size_t n) noexcept {
	DA_IF_UNLIKELY(n & 0x03) { // n should be dividable by 4
		return -1;
	}
	size_t ret = (n >> 2) + (n >> 1);
	uint32_t code = 0;
	while(n > 4) {
		n -= 4;
		// Decode 4 bytes at one step
		code = (b64table_dec[uint8_t(*src)] << 24)
			| (b64table_dec[uint8_t(*(src + 1))] << 16)
			| (b64table_dec[uint8_t(*(src + 2))] << 8)
			| b64table_dec[uint8_t(*(src + 3))];
		// Either invalid or padding, both should not occur here
		DA_IF_UNLIKELY(code & 0xC0C0C0C0UL) {
			return -2;
		}
		src += 4;
		*dest = (code >> 22) | ((code & 0x300000) >> 20);
		++dest;
		*dest = ((code & 0x0F0000) >> 12) | ((code & 0x3C00) >> 10);
		++dest;
		*dest = ((code & 0x0300) >> 2) | (code & 0x3F);
		++dest;
	}
	// Last 4 byte, candidate padding char
	code = (b64table_dec[uint8_t(*src)] << 24)
		| (b64table_dec[uint8_t(*(src + 1))] << 16)
		| (b64table_dec[uint8_t(*(src + 2))] << 8)
		| b64table_dec[uint8_t(*(src + 3))];
	// Padding char should only occur at last 2 byte
	DA_IF_UNLIKELY(code & 0xC0C08080UL) {
		return -2;
	}
	*dest = (code >> 22) | ((code & 0x300000) >> 20);
	if(code & 0x4000) {
		// 2nd byte not padded by 0 or 4th byte not padding char
		DA_IF_UNLIKELY((code & 0x0F0000) || !(code & 0x40)) {
			return -2;
		}
		return ret - 2;
	}
	++dest;
	*dest = ((code & 0x0F0000) >> 12) | ((code & 0x3C00) >> 10);
	if(code & 0x40) {
		// 3rd byte not padded by 0
		DA_IF_UNLIKELY(code & 0x0300) {
			return -2;
		}
		return ret - 1;
	}
	++dest;
	*dest = ((code & 0x0300) >> 2) | (code & 0x3F);
	return ret;
}

END_DA_NAMESPACE

#endif // _DAVM_COMMON_BASE64_H_
