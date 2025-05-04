/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bool_operators.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 17:23:14 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/02 18:36:33 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "boolft.h"

/**
 * @brief Handles logical NOT operation in Reverse Polish Notation (RPN) stack processing.
 *
 * This static function performs a logical negation operation on the top element of the stack:
 * 1. Pops the topmost boolean value
 * 2. Applies boolean negation (! operator)
 * 3. Pushes the result back onto the stack
 *
 * @param tail Double pointer to the stack's topmost element (tail). The function modifies
 *             the stack in-place by removing the current tail and replacing it with the
 *             negated value's node.
 *
 * @return Pointer to the new top of the stack with the negated value, or NULL if:
 *         - The stack was empty (*tail == NULL)
 *         - Memory allocation failed during insert_element()
 *
 * @note
 * - Stack modification: Directly manipulates the tail pointer through double indirection
 * - Operation flow: Original tail element is destroyed and replaced with new value
 * - Error propagation: Returns NULL on empty stack or allocation failure
 *
 * @see pop_element(), insert_element(), t_stack, t_bool
 */
static t_stack* not_operator(t_stack** tail) {
	if (!*tail)
		return (NULL);
	t_bool	operand = pop_element(tail);
	(*tail) = insert_element(!operand, tail);
	return (*tail);
}

/**
 * @brief Applies the logical AND operation to the two topmost elements of the stack.
 *
 * This function pops the two topmost boolean values from the stack,
 * performs a logical AND operation on them, and pushes the result
 * back onto the stack.
 *
 * If the stack contains fewer than two elements, the function returns NULL.
 *
 * @param tail Double pointer to the tail of the stack. The function
 *             will update this pointer if the stack is modified.
 *
 * @return Pointer to the new tail of the stack after the operation,
 *         or NULL if the operation could not be performed due to insufficient elements.
 */
static t_stack* and_operator(t_stack** tail) {
	if (!*tail)
		return (NULL);
	t_bool	b = pop_element(tail);
	if (!*tail)
		return (NULL);
	t_bool	a = pop_element(tail);
	(*tail) = insert_element(a && b, tail);
	return (*tail);
}

/**
 * @brief Applies the logical OR operation to the two topmost elements of the stack.
 *
 * This function pops the two topmost boolean values from the stack,
 * performs a logical OR operation on them, and pushes the result
 * back onto the stack.
 *
 * If the stack contains fewer than two elements, the function returns NULL.
 *
 * @param tail Double pointer to the tail of the stack. The function
 *                     will update this pointer if the stack is modified.
 *
 * @return Pointer to the new tail of the stack after the operation,
 *         or NULL if the operation could not be performed due to insufficient elements.
 *
 */
static t_stack* or_operator(t_stack** tail) {
	if (!*tail)
		return (NULL);
	t_bool	b = pop_element(tail);
	if (!*tail)
		return (NULL);
	t_bool	a = pop_element(tail);
	(*tail) = insert_element(a || b, tail);
	return (*tail);
}

/**
 * @brief Applies the logical XOR (exclusive OR) operation to the two topmost elements of the stack.
 *
 * This function pops the two topmost boolean values from the stack,
 * performs a logical XOR (exclusive OR) operation on them, and pushes
 * the result back onto the stack.
 *
 * If the stack contains fewer than two elements, the function returns NULL.
 *
 * @param tail Double pointer to the tail of the stack. The function
 *                     will update this pointer if the stack is modified.
 *
 * @return Pointer to the new tail of the stack after the operation,
 *         or NULL if the operation could not be performed due to insufficient elements.
 *
 */
static t_stack* xor_operator(t_stack** tail) {
	if (!*tail)
		return (NULL);
	t_bool	b = pop_element(tail);
	if (!*tail)
		return (NULL);
	t_bool	a = pop_element(tail);
	(*tail) = insert_element(a != b, tail);
	return (*tail);
}

