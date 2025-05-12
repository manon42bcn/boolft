/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boolft.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:06:02 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/12 11:21:58 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once
#include <iostream>
#include <stack>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <iostream>
#include <cstring>

class BoolFtException : public std::exception {
private:
	std::string _message;

public:
	explicit BoolFtException(const std::string& message) : _message(message) {};
	explicit BoolFtException(const char* message) : _message(message) {};
	explicit BoolFtException() : _message("BoolFT Error.") {};
	BoolFtException(const BoolFtException& other) : _message(other._message) {}
	virtual ~BoolFtException() throw() {};
	virtual const char* what() const throw() {
		return (_message.c_str());
	}
};



#define BOOL_TO_STRING(x) ((x) < 0 ? "ERROR" : ((x) ? "true" : "false"))
#define BOOL_TO_CHAR(x) ((x) ? '1' : '0')
#define CASE_IDX			0
#define TOTAL_CASES			1

using t_set = std::vector<int>;
using t_pwr_set = std::vector<t_set>;

enum e_type { VAR, AND, OR, NOT };
struct s_cnf_node {
	e_type type;
	std::string var;
	s_cnf_node* left;
	s_cnf_node* right;
	s_cnf_node(e_type t, const std::string& v="", s_cnf_node* l=nullptr, s_cnf_node* r=nullptr)
			: type(t), var(v), left(l), right(r) {}
};

class CNF_Stack {
	private:
		std::vector<s_cnf_node*>	built;
		int 						total_nodes;
	public:
		CNF_Stack() {};
		~CNF_Stack();
		s_cnf_node* new_node(e_type t, const std::string& v="", s_cnf_node* l=nullptr, s_cnf_node* r=nullptr);
};

// --- Multiple functions utilities
bool 			is_operator (char c);

// --- Exercises functions
unsigned int	adder(unsigned int a, unsigned int b);
unsigned int	multiplier(unsigned int a, unsigned int b);
int				gray_code(int n);
bool			eval_formula(char* formula);
void			print_truth_table(char *function);
std::string		negation_normal_form(char *formula);
std::string		conjunctive_normal_form(char* formula);
bool			sat(char *function);
t_pwr_set		powerset(std::vector<int> set);
t_set			eval_set(char* formula, std::vector<t_set> sets);
double			map(uint32_t x, uint32_t y);
void			reverse_map(double t, uint32_t &x, uint32_t &y);
//
//#define TRUE				1
//#define FALSE				0
//#define CASE_IDX			0
//#define TOTAL_CASES			1
//#define UNRECOGNIZED_SYMBOL	-1
//#define NOT_SOLVABLE		-2
//#define ALLOC_ERROR			-3
//#define BUF_SIZE			1024
//
//#define BOOL_TO_STRING(x) ((x) < 0 ? "ERROR" : ((x) ? "true" : "false"))
//#define BOOL_TO_CHAR(x) ((x) ? '1' : '0')
//#define SWITCH_BOOL(x) ((x) ? FALSE : TRUE)
//
//typedef int eval;
//typedef unsigned char t_bool;
//
//typedef enum e_node_mode {
//	LOGIC_VALUE = 0,
//	LOGIC_SYMBOL = 1
//} t_node_mode;
//
//typedef struct s_node {
//	char			symbol;
//	eval			eval;
//	t_node_mode		mode;
//	struct s_node*	left;
//	struct s_node*	right;
//} t_node;
//
//typedef struct s_stack {
//	struct s_stack*	prev;
//	struct s_stack*	next;
//	size_t 			len;
//	t_bool			value;
//	int				pos;
//	char			operand;
//	char			exp[BUF_SIZE];
//} t_stack;
//
//typedef struct s_var {
//	char	var;
//	int		idx;
//	t_bool	value;
//} t_var;
//
//typedef t_bool (*t_op)();
//
//
//typedef t_stack* (*t_operator)(t_stack**);
//
//unsigned int	adder(unsigned int a, unsigned int b);
//unsigned int	multiplier(unsigned int a, unsigned int b);
//int				gray_code(int gray);
//int				eval_formula(char* formula);
//void			print_truth_table(char *function);
//// Stack prototypes
//t_stack*		insert_element(t_bool value, t_stack** parent);
//t_bool			pop_element(t_stack** tail);
//int				get_index_operator(char c);
//void			clear_stack(t_stack** tail);
//int				count_stack(t_stack** tail);
//int				not_solvable(t_stack** tail, int error);
//t_stack*		operate(char c, t_stack **tail);
