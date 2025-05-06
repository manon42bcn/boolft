/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sat.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 21:53:26 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/06 22:50:33 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "boolft.hpp"

/**
 * @brief Updates the variable assignments to the next case to check if a given
 * 		  function is satisfiable.
 *
 * This function is responsible for generating each unique combination of boolean
 * values for the variables in the map, effectively cycling through all possible cases.
 * It uses the current case index to determine the binary
 * representation of the variable assignments, updating both the map and the function
 * string to reflect these new values.
 *
 * The function works as follows:
 * - It increments the current case index.
 * - For each variable in the map (iterated in reverse order), it sets the variable's
 * boolean value based on the corresponding bit in the case index.
 * - It replaces all occurrences of the variable in the function string with its current
 * boolean value (as a character).
 *
 * This approach allows the function to systematically explore all possible variable assignments,
 * which is essential for building a complete truth table.
 *
 * @param vars A reference to the map of variables and their current boolean assignments.
 * @param idx A pointer to an array storing the current case index (to be incremented)
 * 			  and the total number of cases.
 * @param fnc A reference to the string representation of the logical function,
 * 			  which is updated to reflect the new variable assignments.
 */
inline void	update_case(std::map<char, bool>& vars, int *idx, std::string& fnc) {
	idx[CASE_IDX] = idx[CASE_IDX] + 1;
	int i = 0;
	for (auto it = vars.rbegin();  it != vars.rend(); it++) {
		it->second = ((idx[CASE_IDX] >> i) & 1);
		std::replace(fnc.begin(), fnc.end(), it->first, BOOL_TO_CHAR(it->second));
		i++;
	}
}

/**
 * @brief Evaluates a logical function for all possible variable assignments to determine if the
 * function ever evaluates to true.
 *
 * This function systematically explores every possible combination of boolean assignments for
 * the variables in the provided map. For each combination, it updates the variables,
 * substitutes their values into the function string, and evaluates the resulting formula
 * using Reverse Polish Notation (RPN).
 *
 * The process is as follows:
 * - It initializes an array to track the current case index and the total number of cases
 * (which is 2^N, where N is the number of variables).
 * - It enters a loop that iterates over all possible variable assignments:
 *   - For each case, it updates the variable assignments and substitutes their values into the function string.
 *   - It evaluates the resulting formula using eval_formula.
 * - If at any point the formula evaluates to true, the function immediately returns true.
 * - If all cases are exhausted without the formula ever evaluating to true, it returns false.
 *
 * @param vars A reference to the map of variables and their current boolean assignments.
 * @param function The logical function in Reverse Polish Notation, as a C-style string.
 * @return True if the function evaluates to true for at least one variable assignment, false otherwise.
 * @throws BoolFtException propagated from eval_formula
 */
inline bool evaluate(std::map<char, bool>& vars, char* function) {
	int	cases_info[2] = {-1, (1 << vars.size()) - 1};
	while (cases_info[CASE_IDX] < cases_info[TOTAL_CASES]) {
		std::string fnc(function);
		update_case(vars, &cases_info[0], fnc);
		if (eval_formula((char *)fnc.c_str()))
			return true;
	}
	return false;
}

/**
 * @brief Determines if a given logical function is satisfiable.
 *
 * This function checks whether the provided logical function (in Reverse Polish Notation)
 * is satisfiable, i.e., whether there exists at least one assignment of boolean
 * values to its variables that makes the function evaluate to true.
 *
 * The function proceeds as follows:
 * - It scans the input string to identify all unique variables (uppercase letters).
 * - It initializes a map to store these variables and their initial boolean values.
 * - If no variables are found, it throws an exception, as a function without variables
 * 	 cannot be meaningfully evaluated for satisfiability.
 * - It then calls the evaluate function, which systematically checks all possible
 * 	 combinations of variable assignments to determine if the function can be true for any of them.
 *
 * @param function The logical function in Reverse Polish Notation, as a C-style string.
 * @return True if the function is satisfiable (i.e., evaluates to true for at least one variable assignment),
 * 		   false otherwise.
 * @throws BoolFtException If no variables are found in the function.
 */
bool	sat(char *function) {
	std::map<char, bool> vars;

	for (int i = 0; function[i]; i++) {
		if (function[i] >= 'A' && function[i] <= 'Z') {
			vars[function[i]] = false;
		}
	}
	if (vars.empty())
		throw BoolFtException("No vars were found.");
	return (evaluate(vars, function));
}
