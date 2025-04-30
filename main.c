/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:49:54 by mporras-          #+#    #+#             */
/*   Updated: 2025/04/30 15:51:25 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int adder(int x, int y) {
	while (y != 0) {
		int carry = x & y;    // Encuentra los bits de acarreo
		x = x ^ y;            // Suma sin acarreo
		y = carry << 1;       // Desplaza el acarreo a la izquierda
	}
	return x;
}

int main () {
  printf("%d\n", adder(40, 2));
  printf("%d\n", adder(0, 0));
  printf("%d\n", adder(255555, 25555));
  printf("%d\n", adder(9, 1));
  printf("%d\n", adder(99, 99));
}
