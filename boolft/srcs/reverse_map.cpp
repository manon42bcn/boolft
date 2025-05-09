/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_map.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 02:15:31 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/09 02:16:37 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "boolft.hpp"
#include <cstdint>
#include <cmath>
#include <iostream>
#include <limits>

// --- Helpers de deinterleaving ---

// Compacta los bits impares de 'n' en los bits contiguos de menor peso:
//   toma 0b0f0e0d0c0b0a09080706050403020100 → 0bfedcba9876543210
static uint32_t compact1by1(uint32_t n) {
	n &= 0x55555555;                   // extrae solo los bits en posiciones pares
	n = (n | (n >> 1))  & 0x33333333;
	n = (n | (n >> 2))  & 0x0F0F0F0F;
	n = (n | (n >> 4))  & 0x00FF00FF;
	n = (n | (n >> 8))  & 0x0000FFFF;
	return n;
}

// Dado el Morton code de 32 bits, retorna el par (x,y)
static void mortonDecode(uint32_t code, uint32_t &x, uint32_t &y) {
	x = compact1by1(code);       // bits en posiciones pares → x
	y = compact1by1(code >> 1);  // bits en posiciones impares → y
}

// --- Función inversa completa ---
// Toma t ∈ [0,1], lo convierte a código Morton y luego extrae (x,y).
// Comportamiento indefinido si t no está en [0,1], pero avisamos por stderr.
void f_inv(double t, uint32_t &x, uint32_t &y) {
	if (!(t >= 0.0 && t <= 1.0)) {
		std::cerr << "Error: valor fuera de rango [0,1]\n";
		x = y = std::numeric_limits<uint32_t>::max();
		return;
	}
	// Reconstruimos el código de 32 bits
	uint32_t code = uint32_t(std::round(t * 0xFFFFFFFFu));
	mortonDecode(code, x, y);
}

//// --- Ejemplo de uso ---
//int main() {
//	// Pongamos el ejemplo inverso de la llamada anterior
//	double t = 0.000874312;  // por ejemplo, el valor que imprimimos antes
//	uint32_t x, y;
//	f_inv(t, x, y);
//	std::cout << "f_inv(" << t << ") = (" << x << ", " << y << ")\n";
//	return 0;
//}
