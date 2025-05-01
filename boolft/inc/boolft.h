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

#define TRUE 1
#define FALSE 0

typedef int eval;
typedef int t_bool;

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

unsigned int	adder(unsigned int a, unsigned int b);
unsigned int	multiplier(unsigned int a, unsigned int b);
int				gray_code(int gray);

void	tree_try(void);
#endif