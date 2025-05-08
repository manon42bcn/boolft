/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ccf.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:23:54 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/09 01:53:31 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "boolft.hpp"

#include <string>
#include <sstream>
#include <stack>
#include <vector>
#include <cctype>

// --- Tu AST básico ---
enum Type { VAR, AND, OR, NOT };

struct Node {
	Type type;
	std::string var;    // sólo si type == VAR
	Node* left;         // para NOT, left apunta al literal
	Node* right;        // para NOT, right == nullptr
	Node(Type t, const std::string& v="", Node* l=nullptr, Node* r=nullptr)
			: type(t), var(v), left(l), right(r) {}
};

void collect(Node* node, Type t, std::vector<Node*>& out) {
	if (!node)
		return;
	if (node->type == t) {
		collect(node->left,  t, out);
		collect(node->right, t, out);
	} else {
		out.push_back(node);
	}
}

Node* flatten(Node* node) {
	if (!node || (node->type != AND && node->type != OR))
		return node;
	std::vector<Node*> elems;
	collect(node, node->type, elems);
	if (elems.empty())
		return node;
	// reconstruir lista izquierda-derecha
	Node* res = elems[0];
	for (size_t i = 1; i < elems.size(); ++i) {
		res = new Node(node->type, "", res, elems[i]);
	}
	return res;
}

// Aplica flatten recursivo
Node* normalizeCNF(Node* root) {
	if (!root) return nullptr;
	if (root->type == AND || root->type == OR) {
		root->left  = normalizeCNF(root->left);
		root->right = normalizeCNF(root->right);
		root = flatten(root);
	}
	return root;
}

Node* distribute(Node* a, Node* b) {
	if (b->type == AND) {
		// X ∨ (Y∧Z)
		return new Node(AND, "",
						distribute(a, b->left),
						distribute(a, b->right)
		);
	}
	if (a->type == AND) {
		// (X∧Y) ∨ Z
		return new Node(AND, "",
						distribute(a->left, b),
						distribute(a->right, b)
		);
	}
	// Caso base: ni a ni b son AND
	return new Node(OR, "", a, b);
}

Node* toCNF(Node* node) {
	if (!node || node->type == VAR || node->type == NOT)
		return node;
	// ① Recursión **antes** de nada
	node->left  = toCNF(node->left);
	node->right = toCNF(node->right);

	if (node->type == OR) {
		// ② Aquí sí debes llamar a distribute
		return distribute(node->left, node->right);
	} else {
		// AND: reconstruye explícitamente
		return new Node(AND, "", node->left, node->right);
	}
}

// ------------------------------------------------------------------
// 1) Parseo de un RPN (NNF) a AST
// ------------------------------------------------------------------
Node* parseRPN(const std::string& rpn) {
	std::stack<Node*> st;
	for (char c : rpn) {
		if (c == '&' || c == '|') {
			// operador binario
			if (st.size() < 2)
				throw std::runtime_error("RPN malformed: operador binario sin suficientes operandos");
			Node* right = st.top(); st.pop();
			Node* left  = st.top(); st.pop();
			st.push(new Node(c == '&' ? AND : OR, "", left, right));
		}
		else if (c == '!') {
			// operador unario (se asume que NNF sólo aplica ! a variables)
			Node* lit = st.top(); st.pop();
			st.push(new Node(NOT, "", lit, nullptr));
		}
		else {
			// literal / variable
			std::string op(1, c);
			st.push(new Node(VAR, op));
		}
	}
	return st.empty() ? nullptr : st.top();
}

void toRPN(Node* node, std::vector<std::string>& out) {
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

// ------------------------------------------------------------------
// 2) Función principal: de RPN-entrada → RPN-CNF
// ------------------------------------------------------------------
std::string conjunctive_normal_form(char* formula) {
	// 2.1 Obtenemos la NNF en RPN
	std::string nnf_rpn = negation_normal_form(formula);

	// 2.2 Parseamos esa NNF-RPN a un AST
	try {
		Node* nnf_root = parseRPN(nnf_rpn);

		// 2.3 Convertimos ese AST NNF → AST CNF
		Node* cnf_root = toCNF(nnf_root);
		cnf_root = normalizeCNF(cnf_root);

		// 2.4 Recorremos post-order para emitir RPN de la CNF
		std::vector<std::string> cnf_tokens;
		toRPN(cnf_root, cnf_tokens);

		// 2.5 Concatenamos en un único string separado por espacios
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
