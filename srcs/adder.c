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

/*
 * @brief Bitwise adder function.
 * @note how it works?
 *	Basically works as any other add method, but over a 2 base, instead of
 *	the "normal" decimal one.
 *
 *
 *
 * @param x natural number
 * @param y natural number
 * @returns a natural that results of the sum of x + y.
 */
int	adder(int x, int y)
{
	int	carry;

	while (y != 0)
	{
		carry = x & y;
		x = x ^ y;
		y = carry << 1;
	}
	return (x);
}
