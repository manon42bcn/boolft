/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_formula.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 17:23:14 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/02 18:54:35 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "boolft.h"

/**
 * @brief Inserts a new element into a doubly linked stack.
 *
 * This function allocates memory for a new stack element, initializes its value,
 * and links it into an existing doubly linked stack structure. The new element is
 * inserted after the current parent element, and all pointers are updated accordingly.
 *
 * @param value The boolean value to be stored in the new stack element.
 * @param parent A double pointer to the current top (or parent) of the stack. If the stack is empty,
 *
 * @return A pointer to the newly created stack element on success, or NULL if memory allocation fails.
 *
 * @note
 *
 * If *parent is NULL, the new element becomes the first element in the stack.
 * This function does not update the external reference to the stack's head or tail; it only
 * returns the pointer to the newly created element.
 *
 * @see t_stack, t_bool
 */
t_stack* insert_element(t_bool value, t_stack** parent) {
	t_stack*	element = (t_stack*)calloc(1, sizeof(t_stack));
	if (!element)
		return (NULL);
	element->prev = *parent;
	if (*parent) {
		(*parent)->next = element;
		element->pos = (*parent)->pos++;
	} else {
		element->pos = 1;
	}
	element->next = NULL;
	element->value = value;
	return (element);
}

/**
 * @brief Removes and returns the value from the top of a doubly linked stack.
 *
 * This function performs three critical operations:
 * 1. Retrieves the boolean value from the current top of the stack
 * 2. Updates stack pointers to maintain structural integrity
 * 3. Safely deallocates the memory of the removed element
 *
 * @param tail Double pointer to the current top (tail) of the stack. After popping,
 *             *tail will point to the previous element in the stack (or NULL if empty).
 *
 * @return The boolean value that was stored in the removed stack element.
 *
 * @warning
 * - **Critical:** The caller must ensure *tail is not NULL before calling this function.
 *   Attempting to pop from an empty stack will result in undefined behavior.
 * - This function permanently modifies the stack structure by removing its top element.
 *
 * @note
 * - The stack is modified in place through the tail pointer parameter
 * - Memory management: This function handles deallocation of the removed element
 * - After execution, the previous element (if any) becomes the new stack top
 *
 * @see insert_element(), t_stack, t_bool
 */
t_bool	pop_element(t_stack** tail) {
	t_bool	rst = (*tail)->value;
	t_stack	*node = *tail;
	if ((*tail)->prev)
		(*tail)->prev->next = NULL;
	*tail = (*tail)->prev;
	free(node);
	return (rst);
}

/**
 * @brief Completely deallocates a doubly linked stack from memory.
 *
 * This function safely destroys an entire stack structure by:
 * 1. Iterating backwards through the stack from the tail element
 * 2. Systematically freeing each node's memory
 * 3. Setting the original tail pointer to NULL
 *
 * @param tail Double pointer to the stack's topmost element (tail). After execution,
 *             *tail will be NULL to prevent dangling pointer references.
 *
 * @note
 * - Handles both empty stacks (no-op) and populated stacks
 * - Time complexity: O(n) where n = number of stack elements
 *
 * @see insert_element(), pop_element(), t_stack
*/
void	clear_stack(t_stack** tail) {
	t_stack* prev = NULL;
	while (*tail) {
		prev = (*tail)->prev;
		free(*tail);
		*tail = prev;
	}
}

/**
 * @brief Counts the number of elements in a doubly linked stack.
 *
 * This function traverses the stack starting from the given tail element,
 * moving backwards through each node via the `prev` pointer, and increments
 * a counter for each element encountered. The process continues until the
 * beginning of the stack (NULL pointer) is reached.
 *
 * @param tail Double pointer to the stack's topmost element (tail). If the stack is empty,
 *             this should be NULL.
 *
 * @return The total number of elements present in the stack.
 *         Returns 0 if the stack is empty.
 *
 * @note
 * - The function does not modify the stack or its elements.
 * - Time complexity is O(n), where n is the number of elements in the stack.
 * - This function is safe to call on an empty stack (NULL pointer).
 *
 * @see insert_element(), pop_element(), clear_stack(), t_stack
 * @todo this method can be removed when its clear that the position method
 * can be used without doubt
 */
int	count_stack(t_stack** tail) {
	int i = 0;
	t_stack* elem = *tail;
	while (elem) {
		i++;
		elem = elem->prev;
	}
	return (i);
}

/**
 * @brief Clears the stack and returns a specified error code.
 *
 * This utility function is intended for error-handling scenarios where an unrecoverable
 * condition has been detected during stack operations or logical evaluation. It ensures
 * that any dynamically allocated memory associated with the stack is properly freed,
 * preventing memory leaks before propagating the error code up the call stack.
 *
 * @param tail Double pointer to the stack's topmost element (tail). This allows the function
 *             to clear the entire stack using @ref clear_stack and set the caller's stack
 *             pointer to NULL.
 * @param error The integer error code to be returned. This value typically represents the
 *              specific error condition encountered by the calling function.
 *
 * @return The same error code provided as input, allowing for streamlined error propagation.
 *
 * @note
 * - This function is designed to be called when an operation cannot be completed due to
 *   a fatal error (e.g., invalid input, memory allocation failure, or logical inconsistency).
 * - After this function is called, the stack is guaranteed to be empty and the pointer set to NULL.
 *
 * @see clear_stack(), t_stack, macros: UNRECOGNIZED_SYMBOL, NOT_SOLVABLE, ALLOC_ERROR
 */
int not_solvable(t_stack** tail, int error) {
	clear_stack(tail);
	return (error);
}

/**
 * @brief Evaluates a logical formula in Reverse Polish Notation (RPN).
 *
 * This function parses and evaluates a logical formula provided as a string
 * in Reverse Polish Notation (postfix notation). It uses a stack to compute
 * the result by pushing operands ('1' for true, '0' for false) onto the stack
 * and applying logical operators using the operate() function.
 *
 * @param[in] formula Null-terminated string representing the logical formula
 *                    in RPN, using '1' and '0' for boolean values and supported
 *                    operator symbols (e.g., '!', '&', '|', '^', '>', '=').
 *
 * @return The result of the formula evaluation (typically 0 for false, 1 for true),
 *         or an error code if the formula is invalid or an allocation error occurs.
 *
 * @retval ALLOC_ERROR If a memory allocation error occurs during stack operations.
 * @retval NOT_SOLVABLE If the formula is invalid or cannot be evaluated.
 *
 * @note The stack is expected to be empty or contain a single result at the end of evaluation.
 */
int	eval_formula(char* formula) {
	t_stack*	tail = NULL;

	for (size_t i = 0; formula[i]; i++) {
		if (formula[i] == '1' || formula[i] == '0') {
			tail = insert_element(TRUE ? formula[i] == '1': FALSE, &tail);
			if (!tail)
				return (not_solvable(&tail, ALLOC_ERROR));
		} else {
			t_stack* partial = operate(formula[i], &tail);
			if (!partial)
				return (not_solvable(&tail, NOT_SOLVABLE));
		}
	}
	if (!tail || tail->pos != 1)
		return (not_solvable(&tail, NOT_SOLVABLE));
	int result = tail->value;
	clear_stack(&tail);
	return (result);
}
