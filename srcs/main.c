/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:49:54 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/05 11:30:47 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "readySetBool.h"
#include "../boolft/inc/boolft.h"

void print_binary(int n) {
	// Número de bits en un entero (usualmente 32 bits en una máquina moderna)
	int num_bits = sizeof(int) * 8;
	int count = 0;
	// Iterar sobre cada bit, de izquierda a derecha
	for (int i = num_bits - 1; i >= 0; i--) {
		// Imprimir el bit más significativo primero
		if ((n >> i) & 1) {
			printf("1");
		} else {
			printf("0");
		}
		count++;
	}
	printf("\n");
	printf("TOTAL %d\n", count);
}

int		get_flag(char *flag) {
	static char* available[] = {
							"--adder", "--multiplier",
							"--graycode", "--function",
							"--truth-table", NULL};
	for (int i = 0; available[i]; i++) {
		if (ft_match_cmp(flag, available[i])) {
			return (i);
		}
	}
	return (-1);
}

void	entrypoint(int argc, char *argv[]) {
	static t_entry entrypoint[] = {
					&adder_entrypoint, &multiplier_entrypoint,
					&gray_code_entrypoint, &eval_function_entrypoint,
					&truth_table_entrypoint};
	int index = get_flag(argv[1]);
	if (index == -1) {
		ft_putstr_fd("Flag not found\n", STDERR_FILENO);
		exit(1);
	}
	entrypoint[index](argc, argv);
}

int main (int argc, char *argv[]) {
	if (argc < 2) {
		ft_putstr_fd("Not enough arguments to run\n", STDERR_FILENO);
		exit (1);
	}
	// printf("%s\n", argv[1]);
	entrypoint(argc, argv);
	// print_truth_table("AB&A|");
	return (0);
}
