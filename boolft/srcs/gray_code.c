/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gray_code.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 17:23:14 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/02 18:52:53 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "boolft.h"

/**
 * @brief Converts a binary number to its Gray code equivalent.
 *
 * The Gray code is a binary numeral system where two successive values
 * differ in only one bit. This function computes the Gray code of a
 * given integer using the formula:
 *
 *     Gray(n) = n XOR (n >> 1)
 *
 * This method is efficient and commonly used in error correction,
 * digital encoders, and hardware design to avoid transition glitches.
 *
 * @param n The non-negative integer to convert to Gray code.
 * @return The corresponding Gray code of the input integer.
 *
 * @note Gray code for negative int is a generalization of the basic formula.
 *		 Gray code was created for non-negative integers.
 *
 * @see https://en.wikipedia.org/wiki/Gray_code
 */
int gray_code(int n) {
	return ((int)(n ^ (n >> 1)));
}
