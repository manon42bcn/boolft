/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_formula.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:53:33 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/06 17:48:14 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "boolft.hpp"

/**
 * @brief Evaluates a single operator in Reverse Polish Notation (RPN) on a stack
 * of boolean values.
 *
 * This function processes a single logical operator in RPN, updating the stack
 * accordingly. It handles both unary ('!') and binary ('&', '|', '^', '>', '=') operators.
 *
 * For the '!' operator (logical NOT):
 * - It pops the top value from the stack, applies the NOT operation, and pushes
 * 	 the result back.
 * - Throws an exception if there are not enough operands.
 *
 * For binary operators:
 * - It pops the top two values from the stack, applies the specified logical operation,
 * and pushes the result back.
 * - The supported operations are:
 *   - '!' (NOT), '&' (AND), '|' (OR), '^' (XOR), '>' (IMPLICATION), '=' (EQUIVALENCE).
 * - Throws an exception if there are not enough operands.
 *
 * @param func Reference to the stack of boolean values used for RPN evaluation.
 * @param opr The operator to be applied.
 * @throws BoolFtException If there are not enough operands on the stack for the given operator.
 */
void	solve_rpn(std::stack<bool>& func, char opr) {
	if (opr == '!') {
		if (func.size() < 1)
			throw BoolFtException("Not solvable. Too few operands to resolve.");
		bool	a = func.top(); func.pop();
		func.push(!a);
		return ;
	} else {
		if (func.size() < 2)
			throw BoolFtException("Not solvable. Too few operands to resolve.");
		bool	a = func.top(); func.pop();
		bool	b = func.top(); func.pop();
		switch (opr) {
			case '&':
				func.push(a && b);
				break;
			case '|':
				func.push(a || b);
				break;
			case '^':
				func.push(a != b);
				break;
			case '>':
				func.push(!(a && !b));
				break;
			case '=':
				func.push(a == b);
				break;
		}
	}
}

/**
 * @brief Checks if a character is a supported logical operator.
 *
 * This helper function determines if a given character is one of the supported
 * logical operators used in Reverse Polish Notation evaluation.
 * The recognized operators are:
 * '!' (NOT), '&' (AND), '|' (OR), '^' (XOR), '>' (IMPLICATION), '=' (EQUIVALENCE).
 *
 * @param c The character to check.
 * @return True if the character is a recognized operator, false otherwise.
 */
bool is_operator (char c) {
	return (c == '!' || c == '&' || c == '|' || c == '=' || c == '>' || c == '^');
}

/**
 * @brief Evaluates a logical formula in Reverse Polish Notation (RPN).
 *
 * This function evaluates a logical formula provided as a C-style string in RPN.
 * It processes each character in the string, pushing operands ('0' or '1')
 * onto a stack and applying operators as they are encountered.
 *
 * The evaluation proceeds as follows:
 * - For each character in the formula:
 *   - If it is '0' or '1', the corresponding boolean value is pushed onto the stack.
 *   - If it is a recognized operator, solve_rpn is called to process the operator and update the stack.
 *   - If an unrecognized symbol is encountered, an exception is thrown.
 * - After processing all characters, the stack must contain exactly one value,
 * 	 which is the result of the formula.
 * - Throws an exception if the formula is not well-formed (e.g., too many operands, unrecognized symbol).
 *
 * @param formula The logical formula in Reverse Polish Notation, as a C-style string.
 * @return The boolean result of evaluating the formula.
 * @throws BoolFtException If the formula is not well-formed, contains unrecognized symbols or if more than
 * 		   one element at stack at the end of the execution.
 */
bool eval_formula(char* formula) {
	std::stack<bool> func;
	for (size_t i = 0; formula[i]; i++) {
		if (formula[i] == '1' || formula[i] == '0') {
			func.push(true ? formula[i] == '1': false);
		} else {
			if (!is_operator(formula[i])) {
				throw BoolFtException("Unrecognized Symbol");
			}
			solve_rpn(func, formula[i]);
		}
	}
	if (func.size() != 1) {
		throw BoolFtException("Not Solvable. Too many operands.");
	}
	bool	rst = func.top(); func.pop();
	return rst;
}
