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
	print_truth_table("AB&C|");
	// printf("%d\n", adder(40, 2));
	// printf("%d\n", adder(0, 0));
	// printf("%d\n", adder(255555, 25555));
	// printf("%d\n", adder(9, 1));
	// printf("%d\n", adder(99, 99));
	// printf("%d\n", multiplier(7, 3));
	// printf("%d\n", gray_code(7));
	// printf("%s \n", BOOL_TO_STRING(eval_formula("1!")));
	// printf("%s \n", BOOL_TO_STRING(eval_formula("0!")));
	// printf("1! = %s\n", BOOL_TO_STRING(eval_formula("1!")));
	// printf("0! = %s\n", BOOL_TO_STRING(eval_formula("0!")));
	// printf("10& = %s\n", BOOL_TO_STRING(eval_formula("10&")));
	// printf("01| = %s\n", BOOL_TO_STRING(eval_formula("01|")));
	// printf("10^ = %s\n", BOOL_TO_STRING(eval_formula("10^")));
	// printf("11> = %s\n", BOOL_TO_STRING(eval_formula("11>")));
	// printf("10= = %s\n", BOOL_TO_STRING(eval_formula("10=")));
	// printf("101|& = %s\n", BOOL_TO_STRING(eval_formula("101|&")));
	// printf("10!& = %s\n", BOOL_TO_STRING(eval_formula("10!&")));
	// printf("11&! = %s\n", BOOL_TO_STRING(eval_formula("11&!")));
	// printf("011|! = %s\n", BOOL_TO_STRING(eval_formula("011|!")));
	// printf("10>! = %s\n", BOOL_TO_STRING(eval_formula("10>!")));
	// printf("10|1& = %s\n", BOOL_TO_STRING(eval_formula("10|1&")));
	// printf("101&| = %s\n", BOOL_TO_STRING(eval_formula("101&|")));
	// printf("101|!& = %s\n", BOOL_TO_STRING(eval_formula("101|!&")));
	// printf("10!| = %s\n", BOOL_TO_STRING(eval_formula("10!|")));
	// printf("110&> = %s\n", BOOL_TO_STRING(eval_formula("110&>")));
	// printf("110|= = %s\n", BOOL_TO_STRING(eval_formula("110|=")));
	// printf("101&^ = %s\n", BOOL_TO_STRING(eval_formula("101&^")));
	// printf("110|!& = %s\n", BOOL_TO_STRING(eval_formula("110|!&")));
	// printf("1|!& = %s\n", BOOL_TO_STRING(eval_formula("1|!&")));
	return (0);
}
