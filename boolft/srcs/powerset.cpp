/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   powerset.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 09:07:37 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/08 11:52:08 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "boolft.hpp"

/**
 * @brief Recursively calculates all subsets of a given set (the powerset).
 *
 * This function generates all possible subsets (combinations) of the input set by recursively
 * considering each element: either including or excluding it in the current subset.
 *
 * @param[in,out] rst   Reference to the result vector, where each subset will be stored.
 * @param[in]     arr   The set of elements from which to generate subsets.
 * @param[in,out] combo The current subset being built during recursion.
 *
 * @note This function modifies the 'combo' and 'rst' parameters during recursive calls.
 *
 * @details For each element in 'arr', the function makes two recursive calls:
 *          1. Excluding the current element (leaving 'combo' unchanged)
 *          2. Including the current element (adding it to 'combo')
 *          When 'arr' is empty, 'combo' represents a complete subset and is added to 'rst'.
 */
void	calculate_powerset(t_pwr_set& rst, t_set& arr, t_set& combo) {
	if (arr.empty()) {
		rst.push_back(combo);
	} else {
		int current = arr[0];
		t_set rest(arr.begin() + 1, arr.end());
		calculate_powerset(rst, rest, combo);
		combo.push_back(current);
		calculate_powerset(rst, rest, combo);
		combo.pop_back();
	}
}

/**
 * @brief Computes the powerset of a given set.
 *
 * The powerset of a set is the set of all possible subsets, including the empty set.
 * This function initializes the process and returns the complete powerset as a collection of subsets.
 *
 * @param[in] set The input set for which to compute the powerset.
 * @return The powerset of 'set', represented as a vector of subsets.
 *
 * @note If the input set is empty, the result will contain only the empty subset.
 */
t_pwr_set powerset(t_set set) {
	t_pwr_set rst;
	t_set combo;
	if (!set.empty())
		calculate_powerset(rst, set, combo);
	return rst;
}
