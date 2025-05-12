/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boolft.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:06:02 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/12 21:51:48 by mporras-         ###   ########.fr       */
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

#define BOOL_TO_STRING(x) ((x) < 0 ? "ERROR" : ((x) ? "true" : "false"))
#define BOOL_TO_CHAR(x) ((x) ? '1' : '0')
#define CASE_IDX			0
#define TOTAL_CASES			1

using t_set = std::vector<int>;
using t_pwr_set = std::vector<t_set>;

/**
 * @class BoolFtException
 * @brief Custom exception class for handling errors within BoolFT project
 *
 * This exception class provides a standard way to handle errors by allowing custom messages.
 * It inherits from `std::exception` and overrides the `what()` method to return a message.
 *
 * @details
 * - Can be initialized with either `std::string` or `const char*` for message flexibility.
 * - The destructor is `noexcept`, ensuring that it does not throw exceptions during destruction.
 *
 * ### Public Methods
 * - `BoolFtException(const std::string& message)`: Constructor that accepts a custom error message.
 * - `BoolFtException(const char* message)`: Constructor for C-string error messages.
 * - `const char* what() const throw()`: Returns the error message.
 *
 * @note Use this exception throughout the project to ensure consistent error handling.
 *
 * @example
 * ```
 * throw BoolFtException("An unexpected error occurred.");
 * ```
 */
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

/**
 * @brief Enumeration of possible node types in a CNF expression tree.
 *
 * @details
 * This enum defines the kinds of nodes that can appear in the binary tree
 * representation of a logical formula during CNF conversion:
 *   - VAR : A leaf node representing a propositional variable.
 *   - AND : A binary conjunction operator node (logical ∧).
 *   - OR  : A binary disjunction operator node (logical ∨).
 *   - NOT : A unary negation operator node (logical ¬).
 */
enum e_type { VAR, AND, OR, NOT };

/**
 * @brief Node structure for a CNF expression tree.
 *
 * @details
 * Each `s_cnf_node` instance represents either an operator (AND, OR, NOT)
 * or a variable in the expression.  Operator nodes have pointers to their
 * child(ren), while variable nodes store the variable name in `var`.
 *
 * The tree structure facilitates recursive transformations during CNF
 * conversion (e.g., distribution, flattening) and serialization to RPN.
 */
struct s_cnf_node {
	e_type type;
	std::string var;
	s_cnf_node* left;
	s_cnf_node* right;
	/**
     * @brief Constructs a new CNF tree node.
     *
     * @param[in] t  The node type (VAR, AND, OR, NOT).
     * @param[in] v  The variable name for VAR nodes; ignored for operators.
     * @param[in] l  Pointer to the left child node (or operand for NOT).
     * @param[in] r  Pointer to the right child node (nullptr for VAR and NOT).
     */
	s_cnf_node(e_type t, const std::string& v="", s_cnf_node* l=nullptr, s_cnf_node* r=nullptr)
			: type(t), var(v), left(l), right(r) {}
};

/**
 * @brief Manages allocation and automatic cleanup of CNF expression tree nodes.
 *
 * @details
 * The CNF_Stack class acts as a simple memory arena for `s_cnf_node` objects.
 * Each time `new_node()` is called, the newly created node pointer is stored
 * in the internal `built` vector. On destruction, all allocated nodes are
 * deleted to prevent memory leaks. Additionally, `total_nodes` tracks the
 * number of nodes created, which can be useful for diagnostics or optimizations.
 */
class CNF_Stack {
	private:
		std::vector<s_cnf_node*>	built;
		int 						total_nodes;
	public:
		CNF_Stack() {
			total_nodes = 0;
		};
		~CNF_Stack();
		s_cnf_node* new_node(e_type t, const std::string& v="",
							 s_cnf_node* l=nullptr, s_cnf_node* r=nullptr);
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