/**
 * @brief Applies the logical conditional (implication) operation to the two topmost elements of the stack.
 *
 * This function pops the two topmost boolean values from the stack,
 * interprets them as operands 'a' (second from the top) and 'b' (top),
 * and computes the logical implication (a → b), which is equivalent to
 * (!a || b) or !(a && !b). The result is then pushed back onto the stack.
 *
 * If the stack contains fewer than two elements, the function returns NULL.
 *
 * @param tail Double pointer to the tail of the stack. The function
 *                     will update this pointer if the stack is modified.
 *
 * @return Pointer to the new tail of the stack after the operation,
 *         or NULL if the operation could not be performed due to insufficient elements.
 *
 * @note Logical implication (a → b) is true except when 'a' is true and 'b' is false.
 */
static t_stack* conditional_operator(t_stack** tail) {
	if (!*tail)
		return (NULL);
	t_bool	b = pop_element(tail);
	if (!*tail)
		return (NULL);
	t_bool	a = pop_element(tail);
	(*tail) = insert_element(!(a && !b), tail);
	return (*tail);
}

/**
 * @brief Applies the logical biconditional (equivalence) operation to the two topmost elements of the stack.
 *
 * This function pops the two topmost boolean values from the stack,
 * interprets them as operands 'a' (second from the top) and 'b' (top),
 * and computes the logical biconditional (a ↔ b), also known as logical equivalence.
 * The result is true if both operands are equal (both true or both false),
 * and false otherwise. The result is then pushed back onto the stack.
 *
 * If the stack contains fewer than two elements, the function returns NULL.
 *
 * @param tail Double pointer to the tail of the stack. The function
 *                     will update this pointer if the stack is modified.
 *
 * @return Pointer to the new tail of the stack after the operation,
 *         or NULL if the operation could not be performed due to insufficient elements.
 *
 * @note Logical biconditional (a ↔ b) is true if both operands have the same value.
 */
static t_stack* biconditional_operator(t_stack** tail) {
	if (!*tail)
		return (NULL);
	t_bool	b = pop_element(tail);
	if (!*tail)
		return (NULL);
	t_bool	a = pop_element(tail);
	(*tail) = insert_element(a == b, tail);
	return (*tail);
}

/**
 * @brief Returns the index of a logical operator character in the predefined operator list.
 *
 * This function searches for the given character in a static array of supported
 * logical operator symbols: '!' (NOT), '&' (AND), '|' (OR), '^' (XOR), '>' (IMPLICATION), and '=' (BICONDITIONAL).
 * If the character is found, the function returns its index in the array.
 * If the character is not recognized as a valid operator, it returns UNRECOGNIZED_SYMBOL.
 *
 * @param c The character representing a logical operator to search for.
 *
 * @return The index of the operator in the operators array if found,
 *         or UNRECOGNIZED_SYMBOL if the character is not a supported operator.
 *
 */
int	get_index_operator(char c) {
	static char operators[] = {'!', '&', '|', '^', '>', '=', '\0'};
	for (int i = 0; operators[i]; i++) {
		if (c == operators[i])
			return (i);
	}
	return (UNRECOGNIZED_SYMBOL);
}

/**
 * @brief Applies the logical operator corresponding to the given character on the stack.
 *
 * This function maps a logical operator character (such as '!', '&', '|', '^', '>', or '=')
 * to its corresponding stack operation function. It uses the get_index_operator() function
 * to determine the index of the operator and then calls the appropriate function from
 * the operators array. The selected operator function is applied to the stack, modifying
 * it as necessary.
 *
 * If the provided character does not correspond to a recognized operator,
 * the function returns NULL.
 *
 * @param c The character representing the logical operator to apply.
 * @param tail Double pointer to the tail of the stack. The function
 *                     will update this pointer if the stack is modified.
 *
 * @return Pointer to the new tail of the stack after the operation,
 *         or NULL if the operator is unrecognized or the operation could not be performed.
 *
 * @note The operators array must be kept in sync with the order of the
 *       operators defined in get_index_operator().
 */
t_stack*	operate(char c, t_stack **tail) {
	static	t_operator	(operators)[] = {
		&not_operator, &and_operator,
		&or_operator, &xor_operator,
		&conditional_operator, &biconditional_operator};
	int indx = get_index_operator(c);
	if (indx == UNRECOGNIZED_SYMBOL) {
		printf("TODO remove\n");
		return (NULL);
	}
	return (operators[indx](tail));
}
