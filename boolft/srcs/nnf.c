/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nnf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 10:46:04 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/05 15:03:39 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "boolft.h"

t_stack *append_str_element(char* exp, size_t bites, t_stack** parent) {
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
	memcpy(element->exp, exp, bites);
	element->len = strlen(exp);
	element->next = NULL;
	return (element);
}

char *pop_str_elem(t_stack** tail) {
	char*	rst = strdup((*tail)->exp);
	t_stack	*node = *tail;
	if ((*tail)->prev)
		(*tail)->prev->next = NULL;
	*tail = (*tail)->prev;
	free(node);
	return (rst);
}

t_stack *convert(char operator, t_stack** tail) {
	if (!*tail)
		return (NULL);
	if (operator == '&') {
		char *exp = pop_str_elem(tail);
		if (!*exp)
			return (NULL);
		char *exp2 = pop_str_elem(tail);
		if (!*exp)
			return (NULL);
	}
	(*tail) = insert_element(a != b, tail);
	return (*tail);
}

int	eval_formula(char* formula) {
	t_stack*	tail = NULL;

	for (size_t i = 0; formula[i]; i++) {
		if (formula[i] >= 'A' && formula[i] <= 'Z') {
			tail = append_str_element(&formula[i], 1, &tail);
			if (!tail)
				return (not_solvable(&tail, ALLOC_ERROR));
		} else {
			t_stack* partial = operate(formula[i], &tail);
			if (!partial)
				return (not_solvable(&tail, NOT_SOLVABLE));
		}
	}
	if (!tail || count_stack(&tail) != 1) {
		return (not_solvable(&tail, NOT_SOLVABLE));
	}
	int result = tail->value;
	clear_stack(&tail);
	return (result);
}
