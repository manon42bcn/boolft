/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_set.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:03:23 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/08 23:30:22 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "boolft.hpp"

/**
 * @brief Computes the complement of a set with respect to a given universe.
 *
 * Given a set `a` and a universe set `universe`, this function returns a new
 * set that contains all elements from the universe that are not present in `a`.
 * This operation is commonly known as the "complement" of `a` with respect to `universe`.
 *
 * @note logical equivalence: A! = A' = Universe - A
 * @param a The input set whose complement is to be computed.
 * @param universe The set defining the universe of all possible elements.
 * @return t_set The result set, containing all elements from the universe not present in `a`.
 */
t_set	complement_op(t_set& a, std::set<int>& universe) {
	t_set complement_rst;
	for (int e : universe) {
		if (std::find(a.begin(), a.end(), e) == a.end())
			complement_rst.push_back(e);
	}
	return complement_rst;
}

/**
 * @brief Computes the union of two sets.
 *
 * This function takes two sets, `a` and `b`, and returns a new set containing all
 * unique elements from both sets.
 * The resulting set will have no duplicates, as is typical for set operations.
 *
 * @note logical equivalence: AB& = A ∪ B
 * @param a The first set to include in the union.
 * @param b The second set to include in the union.
 * @return t_set A new set representing the union of `a` and `b`.
 *
 */
t_set	union_op(t_set& a, t_set& b) {
	std::set<int> rst(a.begin(), a.end());
	rst.insert(b.begin(), b.end());
	t_set union_rst(rst.begin(), rst.end());
	return union_rst;
}

/**
 * @brief Computes the intersection of two sets.
 *
 * This function takes two sets, `a` and `b`, and returns a new set containing only
 * the elements that are present in both sets.
 * The resulting set will contain no duplicates, as is typical for set operations.
 *
 * @note logical equivalence: AB| = A ∩ B
 * @param a The first set to compare for common elements.
 * @param b The second set to compare for common elements.
 * @return t_set A new set representing the intersection of `a` and `b`.
 */
t_set 	intersec_op(t_set& a, t_set& b) {
	std::set<int> rst;
	for (int a_v : a) {
		if (std::find(b.begin(), b.end(), a_v) != b.end()) {
			rst.insert(a_v);
		}
	}
	t_set intersection_rst(rst.begin(), rst.end());
	return intersection_rst;
}

/**
 * @brief Computes the symmetric difference (XOR) of two sets.
 *
 * This function calculates the symmetric difference between sets `a` and `b`,
 * which consists of elements that are in either `a` or `b`, but not in both.
 * The operation is equivalent to the union minus the intersection of the two sets.
 *
 * @note logical equivalence: AB^ = (A u B) - (A ∩ B)
 * @param a The first set to include in the operation.
 * @param b The second set to include in the operation.
 * @return t_set A new set representing the symmetric difference of `a` and `b`.
 */
t_set 	xor_op(t_set& a, t_set& b) {
	t_set a_u_b = union_op(a, b);
	t_set a_i_b = intersec_op(a, b);
 	std::set<int> rst;
	std::set_difference(a_u_b.begin(), a_u_b.end(),
						a_i_b.begin(), a_i_b.end(),
						std::inserter(rst, rst.end()));
	t_set xor_rst(rst.begin(), rst.end());
	return xor_rst;
}

/**
 * @brief Computes the logical implication between two sets with respect to a universe.
 *
 * This function calculates the logical implication of set `a` to set `b` within
 * the context of a given `universe`.
 * The operation is equivalent to "if a then b", which in set theory terms
 * translates to the union of the complement of `a` (within the `universe`) and the set `b`.
 *
 * @note logical equivalence AB> (A' u B)
 * @param a The antecedent set in the implication.
 * @param b The consequent set in the implication.
 * @param universe The universal set, used to compute the complement of `a`.
 * @return t_set A new set representing the result of the implication operation.
 *
 */
t_set	implication_op(t_set& a, t_set& b, std::set<int>& universe) {
	t_set ap = complement_op(a, universe);
	return union_op(ap, b);
}

/**
 * @brief Computes the logical equivalence between two sets with respect to a universe.
 *
 * This function calculates the logical equivalence (bi-implication) of set `a`
 * and set `b` within the context of a given `universe`.
 * The equivalence operation is defined as the implication between the intersection
 * of `a` and `b` and the intersection of their complements.
 * In set theory, this corresponds to the idea that both sets contain the same elements
 * within the universe, or are "equal" in terms of their presence.
 *
 * @note Logical equivalence AB= = (A ∩ B) U (A' ∩ B')
 * @param a The first set to compare for equivalence.
 * @param b The second set to compare for equivalence.
 * @param universe The universal set, used to compute the complements of `a` and `b`.
 * @return t_set A new set representing the result of the equivalence operation.
 *
 * @note All input sets must be compatible with standard C++ iterators (e.g., `std::set`, `std::vector`).
 * The function internally uses `complement_op`, `intersec_op`, and `implication_op` to
 * perform the required operations.
 */
t_set	equivalence_op(t_set& a, t_set& b, std::set<int>& universe) {
	t_set a_p = complement_op(a, universe);
	t_set b_p = complement_op(b, universe);
	t_set u_a_b = intersec_op(a, b);
	t_set u_ap_bp = intersec_op(a_p, b_p);
	return implication_op(u_a_b, u_ap_bp, universe);
}

