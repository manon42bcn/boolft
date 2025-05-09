/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ccf.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:23:54 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/09 11:07:59 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "boolft.hpp"

#include <string>
#include <sstream>
#include <stack>
#include <vector>
#include <cctype>

int total_cnf = 0;

void collect_nodes(s_cnf_node* node, e_type type, std::vector<s_cnf_node*>& out) {
	if (!node)
		return;
	if (node->type == type) {
		collect_nodes(node->left,  type, out);
		collect_nodes(node->right, type, out);
	} else {
		out.push_back(node);
	}
}

s_cnf_node* flatten(s_cnf_node* node) {
	if (!node)
		return nullptr;
	if (node->type != AND && node->type != OR)
		return node;
	std::vector<s_cnf_node*> elems;
	collect_nodes(node, node->type, elems);
	if (elems.empty())
		return node;
	s_cnf_node* res = elems[0];
	for (size_t i = 1; i < elems.size(); ++i) {
		res = new s_cnf_node(node->type, "", res, elems[i]);
	}
	return res;
}

s_cnf_node* normalize_cnf(s_cnf_node* root) {
	if (!root) {
		return nullptr;
	}
	total_cnf++;
	if (total_cnf > 1000000) {
		throw BoolFtException("CNF Normalization grew too much.");
	}
	if (root->type == AND || root->type == OR) {
		root->left  = normalize_cnf(root->left);
		root->right = normalize_cnf(root->right);
		root = flatten(root);
	}
	return root;
}

s_cnf_node* distribute(s_cnf_node* a, s_cnf_node* b) {
	if (b->type == AND) {
		// X ∨ (Y∧Z)
		return new s_cnf_node(AND, "",
						distribute(a, b->left),
						distribute(a, b->right)
		);
	}
	if (a->type == AND) {
		// (X∧Y) ∨ Z
		return new s_cnf_node(AND, "",
						distribute(a->left, b),
						distribute(a->right, b)
		);
	}
	// Caso base: ni a ni b son AND
	return new s_cnf_node(OR, "", a, b);
}

size_t total_nodes = 0;

s_cnf_node* toCNF(s_cnf_node* node) {
	if (!node || node->type == VAR || node->type == NOT)
		return node;
	total_nodes++;
	// ① Recursión **antes** de nada
	node->left  = toCNF(node->left);
	node->right = toCNF(node->right);

	if (node->type == OR) {
		// ② Aquí sí debes llamar a distribute
		return distribute(node->left, node->right);
	} else {
		// AND: reconstruye explícitamente
		return new s_cnf_node(AND, "", node->left, node->right);
	}
}

// ------------------------------------------------------------------
// 1) Parseo de un RPN (NNF) a AST
// ------------------------------------------------------------------
s_cnf_node* parseRPN(const std::string& rpn) {
	std::stack<s_cnf_node*> st;
	for (char c : rpn) {
		if (c == '&' || c == '|') {
			// operador binario
			if (st.size() < 2)
				throw BoolFtException("Binary Operator Error.");
			s_cnf_node* right = st.top(); st.pop();
			s_cnf_node* left  = st.top(); st.pop();
			st.push(new s_cnf_node(c == '&' ? AND : OR, "", left, right));
		}
		else if (c == '!') {
			if (st.empty())
				throw BoolFtException("Binary Operator Error.");
			// operador unario (se asume que NNF sólo aplica ! a variables)
			s_cnf_node* lit = st.top(); st.pop();
			st.push(new s_cnf_node(NOT, "", lit, nullptr));
		}
		else {
			// literal / variable
			std::string op(1, c);
			st.push(new s_cnf_node(VAR, op));
		}
	}
	return st.empty() ? nullptr : st.top();
}

void toRPN(s_cnf_node* node, std::vector<std::string>& out) {
	if (!node)
		return;
	if (node->type==VAR) {
		out.push_back(node->var);
	} else if (node->type==NOT) {
		toRPN(node->left, out);
		out.push_back("!");
	} else {
		toRPN(node->left,  out);
		toRPN(node->right, out);
		out.push_back(node->type==AND ? "&" : "|");
	}
}

std::string conjunctive_normal_form(char* formula) {
	std::string nnf_rpn = negation_normal_form(formula);
	try {
		s_cnf_node* nnf_root = parseRPN(nnf_rpn);
		s_cnf_node* cnf_root = toCNF(nnf_root);
		cnf_root = normalize_cnf(cnf_root);
		std::vector<std::string> cnf_tokens;
		toRPN(cnf_root, cnf_tokens);
		std::ostringstream oss;
		for (size_t i = 0; i < cnf_tokens.size(); ++i) {
			if (i) oss << "";
			oss << cnf_tokens[i];
		}
		return oss.str();
	} catch (std::exception& e) {
		throw BoolFtException("The CNF grow too much.");
	}
}
