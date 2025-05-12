/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 02:13:18 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/12 12:57:05 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "boolft.hpp"
#include <cstdint>
#include <iostream>
#include <limits>

// Expande los 16 bits de 'n' intercalándolos con ceros:
//   bit pattern 0bfedcba9876543210 → 0b0f0e0d0c0b0a09080706050403020100
static uint32_t part1by1(uint32_t n) {
	n &= 0x0000FFFF;                       // nos aseguramos de 16 bits
	n = (n | (n << 8))  & 0x00FF00FF;
	n = (n | (n << 4))  & 0x0F0F0F0F;
	n = (n | (n << 2))  & 0x33333333;
	n = (n | (n << 1))  & 0x55555555;
	return n;
}

// Intercala los bits de x e y para obtener un Morton code de 32 bits.
static uint32_t mortonInterleave(uint32_t x, uint32_t y) {
	return (part1by1(y) << 1) | part1by1(x);
}

// Prototipo: mapea (x,y) ∈ [0,2^16−1]^2  → t ∈ [0,1]
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
