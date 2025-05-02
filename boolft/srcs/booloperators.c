#include "boolft.h"


static t_stack* not_operator(t_stack** tail) {
	if (!*tail)
		return (NULL);
	t_bool	operand = pop_element(tail);
	(*tail) = insert_element(!operand, tail);
	return (*tail);
}

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

static int	get_index_operator(char c) {
	static char operators[] = {'!', '&', '|', '^', '>', '='};
	for (int i = 0; operators[i]; i++) {
		if (c == operators[i])
			return (i);
	}
	return (UNRECOGNIZED_SYMBOL);
}

t_stack*	operate(char c, t_stack **tail) {
	static	t_operator	(operators)[] = {
		&not_operator, &and_operator,
		&or_operator, &xor_operator,
		&conditional_operator, &biconditional_operator};
	int indx = get_index_operator(c);
	if (indx == UNRECOGNIZED_SYMBOL)
		return (NULL);
	return (operators[indx](tail));
}
