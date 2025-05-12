/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 02:13:18 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/12 21:38:53 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "boolft.hpp"
#include <cstdint>
#include <iostream>
#include <limits>

/**
 * @brief Spreads the lower 16 bits of an integer into the even bit positions of a 32-bit word.
 *
 * @details
 * This function takes a 16-bit value in the lower half of `n` and "expands" it so that
 * each original bit occupies every other bit (the even positions) in the returned 32-bit
 * result.  It does so by a sequence of bitwise masks and shifts:
 *   1. Mask out all but the lowest 16 bits.
 *   2. Spread each group of 8 bits into 16 bits, then mask.
 *   3. Spread each group of 4 bits into 8 bits, then mask.
 *   4. Spread each group of 2 bits into 4 bits, then mask.
 *   5. Spread each bit into the even positions (bit 0,2,4,...) of the result.
 *
 * This operation is the core primitive for computing Morton (Z-order) codes.
 *
 * @param[in] n
 *   A 32-bit unsigned integer whose lower 16 bits will be interleaved with zeros.
 * @return
 *   A 32-bit unsigned integer where the original 16 bits of `n` occupy the even bit
 *   positions (bits 0,2,4,...,30).  All odd bit positions are zero.
 */
static uint32_t part1by1(uint32_t n) {
	n &= 0x0000FFFF;
	n = (n | (n << 8)) & 0x00FF00FF;
	n = (n | (n << 4)) & 0x0F0F0F0F;
	n = (n | (n << 2)) & 0x33333333;
	n = (n | (n << 1)) & 0x55555555;
	return n;
}

/**
 * @brief Computes the 32-bit Morton (Z-order) code for two 16-bit coordinates.
 *
 * @details
 * Morton codes interleave the bits of two coordinates so that the resulting value
 * preserves spatial locality when used as a one-dimensional index.  This function
 * interleaves the bits of `x` and `y` by:
 *   - Spreading the bits of `y` into the odd bit positions (via `part1by1(y) << 1`).
 *   - Spreading the bits of `x` into the even bit positions (via `part1by1(x)`).
 *   - Combining them with a bitwise OR.
 *
 * The result is a 32-bit integer where bits from `x` and `y` alternate.
 *
 * @param[in] x
 *   The X-coordinate (0 ≤ x ≤ 0xFFFF).
 * @param[in] y
 *   The Y-coordinate (0 ≤ y ≤ 0xFFFF).
 * @return
 *   A 32-bit Morton code with interleaved bits of `x` and `y`.
 */
static uint32_t mortonInterleave(uint32_t x, uint32_t y) {
	return (part1by1(y) << 1) | part1by1(x);
}

/**
 * @brief Maps a 2D point into the unit interval [0,1] via Morton encoding.
 *
 * @details
 * This function:
 *   1. Validates that both `x` and `y` fit in 16 bits (0 to 65535).
 *   2. Computes their 32-bit Morton code using `mortonInterleave()`.
 *   3. Normalizes the code to a double in the range [0,1], by dividing by the maximum
 *      32-bit unsigned value (0xFFFFFFFF).
 *
 * This mapping yields a continuous real number that preserves some notion of
 * spatial locality from the 2D plane.
 *
 * @param[in] x
 *   The X-coordinate (must be 0 ≤ x ≤ 65535).
 * @param[in] y
 *   The Y-coordinate (must be 0 ≤ y ≤ 65535).
 * @return
 *   A `double` in [0,1] corresponding to the Morton-encoded and normalized value.
 *
 * @throws BoolFtException
 *   If either `x` or `y` exceeds 65535, throws with the message:
 *   `"Error. Coordinates out of range: [0, 65535]"`
 */
double map(uint32_t x, uint32_t y) {
	const uint32_t MAX = (1u << 16) - 1;
	if (x > MAX || y > MAX) {
		std::ostringstream detail;
		detail << "Error. Coordinates out of range: [0, " << MAX << "]";
		throw BoolFtException(detail.str());
	}
	uint32_t code = mortonInterleave(x, y);
	return double(code) / double(0xFFFFFFFFu);
}
