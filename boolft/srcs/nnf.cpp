/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nnf.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 18:01:11 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/07 16:14:54 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "boolft.hpp"

/**
 * @brief Recursively finds the start index of the subexpression corresponding
 * to a given operator in an RPN string.
 *
 * This helper function is used to locate the beginning of the subexpression
 * that is processed by a particular operator in a Reverse Polish Notation (RPN) string.
 * It is especially useful for parsing or reconstructing logical expressions from RPN.
 *
 * The function works as follows:
 * - If the character at the given position is not an operator, it returns the
 * 	 current position (indicating a simple operand).
 * - If the operator is '!' (NOT), it recursively finds the start of its single operand.
 * - If the operator is '&' (AND) or '|' (OR), it recursively finds the start of both
 * 	 operands (first the right, then the left).
 * - If an unexpected operator is encountered, it throws an exception.
 *
 * @param expr The RPN expression string to search.
 * @param end The index of the operator whose subexpression's start is to be found.
 * @return The index of the start of the subexpression corresponding to the operator
 * 		   at position 'end'.
 * @throws BoolFtException If an unexpected operator is encountered.
 */
static int find_start(const std::string& expr, int end) {
	char c = expr[end];
	if (!is_operator(c))
		return end;
	if (c == '!')
		return find_start(expr, end - 1);
	if (c == '&' || c == '|') {
		int start2 = find_start(expr, end - 1);
		return find_start(expr, start2 - 1);
	}
	throw BoolFtException("Unexpected Token");
}

/**
 * @brief Splits an RPN expression into two subexpressions corresponding to the operands
 * of its root binary operator.
 *
 * This function is designed to decompose a logical expression in Reverse Polish
 * Notation (RPN) into its left and right subexpressions. The root of the expression is
 * assumed to be a binary operator ('&' for AND, '|' for OR) at the end of the string.
 *
 * The function proceeds as follows:
 * - It checks that the expression is long enough to be split (at least 3 characters).
 * - It verifies that the last character is a binary operator ('&' or '|').
 * - It uses the helper function `find_start` to locate the boundary between the two operands.
 * - It splits the original expression into two parts: the left operand (from the start
 * to the boundary) and the right operand (from the boundary to just before the root operator).
 * - It returns these two subexpressions as a pair of strings.
 *
 * @param expr The RPN expression string to be split.
 * @return A pair of strings, where the first is the left subexpression and the second
 * 		   is the right subexpression.
 * @throws BoolFtException If the expression is too short or the root is not a binary operator.
 */
static std::pair<std::string,std::string> split_rpn(const std::string& expr) {
	int n = expr.size();
	if (n < 3)
		throw BoolFtException("Expression too short to split.");
	char root = expr[n-1];
	if (root != '&' && root != '|')
		throw BoolFtException("Not binary operator.");
	int second_start = find_start(expr, n - 2);
	std::string e1 = expr.substr(0, second_start);
	std::string e2 = expr.substr(second_start, (n-1) - second_start);
	return { e1, e2 };
}

/**
 * @brief Applies logical negation to an RPN expression, converting it to its negation
 * in Negation Normal Form (NNF).
 *
 * This function recursively transforms a logical expression in Reverse Polish Notation
 * (RPN) into its negation, following the rules of Negation Normal Form (NNF). In NNF,
 * negations only appear immediately before variables, and De Morgan's laws are used to "push"
 * negations inward.
 *
 * The function handles the following cases:
 * - If the expression is a single variable, it appends a '!' (NOT) operator (negation applied
 *   directly to the variable).
 * - If the root operator is '!' (NOT), it removes the negation (double negation is eliminated).
 * - If the root operator is '&' (AND) or '|' (OR), it splits the expression into its
 * 	 left and right subexpressions, recursively negates each, and combines them with the
 * 	 dual operator, following De Morgan's laws.
 *   - De Morgan's law for AND: \( \neg (A \land B) \equiv \neg A \lor \neg B \)
 *   - De Morgan's law for OR: \( \neg (A \lor B) \equiv \neg A \land \neg B \)
 *   - In RPN, this means replacing '&' with '|' (or vice versa) and negating the operands.
 * - Throws an exception if an unexpected operator or malformed expression is encountered.
 *
 * @param expr The RPN expression to be negated.
 * @return The negated expression in RPN, in Negation Normal Form.
 * @throws BoolFtException If an unexpected operator or malformed expression is encountered.
 * @see De Morgan's laws: https://en.wikipedia.org/wiki/De_Morgan%27s_laws
 * @see Negation Normal Form: https://en.wikipedia.org/wiki/Negation_normal_form
 */
static std::string nnf_apply_neg(const std::string& expr) {
	int n = expr.size();
	if (n == 1 && !is_operator(expr[0]))
		return expr + "!";
	char root = expr[n - 1];
	if (root == '!')
		return expr.substr(0, n-1);
	if (root=='&' || root=='|') {
		auto [e1, e2] = split_rpn(expr);
		char dual = (root == '&' ? '|' : '&');
		std::string ne1 = nnf_apply_neg(e1);
		std::string ne2 = nnf_apply_neg(e2);
		return ne1 + ne2 + dual;
	}
	throw BoolFtException("Apply Negation: Unexpected token.");
}

