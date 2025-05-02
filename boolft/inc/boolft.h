/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boolft.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:28:35 by mporras-          #+#    #+#             */
/*   Updated: 2025/04/30 17:29:51 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOOLFT_H
#define BOOLFT_H
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define TRUE				1
#define FALSE				0
#define UNRECOGNIZED_SYMBOL	-1
#define NOT_SOLVABLE		-2
#define ALLOC_ERROR			-3

#define BOOL_TO_STRING(x) ((x) < 0 ? "ERROR" : ((x) ? "true" : "false"))
#define BOOL_TO_CHAR(x) ((x) ? '1' : '0')

typedef int eval;
typedef unsigned char t_bool;

typedef enum e_node_mode {
	LOGIC_VALUE = 0,
	LOGIC_SYMBOL = 1
} t_node_mode;

typedef struct s_node {
	char			symbol;
	eval			eval;
	t_node_mode		mode;
	struct s_node*	left;
	struct s_node*	right;
} t_node;

typedef struct s_stack {
	struct s_stack*	prev;
	struct s_stack*	next;
	t_bool			value;
	int				pos;
} t_stack;

typedef struct s_var {
	char	var;
	int		idx;
	t_bool	value;
} t_var;

typedef t_bool (*t_op)();


typedef t_stack* (*t_operator)(t_stack**);

unsigned int	adder(unsigned int a, unsigned int b);
unsigned int	multiplier(unsigned int a, unsigned int b);
int				gray_code(int gray);
int				eval_formula(char* formula);
void			print_truth_table(char *function);
// Stack prototypes
t_stack*		insert_element(t_bool value, t_stack** parent);
t_bool			pop_element(t_stack** tail);
int				get_index_operator(char c);
t_stack*		operate(char c, t_stack **tail);


#endif