/**
 * @brief Performs a set operation on the top elements of a stack, using a given operator.
 *
 * This function takes a universal set, a stack of sets, and an operator character,
 * and applies the corresponding set operation to the top elements of the stack.
 * The result is pushed back onto the stack.
 * Supported operators are:
 *   - '!' (complement): Computes the complement of the top set with respect to the universe.
 *   - '&' (intersection): Computes the intersection of the top two sets.
 *   - '|' (union): Computes the union of the top two sets.
 *   - '^' (symmetric difference/XOR): Computes the symmetric difference of the top two sets.
 *   - '>' (implication): Computes the implication from the second-top to the top set, using the universe.
 *   - '=' (equivalence): Computes the equivalence between the top two sets, using the universe.
 *
 * @param universe The universal set, used for complement, implication, and equivalence operations.
 * @param stack The stack of sets on which to perform the operation. Modified in place.
 * @param op The operator character specifying the operation to perform.
 * @throws BoolFtException If the stack does not contain enough operands for the requested operation.
 *
 * @note All set operations are performed using the corresponding helper functions.
 * The function checks the stack size before each operation and throws an exception if
 * there are too few operands.
 */
void	operate (std::set<int>& universe, std::stack<t_set>& stack, char op) {
	if (stack.empty())
		throw BoolFtException("Too few operands at stack.");
	if (op == '!') {
		t_set a = stack.top(); stack.pop();
		stack.push(complement_op(a, universe));
		return;
	}
	if (stack.size() < 2)
		throw BoolFtException("Too few operands at stack.");
	t_set a = stack.top(); stack.pop();
	t_set b = stack.top(); stack.pop();
	switch (op) {
		case '&':
			stack.push(intersec_op(a, b));
			break;
		case '|':
			stack.push(union_op(a, b));
			break;
		case '^':
			stack.push(xor_op(a, b));
			break;
		case '>':
			stack.push(implication_op(b, a, universe));
			break;
		case '=':
			stack.push(equivalence_op(a, b, universe));
			break;
	}
}

/**
 * @brief Evaluates a set formula using a stack-based approach.
 *
 * This function processes a formula string and evaluates it using sets corresponding to variable
 * names and a set of operators.
 * The formula is parsed character by character. Variables (A-Z) are pushed onto the stack
 * as their corresponding sets from `vars`.
 * Operators (such as '!', '&', '|', '^', '>', '=') trigger set operations on the stack using the `operate` function.
 * The final result(s) remain on the stack after processing the entire formula.
 *
 * @param universe The universal set used for operations involving complements or implications.
 * @param vars A map from variable characters (A-Z) to their corresponding sets.
 * @param formula A null-terminated C-style string representing the formula to evaluate.
 * @return std::stack<t_set> A stack containing the result(s) of the evaluated formula.
 *
 * @note The function assumes that the formula is syntactically correct and that all variables and operators are valid.
 * The stack may contain more than one result if the formula is ambiguous or incomplete.
 */
std::stack<t_set>	fnc_set (std::set<int> universe, std::map<char, t_set>& vars, char* formula) {
	std::stack<t_set> stack;
	for (int i = 0; formula[i]; i++) {
		if (formula[i] >= 'A' && formula[i] <= 'Z') {
			stack.push(vars[formula[i]]);
		} else {
			operate(universe, stack, formula[i]);
		}
	}
	return stack;
}

/**
 * @brief Evaluates a set formula and returns the result as a single set.
 *
 * This function processes a formula string, assigning sets from the input vector to
 * variables (A-Z) as they appear in the formula.
 * It also builds the universe set as the union of all elements found in the provided sets.
 * The formula is evaluated using the `fnc_set` function, which applies set operations
 * according to the operators in the formula.
 * The function checks for errors such as mismatches between variables and input sets,
 * unrecognized symbols, and incorrect number of results on the stack.
 *
 * @param formula A null-terminated C-style string representing the formula to evaluate.
 * @param sets A vector of sets to assign to variables in the formula, in the order of
 * 			   their first appearance.
 * @return t_set The result of evaluating the formula as a single set.
 * @throws BoolFtException If there is a mismatch between variables and sets,
 * 		  if an unrecognized symbol is found, if no sets are assigned, or if the stack does
 * 		  not contain exactly one result after evaluation.
 *
 * @note The function assumes that variables are uppercase letters (A-Z) and that the formula uses valid operators.
 * The universe set is constructed as the union of all elements in the provided sets.
 */
t_set eval_set(char* formula, std::vector<t_set> sets) {
	std::map<char, t_set > vars;
	std::set<int> universe;
	size_t size_sets = sets.size();
	size_t set_to_insert = 0;
	for (int i = 0; formula[i]; i++) {
		if (formula[i] >= 'A' && formula[i] <= 'Z') {
			if (vars.find(formula[i]) == vars.end()) {
				if (set_to_insert >= size_sets)
					throw BoolFtException("Sets and Vars in function does not match.");
				vars[formula[i]] = sets[set_to_insert];
				for (int v : sets[set_to_insert])
					universe.insert(v);
				set_to_insert++;
			}
		} else if (!is_operator(formula[i]))
			throw BoolFtException("Unrecognized symbol.");
	}
	if (vars.empty())
		throw BoolFtException("No sets were found.");

	std::stack<t_set> rst = fnc_set(universe, vars, formula);
	if (rst.size() != 1)
		throw BoolFtException("Too many arguments after operate.");

	return rst.top();
}
