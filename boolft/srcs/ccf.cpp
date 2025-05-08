/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ccf.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:23:54 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/07 18:03:26 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

///* ************************************************************************** */
///*                                                                            */
///*                                                        :::      ::::::::   */
///*   ccf.cpp                                            :+:      :+:    :+:   */
///*                                                    +:+ +:+         +:+     */
///*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
///*                                                +#+#+#+#+#+   +#+           */
///*   Created: 2025/05/07 17:23:54 by mporras-          #+#    #+#             */
///*   Updated: 2025/05/07 17:24:13 by mporras-         ###   ########.fr       */
///*                                                                            */
///* ************************************************************************** */
//
//#include "boolft.hpp"
//#include <string>
//#include <vector>
//#include <memory>
//#include <stdexcept>
//#include <cctype>
//
//// --- 1) Estructura de nodo AST ---
//struct Node {
//	char op;  // 'A'..'Z' para variable, '!' unario, '&'|'|' binarios
//	std::shared_ptr<Node> left, right;
//	Node(char c)             : op(c), left(nullptr),  right(nullptr) {} // var
//	Node(char c, Ptr l)      : op(c), left(l),         right(nullptr) {} // unario
//	Node(char c, Ptr l, Ptr r): op(c), left(l),        right(r)   {}     // binario
//};
//using Ptr = std::shared_ptr<Node>;
//
//// --- 2) Parsear RPN (sólo !, &, |) a AST ---
//Ptr parse_nnf_rpn(const std::string& rpn) {
//	std::vector<Ptr> st;
//	for (char c : rpn) {
//		if (std::isupper(c)) {
//			st.push_back(std::make_shared<Node>(c));
//		}
//		else if (c == '!') {
//			if (st.empty()) throw std::runtime_error("Unexpected '!'");
//			Ptr a = st.back(); st.pop_back();
//			st.push_back(std::make_shared<Node>('!', a));
//		}
//		else if (c=='&' || c=='|') {
//			if (st.size()<2) throw std::runtime_error("Not enough operands for binary");
//			Ptr b = st.back(); st.pop_back();
//			Ptr a = st.back(); st.pop_back();
//			st.push_back(std::make_shared<Node>(c, a, b));
//		}
//		else {
//			throw std::runtime_error(std::string("Invalid token in NNF RPN: ") + c);
//		}
//	}
//	if (st.size()!=1) throw std::runtime_error("Invalid NNF RPN");
//	return st[0];
//}
//
//// --- 3) Distribuir OR sobre AND ---
//Ptr distribute_or(const Ptr& root) {
//	if (!root) return nullptr;
//	// Postorden
//	if (root->left)  root->left  = distribute_or(root->left);
//	if (root->right) root->right = distribute_or(root->right);
//
//	if (root->op == '|') {
//		// (X & Y) | Z  =>  (X|Z) & (Y|Z)
//		if (root->left->op == '&') {
//			Ptr X = root->left->left, Y = root->left->right, Z = root->right;
//			Ptr a = std::make_shared<Node>('|', X, Z);
//			Ptr b = std::make_shared<Node>('|', Y, Z);
//			return std::make_shared<Node>('&',
//										  distribute_or(a),
//										  distribute_or(b));
//		}
//		// X | (Y & Z)  =>  (X|Y) & (X|Z)
//		if (root->right->op == '&') {
//			Ptr X = root->left, Y = root->right->left, Z = root->right->right;
//			Ptr a = std::make_shared<Node>('|', X, Y);
//			Ptr b = std::make_shared<Node>('|', X, Z);
//			return std::make_shared<Node>('&',
//										  distribute_or(a),
//										  distribute_or(b));
//		}
//	}
//	return root;
//}
//
//// --- 4) Iterar hasta estabilidad ---
//Ptr to_cnf_ast(Ptr root) {
//	Ptr prev;
//	do {
//		prev = root;
//		root = distribute_or(root);
//	} while (root != prev);
//	return root;
//}
//
//// --- 5) Volver a RPN con postorden ---
//void to_rpn(const Ptr& root, std::string &out) {
//	if (!root) return;
//	to_rpn(root->left, out);
//	to_rpn(root->right, out);
//	out.push_back(root->op);
//}
//
//// --- Función final que encadena todo ---
//// Recibe la fórmula original en RPN (con >,=,^,…),
//// primero la normaliza (NNF) y luego la convierte a CNF.
//std::string full_rpn_to_cnf(char *formula) {
//	// 1) NNF: sólo !, &, |
//	std::string nnf = negation_normal_form(formula);
//
//	// 2) AST
//	Ptr ast = parse_nnf_rpn(nnf);
//
//	// 3) Distribución y estabilidad
//	Ptr cnf_ast = to_cnf_ast(ast);
//
//	// 4) RPN de salida
//	std::string cnf_rpn;
//	to_rpn(cnf_ast, cnf_rpn);
//	return cnf_rpn;
//}
