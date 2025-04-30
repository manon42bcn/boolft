/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:28:47 by mporras-          #+#    #+#             */
/*   Updated: 2025/04/30 17:35:57 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "boolft.h"

/**
 * @brief Bitwise add function.
 * 
 * @note how it works?
 *	Basically works as any other add method, but over a 2 base, instead of
 *	the "normal" decimal one, e.g:
 *		1 + 1 => 01b + 01b
 *		carry = get how much we should carry if the add cannot be represented
 *				with only one digit.
 *		carry = 0      1b
 *				0      1b AND (&)
 *				false|true
 *				---------
 *		carry =	0      1b
 *
 *	Then a      0      1b
 *		 b      0      1b XOR(^)
 *				false|false
 *				-----------
 *		 a    = 0      0b
 *	and  b    = carry << 1 (moved 1 bit left)
 *		 b    = 10b
 *	With decimal numbers we often say: nn + nn -> zero and carry one. That's the idea.
 *	Next iter:
 *	carry =     0      0b
 *				1      0b AND (&)
 *				false|false
 *				---------
 *		carry =	0      0b
 *
 *	Then a      0      0b
 *		 b      1      0b XOR(^)
 *				true|false
 *				-----------
 *		 a    = 1      0b
 *	1 + 1 = 2 = 10b.
 *
 * @note: this function was implemented to add natural numbers (0 -> N)
 * @param a natural number.
 * @param b natural number.
 * @returns a natural that results of the sum of a + b.
 */
unsigned int	adder(unsigned int a, unsigned int b) {
	while (b != 0)
	{
		unsigned int carry = a & b;
		a = a ^ b;
		b = carry << 1;
	}
	return (a);
}
