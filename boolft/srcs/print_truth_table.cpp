/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_truth_table.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:53:33 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/06 17:41:36 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "boolft.hpp"

/**
 * @brief Prints the header of a truth table for logical variables.
 *
 * This function prints a formatted header row for a truth table.
 * Each variable from the map is displayed as a column, followed by a column for the result.
 * A separator line is printed below the header for clarity.
 *
 * @param vars A map associating each variable (as a character) to its current boolean value.
 */
void	print_header(std::map<char, bool>& vars) {
	std::string sep;
	for (auto it = vars.begin(); it != vars.end(); it++) {
		std::cout << "|  " << it->first << "  ";
		sep += "| --- ";
	}
	std::cout << "|  =  |" << std::endl;
	sep += "| --- |";
	std::cout << sep << std::endl;
}

/**
 * @brief Prints a single row of the truth table for a given variable assignment.
 *
 * For each possible assignment of boolean values to variables (as per the map),
 * this function prints the current assignment and the result of evaluating the logical
 * function. On the first call, it also prints the table header by invoking print_header.
 *
 * @note this function calls eval_formula to get its results, meaning that if a formula has
 * 		 an error or is not solvable, a BoolFTException will be raised. The evaluation is the
 * 		 first step of the function, because if is any error with the formula the exception should
 * 		 be raised before print the header or any other result.
 *
 * @param vars A map associating each variable (as a character) to its current boolean value.
 * @param fnc A reference to the string representation of the logical function in Reverse Polish Notation.
 * @param case_idx The current case index (used to determine if this is the first row, for header printing).
 * @throws BoolFTException propagated from eval_formula(char *formula).
 */
void	print_line(std::map<char, bool> vars, std::string& fnc, int case_idx) {
	bool rst = eval_formula((char *)fnc.c_str());
	if (case_idx == 0)
		print_header(vars);
	for (auto it = vars.begin(); it != vars.end(); it++) {
		std::cout << "|  " << BOOL_TO_CHAR(it->second) << "  ";
	}
	std::cout << "|  " << BOOL_TO_CHAR(rst) << "  |" << std::endl;
}

/**
 * @brief Updates the variable assignments to the next case in the truth table.
 *
 * This function is responsible for generating each unique combination of boolean
 * values for the variables in the map, effectively cycling through all possible cases
 * in the truth table. It uses the current case index to determine the binary
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
void	update_case(std::map<char, bool>& vars, int *idx, std::string& fnc) {
	idx[CASE_IDX] = idx[CASE_IDX] + 1;
	int i = 0;
	for (auto it = vars.rbegin();  it != vars.rend(); it++) {
		it->second = ((idx[CASE_IDX] >> i) & 1);
		std::replace(fnc.begin(), fnc.end(), it->first, BOOL_TO_CHAR(it->second));
		i++;
	}
}

/**
 * @brief Prints the complete truth table for a logical function.
 *
 * This function orchestrates the printing of the entire truth table for a given logical
 * function in Reverse Polish Notation. It does so by iterating through all possible
 * combinations of variable assignments, updating the variables and the function string
 * for each case, and printing the corresponding row in the table.
 *
 * The process is as follows:
 * - It initializes an array to keep track of the current case index and the total
 * 	 number of cases (which is 2^N, where N is the number of variables).
 * - It enters a loop that continues until all cases have been processed.
 * - For each case, it updates the variable assignments using update_case,
 * 	 and then prints the current row using print_line.
 *
 * This function is central to the truth table generation, ensuring that every
 * possible input combination is evaluated and displayed.
 *
 * @param vars A reference to the map of variables and their current assignments.
 * @param function The logical function in Reverse Polish Notation, as a C-style string.
 */
void print_table(std::map<char, bool>& vars, char* function) {
	int	cases_info[2] = {-1, (1 << vars.size()) - 1};
		while (cases_info[CASE_IDX] < cases_info[TOTAL_CASES]) {
		std::string fnc(function);
		update_case(vars, &cases_info[0], fnc);
		print_line(vars, fnc, cases_info[CASE_IDX]);
	}
}

/**
 * @brief Prints the truth table for a given logical function in Reverse Polish Notation.
 *
 * This function is the main entry point for truth table generation.
 * It first identifies all unique variables present in the function (uppercase letters),
 * initializes a map to store their boolean values, and then delegates the printing
 * of the truth table to print_table.
 *
 * The steps performed are:
 * - It parses the input function string to collect all unique variable names.
 * - It initializes a map with these variables, setting their initial values to false.
 * - If no variables are found, it throws an exception to indicate an error.
 * - Otherwise, it calls print_table to generate and print the complete truth table for the function.
 *
 * This function ensures that the logical function is valid (contains variables) and coordinates
 * the overall process of truth table generation.
 *
 * @param function The logical function in Reverse Polish Notation, as a C-style string.
 * @throws BoolFtException If no variables are found in the function.
 */
void	print_truth_table(char *function) {
	std::map<char, bool> vars;

	for (int i = 0; function[i]; i++) {
		if (function[i] >= 'A' && function[i] <= 'Z') {
			vars[function[i]] = false;
		}
	}
	if (vars.empty())
		throw BoolFtException("No vars were found.");
	print_table(vars, function);
}
