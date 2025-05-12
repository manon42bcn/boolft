/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_map.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 02:15:31 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/12 21:55:45 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "boolft.hpp"
#include <cstdint>
#include <cmath>
#include <iostream>
#include <limits>

/**
 * @brief Compacts bits from even positions into consecutive lower bits.
 *
 * @details
 * This function reverses the “part1by1” expansion by collapsing every other bit
 * (the even bit positions 0,2,4,…30) of the input word into a contiguous 16-bit value:
 *   1. Masks out all bits except those in even positions (`0x55555555`).
 *   2. Merges adjacent bit pairs with a right shift and mask (`0x33333333`).
 *   3. Merges nibbles with a 2-bit right shift and mask (`0x0F0F0F0F`).
 *   4. Merges bytes with a 4-bit right shift and mask (`0x00FF00FF`).
 *   5. Finally merges the low and high halves with an 8-bit right shift and mask
 *      (`0x0000FFFF`), producing the original 16-bit value in the lower bits.
 *
 * @param[in] n
 *   The 32-bit unsigned integer containing bits in even positions to compact.
 * @return
 *   A 32-bit unsigned integer whose lower 16 bits are the compacted result,
 *   and whose upper 16 bits are zero.
 */
static uint32_t compact1by1(uint32_t n) {
	n &= 0x55555555;
	n = (n | (n >> 1))  & 0x33333333;
	n = (n | (n >> 2))  & 0x0F0F0F0F;
	n = (n | (n >> 4))  & 0x00FF00FF;
	n = (n | (n >> 8))  & 0x0000FFFF;
	return n;
}

/**
 * @brief Decodes a 32-bit Morton (Z-order) code into its original 16-bit x and y.
 *
 * @details
 * Morton (Z-order) codes interleave the bits of two coordinates. This function
 * extracts the original 16-bit `x` and `y` values by:
 *   - Calling `compact1by1` on the code to recover the bits that were placed
 *     in even positions (the original `x`).
 *   - Shifting the code right by one to bring the bits from odd positions
 *     into even positions, then calling `compact1by1` to recover the original `y`.
 *
 * @param[in]  code
 *   The 32-bit Morton code containing interleaved bits of `x` and `y`.
 * @param[out] x
 *   Reference to a `uint32_t` where the recovered X-coordinate (0 ≤ x ≤ 65535) will be stored.
 * @param[out] y
 *   Reference to a `uint32_t` where the recovered Y-coordinate (0 ≤ y ≤ 65535) will be stored.
 */
static void morton_decode(uint32_t code, uint32_t &x, uint32_t &y) {
	x = compact1by1(code);
	y = compact1by1(code >> 1);
}

/**
 * @brief Maps a normalized parameter t ∈ [0,1] back to 2D coordinates (x, y) via Morton decoding.
 *
 * @details
 * This function performs the inverse of the Morton-based mapping:
 *   1. Validates that the input `t` lies within the closed interval [0.0, 1.0].
 *   2. Scales `t` to the full 32-bit Morton code range by multiplying by `0xFFFFFFFF`
 *      and rounding to the nearest integer.
 *   3. Calls `morton_decode` to split the 32-bit code into its original 16-bit X and Y.
 *
 * @param[in]  t
 *   A `double` in [0.0, 1.0] representing the normalized Morton code.
 * @param[out] x
 *   Reference to a `uint32_t` where the decoded X-coordinate (0 ≤ x ≤ 65535) will be stored.
 * @param[out] y
 *   Reference to a `uint32_t` where the decoded Y-coordinate (0 ≤ y ≤ 65535) will be stored.
 *
 * @throws BoolFtException
 *   If `t` is outside the interval [0.0, 1.0], throws with message:
 *   `"Error Value out of range [0,1]."`
 */
void reverse_map(double t, uint32_t &x, uint32_t &y) {
	if (!(t >= 0.0 && t <= 1.0)) {
		throw BoolFtException("Error Value out of range [0,1].");
	}
	uint32_t code = uint32_t(std::round(t * 0xFFFFFFFFu));
	morton_decode(code, x, y);
}

