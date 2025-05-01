/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:49:54 by mporras-          #+#    #+#             */
/*   Updated: 2025/04/30 17:20:03 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "readySetBool.h"
#include "../boolft/inc/boolft.h"

void print_binary(unsigned int n) {
	// Número de bits en un entero (usualmente 32 bits en una máquina moderna)
	int num_bits = sizeof(n) * 8;

	// Iterar sobre cada bit, de izquierda a derecha
	for (int i = num_bits - 1; i >= 0; i--) {
		// Imprimir el bit más significativo primero
		if ((n >> i) & 1) {
			printf("1");
		} else {
			printf("0");
		}
	}
	printf("\n");
}

int main () {
	// printf("%d\n", adder(40, 2));
	// printf("%d\n", adder(0, 0));
	// printf("%d\n", adder(255555, 25555));
	// printf("%d\n", adder(9, 1));
	// printf("%d\n", adder(99, 99));
	// printf("%d\n", multiplier(7, 3));
	// printf("%d\n", gray_code(7));
	tree_try();
}