/**
 * @brief Applies a logical operator to the top elements of an RPN expression stack,
 * pushing the result back onto the stack.
 *
 * This function processes both unary and binary logical operators on a stack of RPN
 * subexpressions. It is designed to work with expressions in Negation Normal Form (NNF),
 * and it handles the following operators:
 * - '!' (NOT, unary): Applies negation to the top element using `nnf_apply_neg`.
 * - '&' (AND, binary): Combines the top two elements with an AND operator.
 * - '|' (OR, binary): Combines the top two elements with an OR operator.
 * - '>' (IMPLICATION): Translates "A implies B" to "¬A ∨ B" in NNF.
 * - '=' (EQUIVALENCE): Translates "A ≡ B" to "(A ∧ B) ∨ (¬A ∧ ¬B)" in NNF.
 * - '^' (XOR): Translates "A XOR B" to "(A ∧ ¬B) ∨ (¬A ∧ B)" in NNF.
 *
 * For binary operators, the function pops the top two elements, constructs the appropriate
 * NNF expression, and pushes the result back onto the stack. For the unary operator '!',
 * it pops the top element, applies negation, and pushes the result.
 *
 * The function throws exceptions if there are not enough elements on the stack for the
 * given operator, or if an unknown operator is encountered.
 *
 * @param fnc Reference to the stack of RPN subexpressions.
 * @param op The operator to be applied ('!', '&', '|', '>', '=', '^').
 * @throws BoolFtException If there are not enough elements on the stack or if an unknown
 * 		   operator is encountered.
 *
 * @note
 * - For implication ('>'), the function constructs "¬A ∨ B" in NNF.
 * - For equivalence ('='), the function constructs "(A ∧ B) ∨ (¬A ∧ ¬B)" in NNF.
 * - For XOR ('^'), the function constructs "(A ∧ ¬B) ∨ (¬A ∧ B)" in NNF.
 * - All negations are applied using `nnf_apply_neg`, ensuring the result is in NNF.
 */
void operate(std::vector<std::string>& fnc, char op) {
	using std::string;
	if (op == '!') {
		if (fnc.empty())
			throw BoolFtException("Not enough elements at stack: Unitary operator.");
		string s = fnc.back(); fnc.pop_back();
		fnc.push_back( nnf_apply_neg(s) );
		return;
	}
	if (fnc.size() < 2)
		throw BoolFtException("Not enough elements at stack: Binary operator.");

	string b = fnc.back(); fnc.pop_back();
	string a = fnc.back(); fnc.pop_back();
	string res;
	switch(op) {
		case '&':
		case '|':
			res = a + b + op;
			break;
		case '>': {
			// A → B  ≡  ¬A ∨ B
			string na = nnf_apply_neg(a);
			res = na + b + "|";
			break;
		}
		case '=': {
			// A ≡ B  ≡  (A ∧ B) ∨ (¬A ∧ ¬B)
			string na = nnf_apply_neg(a);
			string nb = nnf_apply_neg(b);
			// construye: A B &   na nb &   |
			res = a + b + "&"
				  + na + nb + "&"
				  + "|";
			break;
		}
		case '^': {
			// A ⊕ B  ≡  (A ∧ ¬B) ∨ (¬A ∧ B)
			string nb = nnf_apply_neg(b);
			string na = nnf_apply_neg(a);
			// sub1 = A nb &, sub2 = na B &
			res = a + nb + "&"
				  + na + b  + "&"
				  + "|";
			break;
		}
		default:
			throw BoolFtException("Operate: Unknown operator.");
	}
	fnc.push_back(res);
}

/**
 * @brief Converts a logical formula in Reverse Polish Notation (RPN) to its
 * Negation Normal Form (NNF).
 *
 * This function processes a logical formula given as a C-style string in RPN.
 * It parses the formula character by character, pushing operands onto a stack and
 * applying operators using the `operate` helper function. The result is a single
 * expression in RPN, where all negations are applied directly to variables, and all
 * other logical operators are in NNF.
 *
 * The function proceeds as follows:
 * - For each character in the input string:
 *   - If the character is an uppercase letter (A-Z), it is treated as an operand and
 *     pushed onto the stack.
 *   - If the character is a recognized operator ('!', '&', '|', '>', '=', '^'),
 *     it is applied to the top elements of the stack using `operate`.
 *   - If the character is not recognized, an exception is thrown.
 * - After processing all characters, the stack must contain exactly one element, which
 * 	 is the final NNF expression in RPN.
 * - If the stack is empty or contains more than one element after processing,
 * 	 an exception is thrown.
 *
 * @param formula The logical formula in RPN, as a C-style string.
 * @return The formula converted to NNF, as a string in RPN.
 * @throws BoolFtException If an unrecognized symbol is encountered, or if the stack does
 * 		   not contain exactly one element after processing.
 *
 * @note
 * - The input formula must be in Reverse Polish Notation (RPN).
 * - The output is also in RPN, with all negations applied directly to variables.
 * - All other logical operators are translated to NNF, following De Morgan's laws and
 * 	 standard logical equivalences.
 */
std::string negation_normal_form(char *formula) {
	std::vector<std::string> fnc;
	for (size_t i = 0; formula[i]; i++) {
		if (formula[i] >= 'A' && formula[i] <= 'Z') {
			std::string operands;
			operands.assign(formula + i, 1);
			fnc.push_back(operands);
		} else {
			if (!is_operator(formula[i]))
				throw BoolFtException("Unrecognized symbol.");
			operate(fnc, formula[i]);
		}
	}
	if (fnc.size() != 1)
		throw BoolFtException("Unsolvable. Too many operands after conversion.");
	std::string result = fnc.back(); fnc.pop_back();
	return result;
}

